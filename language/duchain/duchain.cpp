/* This  is part of KDevelop
    Copyright 2006-2008 Hamish Rodda <rodda@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "duchain.h"
#include "duchainlock.h"

#include <QCoreApplication>
#include <QHash>
#include <QMultiMap>
#include <qatomic.h>

#include <kglobal.h>

#include <KTextEditor/Document>

#include "editorintegrator.h"

#include "topducontext.h"
#include "parsingenvironment.h"
#include "declaration.h"
#include "definitions.h"
#include "duchainutils.h"
#include "use.h"
#include "uses.h"
#include "symboltable.h"
#include "abstractfunctiondeclaration.h"
#include "smartconverter.h"
#include "backgroundparser.h"
#include <idocumentcontroller.h>
#include <icore.h>
#include <ilanguage.h>
#include <ilanguagecontroller.h>
#include <ilanguagesupport.h>
#include <icodehighlighting.h>
#include "duchainutils.h"
#include "repositories/itemrepository.h"

namespace KDevelop
{
class DUChainPrivate;
static DUChainPrivate* duChainPrivateSelf = 0;
class DUChainPrivate
{
public:
  DUChainPrivate() : instance(0)             
  {
    duChainPrivateSelf = this;
    qRegisterMetaType<DUChainBasePointer>("KDevelop::DUChainBasePointer");
    qRegisterMetaType<DUContextPointer>("KDevelop::DUContextPointer");
    qRegisterMetaType<TopDUContextPointer>("KDevelop::TopDUContextPointer");
    qRegisterMetaType<DeclarationPointer>("KDevelop::DeclarationPointer");
    qRegisterMetaType<UsePointer>("KDevelop::UsePointer");
    qRegisterMetaType<FunctionDeclarationPointer>("KDevelop::FunctionDeclarationPointer");
    //qRegisterMetaType<DUChainObserver::Modification>("KDevelop::DUChainObserver::Modification");
    //qRegisterMetaType<DUChainObserver::Relationship>("KDevelop::DUChainObserver::Relationship");
    qRegisterMetaType<Problem>("KDevelop::Problem");

    notifier = new DUChainObserver();
    instance = new DUChain();
  }
  ~DUChainPrivate() {
    delete instance;
  }
  
//   void storeChain(const IdentifiedFile& file) {
//     Q_ASSERT(m_chains.contains(file));
//     TopDUContext* top = m_chains[file];
//     top->m_dynamicData->store();
//   }
//   
//   void loadChain(const IdentifiedFile& file) {
//     Q_ASSERT(!m_chains.contains(file));
//   }

  DUChain* instance;
  DUChainLock lock;
  QMap<IdentifiedFile, TopDUContext*> m_chains;
  QHash<uint, TopDUContext*> m_chainsByIndex;
  QMultiMap<IndexedString, ParsingEnvironmentFilePointer> m_fileEnvironmentInformations;
  DUChainObserver* notifier;
  Definitions m_definitions;
  Uses m_uses;
};

K_GLOBAL_STATIC(DUChainPrivate, sdDUChainPrivate)


DUChain::DUChain()
{
  SymbolTable::self(); //Initialize singleton

  connect(EditorIntegrator::notifier(), SIGNAL(documentAboutToBeDeleted(KTextEditor::Document*)), SLOT(documentAboutToBeDeleted(KTextEditor::Document*)));
  if(ICore::self()) {
    Q_ASSERT(ICore::self()->documentController());
    connect(ICore::self()->documentController(), SIGNAL(documentLoadedPrepare(KDevelop::IDocument*)), this, SLOT(documentLoadedPrepare(KDevelop::IDocument*)));
    connect(ICore::self()->documentController(), SIGNAL(documentActivated(KDevelop::IDocument*)), this, SLOT(documentActivated(KDevelop::IDocument*)));
  }
}

DUChain::~DUChain()
{
}

DUChain* DUChain::self()
{
  return sdDUChainPrivate->instance;
}

DUChainLock* DUChain::lock()
{
  return &sdDUChainPrivate->lock;
}

QList<TopDUContext*> DUChain::allChains() const
{
  return sdDUChainPrivate->m_chains.values();
}

void DUChain::updateContextEnvironment( TopDUContext* context, ParsingEnvironmentFile* file ) {
  ENSURE_CHAIN_WRITE_LOCKED

  removeFromEnvironmentManager( context );

  if( context->parsingEnvironmentFile() )
    sdDUChainPrivate->m_chains.remove( context->parsingEnvironmentFile()->identity() );

  context->setParsingEnvironmentFile( file );

  sdDUChainPrivate->m_chains.insert( context->parsingEnvironmentFile()->identity(), context );

  addToEnvironmentManager( context );

  branchModified(context);
}


void DUChain::removeDocumentChain( const  IdentifiedFile& document )
{
  ENSURE_CHAIN_WRITE_LOCKED

  kDebug(9505) << "duchain: removing document" << document.toString();
  if (sdDUChainPrivate->m_chains.contains(document)) {
    TopDUContext* context = sdDUChainPrivate->m_chains.take(document);
    uint index = context->ownIndex();

    if (context->smartRange())
      ICore::self()->languageController()->backgroundParser()->removeManagedTopRange(context->smartRange());

    branchRemoved(context);

    context->deleteSelf();
    
    sdDUChainPrivate->m_chainsByIndex.remove( index );
  }
}

void DUChain::addDocumentChain( const IdentifiedFile& document, TopDUContext * chain )
{
  ENSURE_CHAIN_WRITE_LOCKED

  kDebug(9505) << "duchain: adding document" << document.toString() << " " << chain;
  Q_ASSERT(chain);
  if (chain->smartRange()) {
    Q_ASSERT(!chain->smartRange()->parentRange());
    ICore::self()->languageController()->backgroundParser()->addManagedTopRange(KUrl(document.url().str()), chain->smartRange());
  }

  if(chainForDocument(document)) {
    ///@todo practically this will result in lost memory, we will currently never delete the overwritten chain. Care about such stuff.
    kDebug(9505) << "duchain: error: A document with the same identity is already in the du-chain";
  }

//   {
//     ///Remove obsolete versions of the document
//     IdentifiedFile firstDoc( document.url(), 0 );
//     QMap<IdentifiedFile, TopDUContext*>::Iterator it = sdDUChainPrivate->m_chains.lowerBound(firstDoc);
//
//     ModificationRevision rev = EditorIntegrator::modificationRevision( document.url() );
//
//     for( ;it != sdDUChainPrivate->m_chains.end() && it.key().url() == document.url(); )
//     {
//       ModificationRevision thisRev = (*it)->parsingEnvironmentFile()->modificationRevision();
//
//       if( (*it)->parsingEnvironmentFile() && !(thisRev == rev) )
//       {
//       } else {
//         kDebug(9505) << "duchain: leaving other version of document " << (*it)->parsingEnvironmentFile()->identity().toString() << " in du-chain. Current rev.: " << rev << " document's rev.: " << (*it)->parsingEnvironmentFile()->modificationRevision();
//       }
//       ++it;
//     }
//   }

  sdDUChainPrivate->m_chains.insert(document, chain);
  sdDUChainPrivate->m_chainsByIndex.insert(chain->ownIndex(), chain);

  {
    //This is just for debugging, and should be disabled later.
    int realChainCount = 0;
    int proxyChainCount = 0;
    for(QMap<IdentifiedFile, TopDUContext*>::const_iterator it = sdDUChainPrivate->m_chains.begin(); it != sdDUChainPrivate->m_chains.end(); ++it) {
      if((*it)->flags() & TopDUContext::ProxyContextFlag)
        ++proxyChainCount;
      else
        ++realChainCount;
    }

    kDebug(9505) << "new count of real chains: " << realChainCount << " proxy-chains: " << proxyChainCount;
  }
  chain->setInDuChain(true);
  addToEnvironmentManager(chain);

  //contextChanged(0L, DUChainObserver::Addition, DUChainObserver::ChildContexts, chain);
  branchAdded(chain);
}

void DUChain::addToEnvironmentManager( TopDUContext * chain ) {
  ENSURE_CHAIN_WRITE_LOCKED

  ParsingEnvironmentFilePointer file = chain->parsingEnvironmentFile();
  if( !file )
    return; //We don't need to manage

  sdDUChainPrivate->m_fileEnvironmentInformations.insert(chain->url(), file);
}

void DUChain::removeFromEnvironmentManager( TopDUContext * chain ) {
  ENSURE_CHAIN_WRITE_LOCKED

  ParsingEnvironmentFilePointer file = chain->parsingEnvironmentFile();
  if( !file )
    return; //We don't need to manage

  sdDUChainPrivate->m_fileEnvironmentInformations.remove(chain->url(), file);
}

TopDUContext* DUChain::chainForDocument(const KUrl& document) const {
  return chainForDocument(IdentifiedFile(document));
}

TopDUContext* DUChain::chainForDocument(const HashedString& document) const {
  return chainForDocument( IdentifiedFile(document) );
}

TopDUContext* DUChain::chainForIndex(uint index) const {
  QHash<uint, TopDUContext*>::const_iterator it = sdDUChainPrivate->m_chainsByIndex.find(index);
  if(it != sdDUChainPrivate->m_chainsByIndex.end())
    return *it;
  else
    return 0;
}

TopDUContext * DUChain::chainForDocument( const IdentifiedFile & document ) const
{
  if (!document.identity()) {
    {
      int count = 0;
      QMap<IdentifiedFile, TopDUContext*>::Iterator it = sdDUChainPrivate->m_chains.lowerBound(document);
      for( ; it != sdDUChainPrivate->m_chains.end() && it.key().url() == document.url(); ++it )
        ++count;
      if( count > 1 )
        kDebug(9505) << "found " << count << " chains for " << document.url().str();

    }
    // Match any parsed version of this document
    QMap<IdentifiedFile, TopDUContext*>::Iterator it = sdDUChainPrivate->m_chains.lowerBound(document);
    if (it != sdDUChainPrivate->m_chains.constEnd())
      if (it.key().url() == document.url())
        return it.value();

  } else if (sdDUChainPrivate->m_chains.contains(document))
    return sdDUChainPrivate->m_chains[document];

  kDebug(9505) << "No chain found for document " << document.toString();

  return 0;
}

QList<TopDUContext*> DUChain::chainsForDocument(const KUrl& document) const
{
  return chainsForDocument(HashedString(document.pathOrUrl()));
}

QList<TopDUContext*> DUChain::chainsForDocument(const HashedString& document) const
{
  QList<TopDUContext*> chains;

  // Match all parsed versions of this document
  for (QMap<IdentifiedFile, TopDUContext*>::Iterator it = sdDUChainPrivate->m_chains.lowerBound(IdentifiedFile(document)); it != sdDUChainPrivate->m_chains.constEnd(); ++it) {
    if (it.key().url() == IndexedString(document.str()))
      chains << it.value();
    else
      break;
  }

  return chains;
}

TopDUContext* DUChain::chainForDocument( const KUrl& document, const ParsingEnvironment* environment, TopDUContext::Flags flags ) const {
  return chainForDocument( HashedString(document.pathOrUrl()), environment, flags );
}
TopDUContext* DUChain::chainForDocument( const HashedString& document, const ParsingEnvironment* environment, TopDUContext::Flags flags ) const {

  IndexedString indexed(document.str());

  QMultiMap<IndexedString, ParsingEnvironmentFilePointer>::const_iterator it = sdDUChainPrivate->m_fileEnvironmentInformations.lowerBound(indexed);
  while(it != sdDUChainPrivate->m_fileEnvironmentInformations.end() && it.key() == indexed) {
    if(*it) {
      ///@todo don't load the context just for testing the flags, store the flags in the parsing-environment file
      TopDUContext* ctx = DUChain::self()->chainForDocument((*it)->identity());
      if(flags == TopDUContext::AnyFlag || (ctx && ctx->flags() == flags)) {
        if((*it)->matchEnvironment(environment))
          return ctx;
      }
    }
    ++it;
  }

  return 0;
}

void DUChain::clear()
{
  DUChainWriteLocker writeLock(lock());
  foreach (TopDUContext* context, sdDUChainPrivate->m_chains) {
    if( context->smartRange() )
      KDevelop::EditorIntegrator::releaseTopRange(context->smartRange());
    delete context;
  }

  sdDUChainPrivate->m_fileEnvironmentInformations.clear();
  sdDUChainPrivate->m_chains.clear();
  sdDUChainPrivate->m_chainsByIndex.clear();
}

DUChainObserver* DUChain::notifier()
{
  return sdDUChainPrivate->notifier;
}

void DUChain::branchAdded(DUContext* context)
{
  emit sdDUChainPrivate->notifier->branchAdded(DUContextPointer(context));
}

void DUChain::branchModified(DUContext* context)
{
  emit sdDUChainPrivate->notifier->branchModified(DUContextPointer(context));
}

void DUChain::branchRemoved(DUContext* context)
{
  emit sdDUChainPrivate->notifier->branchRemoved(DUContextPointer(context));
}

QList<KUrl> DUChain::documents() const
{
  QList<KUrl> ret;
  foreach (const IdentifiedFile& file, sdDUChainPrivate->m_chains.keys()) {
    ret << KUrl(file.url().str());
  }
  return ret;
}

void DUChain::documentActivated(KDevelop::IDocument* doc)
{
  //Check whether the document has an attached environment-manager, and whether that one thinks the document needs to be updated.
  //If yes, update it.
  DUChainWriteLocker lock( DUChain::lock() );
  TopDUContext* ctx = DUChainUtils::standardContextForUrl(doc->url());
  if(ctx && ctx->parsingEnvironmentFile())
    if(ctx->parsingEnvironmentFile()->needsUpdate())
      ICore::self()->languageController()->backgroundParser()->addDocument(doc->url());
}

void DUChain::documentAboutToBeDeleted(KTextEditor::Document* doc)
{
  QList<TopDUContext*> chains = chainsForDocument(doc->url());

  EditorIntegrator editor;
  SmartConverter sc(&editor);

  foreach (TopDUContext* top, chains) {
    DUChainWriteLocker lock( DUChain::lock() );
    sc.deconvertDUChain( top );
  }
}

void DUChain::documentLoadedPrepare(KDevelop::IDocument* doc)
{
  // Convert any duchains to the smart equivalent first
  EditorIntegrator editor;
  SmartConverter sc(&editor);

  QList<TopDUContext*> chains = chainsForDocument(doc->url());

  foreach (TopDUContext* chain, chains) {
    DUChainWriteLocker lock( DUChain::lock() );
    sc.convertDUChain(chain);
    if(chain->smartRange())
      ICore::self()->languageController()->backgroundParser()->addManagedTopRange(doc->url(), chain->smartRange());
  }

  QList<KDevelop::ILanguage*> languages = ICore::self()->languageController()->languagesForUrl(doc->url());

  TopDUContext* standardContext = DUChainUtils::standardContextForUrl(doc->url());
  if(standardContext) {
    foreach( KDevelop::ILanguage* language, languages)
      if(language->languageSupport() && language->languageSupport()->codeHighlighting())
        language->languageSupport()->codeHighlighting()->highlightDUChain(standardContext);
  }

  //Since there may be additional information like include-paths available, always reparse opened documents
  ICore::self()->languageController()->backgroundParser()->addDocument(doc->url());
}

Uses* DUChain::uses()
{
  return &sdDUChainPrivate->m_uses;
}

Definitions* DUChain::definitions()
{
  return &sdDUChainPrivate->m_definitions;
}

uint DUChain::newTopContextIndex() {
  static QAtomicInt currentId(1);
  return currentId.fetchAndAddRelaxed(1);
}


}

#include "duchain.moc"

// kate: space-indent on; indent-width 2; tab-width 4; replace-tabs on; auto-insert-doxygen on
