//This is file has been generated by xmltokross, you should not edit this file but the files used to generate it.

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <kross/core/manager.h>
#include <kross/core/wrapperinterface.h>
#include <language/duchain/ducontext.h>
#include <language/duchain/topducontext.h>

class KrossKDevelopIndexedDUContext : public QObject, public Kross::WrapperInterface
{
	Q_OBJECT
	public:
		KrossKDevelopIndexedDUContext(KDevelop::IndexedDUContext* obj, QObject* parent=0) : QObject(parent), wrapped(obj)		{ setObjectName("KDevelop::IndexedDUContext"); }
		void* wrappedObject() const { return wrapped; }

		Q_SCRIPTABLE KDevelop::DUContext* context() const { return wrapped->context(); }
		Q_SCRIPTABLE KDevelop::DUContext* data() const { return wrapped->data(); }
		Q_SCRIPTABLE bool operator==(const KDevelop::IndexedDUContext& x0) const { return wrapped->operator==(x0); }
		Q_SCRIPTABLE uint hash() const { return wrapped->hash(); }
		Q_SCRIPTABLE bool isValid() const { return wrapped->isValid(); }
		Q_SCRIPTABLE bool operator<(const KDevelop::IndexedDUContext& x0) const { return wrapped->operator<(x0); }
		Q_SCRIPTABLE uint localIndex() const { return wrapped->localIndex(); }
		Q_SCRIPTABLE uint topContextIndex() const { return wrapped->topContextIndex(); }
		Q_SCRIPTABLE KDevelop::IndexedTopDUContext indexedTopContext() const { return wrapped->indexedTopContext(); }
		Q_SCRIPTABLE void setIsDummy(bool x0) { wrapped->setIsDummy(x0); }
		Q_SCRIPTABLE bool isDummy() const { return wrapped->isDummy(); }
		Q_SCRIPTABLE QPair< uint, uint > dummyData() const { return wrapped->dummyData(); }
		Q_SCRIPTABLE void setDummyData(QPair< uint, uint > x0) { wrapped->setDummyData(x0); }
	private:
		KDevelop::IndexedDUContext* wrapped;
};

class KrossKDevelopLocalIndexedDUContext : public QObject, public Kross::WrapperInterface
{
	Q_OBJECT
	public:
		KrossKDevelopLocalIndexedDUContext(KDevelop::LocalIndexedDUContext* obj, QObject* parent=0) : QObject(parent), wrapped(obj)		{ setObjectName("KDevelop::LocalIndexedDUContext"); }
		void* wrappedObject() const { return wrapped; }

		Q_SCRIPTABLE KDevelop::DUContext* data(KDevelop::TopDUContext* x0) const { return wrapped->data(x0); }
		Q_SCRIPTABLE bool operator==(const KDevelop::LocalIndexedDUContext& x0) const { return wrapped->operator==(x0); }
		Q_SCRIPTABLE bool isValid() const { return wrapped->isValid(); }
		Q_SCRIPTABLE uint hash() const { return wrapped->hash(); }
		Q_SCRIPTABLE bool operator<(const KDevelop::LocalIndexedDUContext& x0) const { return wrapped->operator<(x0); }
		Q_SCRIPTABLE uint localIndex() const { return wrapped->localIndex(); }
		Q_SCRIPTABLE bool isLoaded(KDevelop::TopDUContext* x0) const { return wrapped->isLoaded(x0); }
	private:
		KDevelop::LocalIndexedDUContext* wrapped;
};

class KrossKDevelopImportTraceItem : public QObject, public Kross::WrapperInterface
{
	Q_OBJECT
	public:
		KrossKDevelopImportTraceItem(KDevelop::ImportTraceItem* obj, QObject* parent=0) : QObject(parent), wrapped(obj)		{ setObjectName("KDevelop::ImportTraceItem"); }
		void* wrappedObject() const { return wrapped; }

		typedef KDevelop::DUContext KDevelopDUContext;
		Q_PROPERTY(const KDevelopDUContext* ctx READ getctx SCRIPTABLE true)
		Q_SCRIPTABLE const KDevelopDUContext* getctx() const { return wrapped->ctx; }
		typedef KDevelop::SimpleCursor KDevelopSimpleCursor;
		Q_PROPERTY(KDevelopSimpleCursor  position READ getposition WRITE setposition SCRIPTABLE true)
		Q_SCRIPTABLE void setposition(const KDevelopSimpleCursor  val) { wrapped->position=val; }
		Q_SCRIPTABLE KDevelopSimpleCursor  getposition() const { return wrapped->position; }
	private:
		KDevelop::ImportTraceItem* wrapped;
};

class KrossKDevelopImportTrace : public QObject, public Kross::WrapperInterface
{
	Q_OBJECT
	public:
		KrossKDevelopImportTrace(KDevelop::ImportTrace* obj, QObject* parent=0) : QObject(parent), wrapped(obj)		{ setObjectName("KDevelop::ImportTrace"); }
		void* wrappedObject() const { return wrapped; }

	private:
		KDevelop::ImportTrace* wrapped;
};

class KrossKDevelopDUContext : public QObject, public Kross::WrapperInterface
{
	Q_OBJECT
	Q_ENUMS(ContextType)
	Q_FLAGS(ContextType Global Namespace Class Function Template Enum Helper Other)

	Q_ENUMS(SearchFlag)
	Q_FLAGS(SearchFlag NoSearchFlags InImportedParentContext OnlyContainerTypes DontSearchInParent NoUndefinedTemplateParams DirectQualifiedLookup NoFiltering OnlyFunctions NoImportsCheck)

	public:
		enum KrossContextType { Global=KDevelop::DUContext::Global, Namespace=KDevelop::DUContext::Namespace, Class=KDevelop::DUContext::Class, Function=KDevelop::DUContext::Function, Template=KDevelop::DUContext::Template, Enum=KDevelop::DUContext::Enum, Helper=KDevelop::DUContext::Helper, Other=KDevelop::DUContext::Other };
		enum KrossSearchFlag { NoSearchFlags=KDevelop::DUContext::NoSearchFlags, InImportedParentContext=KDevelop::DUContext::InImportedParentContext, OnlyContainerTypes=KDevelop::DUContext::OnlyContainerTypes, DontSearchInParent=KDevelop::DUContext::DontSearchInParent, NoUndefinedTemplateParams=KDevelop::DUContext::NoUndefinedTemplateParams, DirectQualifiedLookup=KDevelop::DUContext::DirectQualifiedLookup, NoFiltering=KDevelop::DUContext::NoFiltering, OnlyFunctions=KDevelop::DUContext::OnlyFunctions, NoImportsCheck=KDevelop::DUContext::NoImportsCheck };
		KrossKDevelopDUContext(KDevelop::DUContext* obj, QObject* parent=0) : QObject(parent), wrapped(obj)		{ setObjectName("KDevelop::DUContext"); }
		void* wrappedObject() const { return wrapped; }

		Q_SCRIPTABLE KDevelop::DUContext::ContextType type() const { return wrapped->type(); }
		Q_SCRIPTABLE void setType(KDevelop::DUContext::ContextType x0) { wrapped->setType(x0); }
		Q_SCRIPTABLE KDevelop::Declaration* owner() const { return wrapped->owner(); }
		Q_SCRIPTABLE void setOwner(KDevelop::Declaration* x0) { wrapped->setOwner(x0); }
		Q_SCRIPTABLE int depth() const { return wrapped->depth(); }
		Q_SCRIPTABLE KDevelop::TopDUContext* topContext() const { return wrapped->topContext(); }
		Q_SCRIPTABLE KDevelop::DUContext* findContextAt(const KDevelop::SimpleCursor& x0, bool x1=false) const { return wrapped->findContextAt(x0, x1); }
		Q_SCRIPTABLE KDevelop::Declaration* findDeclarationAt(const KDevelop::SimpleCursor& x0) const { return wrapped->findDeclarationAt(x0); }
		Q_SCRIPTABLE KDevelop::DUContext* findContextIncluding(const KDevelop::SimpleRange& x0) const { return wrapped->findContextIncluding(x0); }
		Q_SCRIPTABLE KDevelop::QualifiedIdentifier scopeIdentifier(bool x0=false) const { return wrapped->scopeIdentifier(x0); }
		Q_SCRIPTABLE bool equalScopeIdentifier(const KDevelop::DUContext* x0) const { return wrapped->equalScopeIdentifier(x0); }
		Q_SCRIPTABLE KDevelop::QualifiedIdentifier localScopeIdentifier() const { return wrapped->localScopeIdentifier(); }
		Q_SCRIPTABLE KDevelop::IndexedQualifiedIdentifier indexedLocalScopeIdentifier() const { return wrapped->indexedLocalScopeIdentifier(); }
		Q_SCRIPTABLE void setLocalScopeIdentifier(const KDevelop::QualifiedIdentifier& x0) { wrapped->setLocalScopeIdentifier(x0); }
		Q_SCRIPTABLE bool inSymbolTable() const { return wrapped->inSymbolTable(); }
		Q_SCRIPTABLE void setInSymbolTable(bool x0) { wrapped->setInSymbolTable(x0); }
		Q_SCRIPTABLE KDevelop::DUContext* parentContext() const { return wrapped->parentContext(); }
		Q_SCRIPTABLE QVector< KDevelop::DUContext::Import > importedParentContexts() const { return wrapped->importedParentContexts(); }
		Q_SCRIPTABLE KDevelop::SimpleCursor importPosition(const KDevelop::DUContext* x0) const { return wrapped->importPosition(x0); }
		Q_SCRIPTABLE bool imports(const KDevelop::DUContext* x0, const KDevelop::SimpleCursor& x1=KDevelop::SimpleCursor::invalid()) const { return wrapped->imports(x0, x1); }
		Q_SCRIPTABLE void addImportedParentContext(KDevelop::DUContext* x0, const KDevelop::SimpleCursor& x1=KDevelop::SimpleCursor::invalid(), bool x2=false, bool x3=false) { wrapped->addImportedParentContext(x0, x1, x2, x3); }
		Q_SCRIPTABLE bool addIndirectImport(const KDevelop::DUContext::Import& x0) { return wrapped->addIndirectImport(x0); }
		Q_SCRIPTABLE void removeImportedParentContext(KDevelop::DUContext* x0) { wrapped->removeImportedParentContext(x0); }
		Q_SCRIPTABLE void clearImportedParentContexts() { wrapped->clearImportedParentContexts(); }
		Q_SCRIPTABLE void setPropagateDeclarations(bool x0) { wrapped->setPropagateDeclarations(x0); }
		Q_SCRIPTABLE bool isPropagateDeclarations() const { return wrapped->isPropagateDeclarations(); }
		Q_SCRIPTABLE QVector< KDevelop::DUContext* > importers() const { return wrapped->importers(); }
		Q_SCRIPTABLE QVector< KDevelop::DUContext* > childContexts() const { return wrapped->childContexts(); }
		Q_SCRIPTABLE void deleteChildContextsRecursively() { wrapped->deleteChildContextsRecursively(); }
		Q_SCRIPTABLE bool inDUChain() const { return wrapped->inDUChain(); }
		Q_SCRIPTABLE KDevelop::DUContext* specialize(uint x0, const KDevelop::TopDUContext* x1, int x2=0) { return wrapped->specialize(x0, x1, x2); }
		Q_SCRIPTABLE QList< KDevelop::Declaration* > findDeclarations(const KDevelop::QualifiedIdentifier& x0, const KDevelop::SimpleCursor& x1=KDevelop::SimpleCursor::invalid(), const KDevelop::AbstractType::Ptr& x2=KDevelop::AbstractType::Ptr(), const KDevelop::TopDUContext* x3=0, KDevelop::DUContext::SearchFlags x4=KDevelop::DUContext::NoSearchFlags) const { return wrapped->findDeclarations(x0, x1, x2, x3, x4); }
		Q_SCRIPTABLE QList< KDevelop::Declaration* > findDeclarations(const KDevelop::Identifier& x0, const KDevelop::SimpleCursor& x1=KDevelop::SimpleCursor::invalid(), const KDevelop::TopDUContext* x2=0, KDevelop::DUContext::SearchFlags x3=KDevelop::DUContext::NoSearchFlags) const { return wrapped->findDeclarations(x0, x1, x2, x3); }
		Q_SCRIPTABLE QList< KDevelop::Declaration* > findLocalDeclarations(const KDevelop::Identifier& x0, const KDevelop::SimpleCursor& x1=KDevelop::SimpleCursor::invalid(), const KDevelop::TopDUContext* x2=0, const KDevelop::AbstractType::Ptr& x3=KDevelop::AbstractType::Ptr(), KDevelop::DUContext::SearchFlags x4=KDevelop::DUContext::NoSearchFlags) const { return wrapped->findLocalDeclarations(x0, x1, x2, x3, x4); }
		Q_SCRIPTABLE QVector< KDevelop::Declaration* > clearLocalDeclarations() { return wrapped->clearLocalDeclarations(); }
		Q_SCRIPTABLE void deleteLocalDeclarations() { wrapped->deleteLocalDeclarations(); }
		Q_SCRIPTABLE QVector< KDevelop::Declaration* > localDeclarations(const KDevelop::TopDUContext* x0=0) const { return wrapped->localDeclarations(x0); }
		Q_SCRIPTABLE KDevelop::DUContext* findContext(const KDevelop::SimpleCursor& x0, KDevelop::DUContext* x1=0) const { return wrapped->findContext(x0, x1); }
		Q_SCRIPTABLE QList< KDevelop::DUContext* > findContexts(KDevelop::DUContext::ContextType x0, const KDevelop::QualifiedIdentifier& x1, const KDevelop::SimpleCursor& x2=KDevelop::SimpleCursor::invalid(), const KDevelop::TopDUContext* x3=0, KDevelop::DUContext::SearchFlags x4=KDevelop::DUContext::NoSearchFlags) const { return wrapped->findContexts(x0, x1, x2, x3, x4); }
		Q_SCRIPTABLE bool parentContextOf(KDevelop::DUContext* x0) const { return wrapped->parentContextOf(x0); }
		Q_SCRIPTABLE QList< QPair< KDevelop::Declaration*, int > > allDeclarations(const KDevelop::SimpleCursor& x0, const KDevelop::TopDUContext* x1, bool x2=true) const { return wrapped->allDeclarations(x0, x1, x2); }
		Q_SCRIPTABLE QList< KDevelop::Declaration* > allLocalDeclarations(const KDevelop::Identifier& x0) const { return wrapped->allLocalDeclarations(x0); }
		Q_SCRIPTABLE void cleanIfNotEncountered(const QSet< KDevelop::DUChainBase* >& x0) { wrapped->cleanIfNotEncountered(x0); }
		Q_SCRIPTABLE void changingIdentifier(KDevelop::Declaration* x0, const KDevelop::Identifier& x1, const KDevelop::Identifier& x2) { wrapped->changingIdentifier(x0, x1, x2); }
		Q_SCRIPTABLE const KDevelop::Use* uses() const { return wrapped->uses(); }
		Q_SCRIPTABLE int usesCount() const { return wrapped->usesCount(); }
		Q_SCRIPTABLE int findUseAt(const KDevelop::SimpleCursor& x0) const { return wrapped->findUseAt(x0); }
		Q_SCRIPTABLE KTextEditor::SmartRange* useSmartRange(int x0) { return wrapped->useSmartRange(x0); }
		Q_SCRIPTABLE void setUseSmartRange(int x0, KTextEditor::SmartRange* x1) { wrapped->setUseSmartRange(x0, x1); }
		Q_SCRIPTABLE void clearUseSmartRanges() { wrapped->clearUseSmartRanges(); }
		Q_SCRIPTABLE void setUseDeclaration(int x0, int x1) { wrapped->setUseDeclaration(x0, x1); }
		Q_SCRIPTABLE int createUse(int x0, const KDevelop::SimpleRange& x1, KTextEditor::SmartRange* x2, int x3=-1) { return wrapped->createUse(x0, x1, x2, x3); }
		Q_SCRIPTABLE void deleteUse(int x0) { wrapped->deleteUse(x0); }
		Q_SCRIPTABLE void deleteUses() { wrapped->deleteUses(); }
		Q_SCRIPTABLE QVector< KTextEditor::SmartRange* > useRanges() { return wrapped->useRanges(); }
		Q_SCRIPTABLE QVector< KTextEditor::SmartRange* > takeUseRanges() { return wrapped->takeUseRanges(); }
		Q_SCRIPTABLE QWidget* createNavigationWidget(KDevelop::Declaration* x0=0, KDevelop::TopDUContext* x1=0, const QString& x2=QString(), const QString& x3=QString()) const { return wrapped->createNavigationWidget(x0, x1, x2, x3); }
		Q_SCRIPTABLE bool findDeclarationsInternal(const KDevelop::DUContext::SearchItem::PtrList& x0, const KDevelop::SimpleCursor& x1, const KDevelop::AbstractType::Ptr& x2, KDevelop::DUContext::DeclarationList& x3, const KDevelop::TopDUContext* x4, KDevelop::DUContext::SearchFlags x5) const { return wrapped->findDeclarationsInternal(x0, x1, x2, x3, x4, x5); }
		Q_SCRIPTABLE void squeeze() { wrapped->squeeze(); }
	private:
		KDevelop::DUContext* wrapped;
};

class KrossKDevelopDUContextImport : public QObject, public Kross::WrapperInterface
{
	Q_OBJECT
	public:
		KrossKDevelopDUContextImport(KDevelop::DUContext::Import* obj, QObject* parent=0) : QObject(parent), wrapped(obj)		{ setObjectName("KDevelop::DUContext::Import"); }
		void* wrappedObject() const { return wrapped; }

		Q_SCRIPTABLE bool operator==(const KDevelop::DUContext::Import& x0) const { return wrapped->operator==(x0); }
		Q_SCRIPTABLE KDevelop::DUContext* context(const KDevelop::TopDUContext* x0) const { return wrapped->context(x0); }
		Q_SCRIPTABLE uint topContextIndex() const { return wrapped->topContextIndex(); }
		Q_SCRIPTABLE KDevelop::IndexedDUContext indexedContext() const { return wrapped->indexedContext(); }
		Q_SCRIPTABLE bool isBackwardMapped() const { return wrapped->isBackwardMapped(); }
		typedef KDevelop::SimpleCursor KDevelopSimpleCursor;
		Q_PROPERTY(KDevelopSimpleCursor  position READ getposition WRITE setposition SCRIPTABLE true)
		Q_SCRIPTABLE void setposition(const KDevelopSimpleCursor  val) { wrapped->position=val; }
		Q_SCRIPTABLE KDevelopSimpleCursor  getposition() const { return wrapped->position; }
	private:
		KDevelop::DUContext::Import* wrapped;
};

class KrossKDevelopDUContextSearchItem : public QObject, public Kross::WrapperInterface
{
	Q_OBJECT
	public:
		KrossKDevelopDUContextSearchItem(KDevelop::DUContext::SearchItem* obj, QObject* parent=0) : QObject(parent), wrapped(obj)		{ setObjectName("KDevelop::DUContext::SearchItem"); }
		void* wrappedObject() const { return wrapped; }

		Q_SCRIPTABLE bool isEmpty() const { return wrapped->isEmpty(); }
		Q_SCRIPTABLE bool hasNext() const { return wrapped->hasNext(); }
		Q_SCRIPTABLE void addToEachNode(KDevelop::DUContext::SearchItem::Ptr x0) { wrapped->addToEachNode(x0); }
		Q_SCRIPTABLE void addToEachNode(KDevelop::DUContext::SearchItem::PtrList x0) { wrapped->addToEachNode(x0); }
		Q_SCRIPTABLE bool match(const KDevelop::QualifiedIdentifier& x0, int x1=0) const { return wrapped->match(x0, x1); }
		Q_SCRIPTABLE QList< KDevelop::QualifiedIdentifier > toList(const KDevelop::QualifiedIdentifier& x0=KDevelop::QualifiedIdentifier()) const { return wrapped->toList(x0); }
		Q_SCRIPTABLE void addNext(KDevelop::DUContext::SearchItem::Ptr x0) { wrapped->addNext(x0); }
		Q_PROPERTY(bool  isExplicitlyGlobal READ getisExplicitlyGlobal WRITE setisExplicitlyGlobal SCRIPTABLE true)
		Q_SCRIPTABLE void setisExplicitlyGlobal(const bool  val) { wrapped->isExplicitlyGlobal=val; }
		Q_SCRIPTABLE bool  getisExplicitlyGlobal() const { return wrapped->isExplicitlyGlobal; }
		typedef KDevelop::Identifier KDevelopIdentifier;
		Q_PROPERTY(KDevelopIdentifier  identifier READ getidentifier WRITE setidentifier SCRIPTABLE true)
		Q_SCRIPTABLE void setidentifier(const KDevelopIdentifier  val) { wrapped->identifier=val; }
		Q_SCRIPTABLE KDevelopIdentifier  getidentifier() const { return wrapped->identifier; }
		typedef KDevelop::DUContext::SearchItem::PtrList KDevelopDUContextSearchItemPtrList;
		Q_PROPERTY(KDevelopDUContextSearchItemPtrList  next READ getnext WRITE setnext SCRIPTABLE true)
		Q_SCRIPTABLE void setnext(const KDevelopDUContextSearchItemPtrList  val) { wrapped->next=val; }
		Q_SCRIPTABLE KDevelopDUContextSearchItemPtrList  getnext() const { return wrapped->next; }
	private:
		KDevelop::DUContext::SearchItem* wrapped;
};

bool krossducontext_registerHandler(const QByteArray& name, Kross::MetaTypeHandler::FunctionPtr* handler)
{ Kross::Manager::self().registerMetaTypeHandler(name, handler); return false; }

namespace Handlers
{
QVariant _kDevelopDUContextSearchItemHandler(void* type)
{
	if(!type) return QVariant();
	KDevelop::DUContext::SearchItem* t=static_cast<KDevelop::DUContext::SearchItem*>(type);
	Q_ASSERT(dynamic_cast<KDevelop::DUContext::SearchItem*>(t));
	return qVariantFromValue((QObject*) new KrossKDevelopDUContextSearchItem(t, 0));
}
bool b_KDevelopDUContextSearchItem1=krossducontext_registerHandler("DUContext::SearchItem*", _kDevelopDUContextSearchItemHandler);
bool b_KDevelopDUContextSearchItem=krossducontext_registerHandler("KDevelop::DUContext::SearchItem*", _kDevelopDUContextSearchItemHandler);
QVariant kDevelopDUContextSearchItemHandler(KDevelop::DUContext::SearchItem* type){ return _kDevelopDUContextSearchItemHandler(type); }
QVariant kDevelopDUContextSearchItemHandler(const KDevelop::DUContext::SearchItem* type) { return _kDevelopDUContextSearchItemHandler((void*) type); }

QVariant _kDevelopDUContextImportHandler(void* type)
{
	if(!type) return QVariant();
	KDevelop::DUContext::Import* t=static_cast<KDevelop::DUContext::Import*>(type);
	Q_ASSERT(dynamic_cast<KDevelop::DUContext::Import*>(t));
	return qVariantFromValue((QObject*) new KrossKDevelopDUContextImport(t, 0));
}
bool b_KDevelopDUContextImport1=krossducontext_registerHandler("DUContext::Import*", _kDevelopDUContextImportHandler);
bool b_KDevelopDUContextImport=krossducontext_registerHandler("KDevelop::DUContext::Import*", _kDevelopDUContextImportHandler);
QVariant kDevelopDUContextImportHandler(KDevelop::DUContext::Import* type){ return _kDevelopDUContextImportHandler(type); }
QVariant kDevelopDUContextImportHandler(const KDevelop::DUContext::Import* type) { return _kDevelopDUContextImportHandler((void*) type); }

QVariant _kDevelopDUContextHandler(void* type)
{
	if(!type) return QVariant();
	KDevelop::DUContext* t=static_cast<KDevelop::DUContext*>(type);
	Q_ASSERT(dynamic_cast<KDevelop::DUContext*>(t));
	return qVariantFromValue((QObject*) new KrossKDevelopDUContext(t, 0));
}
bool b_KDevelopDUContext1=krossducontext_registerHandler("DUContext*", _kDevelopDUContextHandler);
bool b_KDevelopDUContext=krossducontext_registerHandler("KDevelop::DUContext*", _kDevelopDUContextHandler);
QVariant kDevelopDUContextHandler(KDevelop::DUContext* type){ return _kDevelopDUContextHandler(type); }
QVariant kDevelopDUContextHandler(const KDevelop::DUContext* type) { return _kDevelopDUContextHandler((void*) type); }

QVariant _kDevelopImportTraceHandler(void* type)
{
	if(!type) return QVariant();
	KDevelop::ImportTrace* t=static_cast<KDevelop::ImportTrace*>(type);
	Q_ASSERT(dynamic_cast<KDevelop::ImportTrace*>(t));
	return qVariantFromValue((QObject*) new KrossKDevelopImportTrace(t, 0));
}
bool b_KDevelopImportTrace1=krossducontext_registerHandler("ImportTrace*", _kDevelopImportTraceHandler);
bool b_KDevelopImportTrace=krossducontext_registerHandler("KDevelop::ImportTrace*", _kDevelopImportTraceHandler);
QVariant kDevelopImportTraceHandler(KDevelop::ImportTrace* type){ return _kDevelopImportTraceHandler(type); }
QVariant kDevelopImportTraceHandler(const KDevelop::ImportTrace* type) { return _kDevelopImportTraceHandler((void*) type); }

QVariant _kDevelopImportTraceItemHandler(void* type)
{
	if(!type) return QVariant();
	KDevelop::ImportTraceItem* t=static_cast<KDevelop::ImportTraceItem*>(type);
	Q_ASSERT(dynamic_cast<KDevelop::ImportTraceItem*>(t));
	return qVariantFromValue((QObject*) new KrossKDevelopImportTraceItem(t, 0));
}
bool b_KDevelopImportTraceItem1=krossducontext_registerHandler("ImportTraceItem*", _kDevelopImportTraceItemHandler);
bool b_KDevelopImportTraceItem=krossducontext_registerHandler("KDevelop::ImportTraceItem*", _kDevelopImportTraceItemHandler);
QVariant kDevelopImportTraceItemHandler(KDevelop::ImportTraceItem* type){ return _kDevelopImportTraceItemHandler(type); }
QVariant kDevelopImportTraceItemHandler(const KDevelop::ImportTraceItem* type) { return _kDevelopImportTraceItemHandler((void*) type); }

QVariant _kDevelopLocalIndexedDUContextHandler(void* type)
{
	if(!type) return QVariant();
	KDevelop::LocalIndexedDUContext* t=static_cast<KDevelop::LocalIndexedDUContext*>(type);
	Q_ASSERT(dynamic_cast<KDevelop::LocalIndexedDUContext*>(t));
	return qVariantFromValue((QObject*) new KrossKDevelopLocalIndexedDUContext(t, 0));
}
bool b_KDevelopLocalIndexedDUContext1=krossducontext_registerHandler("LocalIndexedDUContext*", _kDevelopLocalIndexedDUContextHandler);
bool b_KDevelopLocalIndexedDUContext=krossducontext_registerHandler("KDevelop::LocalIndexedDUContext*", _kDevelopLocalIndexedDUContextHandler);
QVariant kDevelopLocalIndexedDUContextHandler(KDevelop::LocalIndexedDUContext* type){ return _kDevelopLocalIndexedDUContextHandler(type); }
QVariant kDevelopLocalIndexedDUContextHandler(const KDevelop::LocalIndexedDUContext* type) { return _kDevelopLocalIndexedDUContextHandler((void*) type); }

QVariant _kDevelopIndexedDUContextHandler(void* type)
{
	if(!type) return QVariant();
	KDevelop::IndexedDUContext* t=static_cast<KDevelop::IndexedDUContext*>(type);
	Q_ASSERT(dynamic_cast<KDevelop::IndexedDUContext*>(t));
	return qVariantFromValue((QObject*) new KrossKDevelopIndexedDUContext(t, 0));
}
bool b_KDevelopIndexedDUContext1=krossducontext_registerHandler("IndexedDUContext*", _kDevelopIndexedDUContextHandler);
bool b_KDevelopIndexedDUContext=krossducontext_registerHandler("KDevelop::IndexedDUContext*", _kDevelopIndexedDUContextHandler);
QVariant kDevelopIndexedDUContextHandler(KDevelop::IndexedDUContext* type){ return _kDevelopIndexedDUContextHandler(type); }
QVariant kDevelopIndexedDUContextHandler(const KDevelop::IndexedDUContext* type) { return _kDevelopIndexedDUContextHandler((void*) type); }

}
#include "krossducontext.moc"
