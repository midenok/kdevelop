/***************************************************************************
 *   This file is part of KDevelop                                         *
 *   Copyright 2006 Hamish Rodda <rodda@kde.org>                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef ducontext_p_H
#define ducontext_p_H

#include <QMutex>

#include "duchainpointer.h"

namespace KDevelop{
class DUContext;
class DUContextPrivate
{
public:
  DUContextPrivate( DUContext* );
  DUContext::ContextType m_contextType;
  QualifiedIdentifier m_scopeIdentifier;
  DUContextPointer m_parentContext;
  DeclarationPointer m_declaration;
  QList<DUContextPointer> m_importedParentContexts;
  QList<DUContext*> m_childContexts;
  QList<DUContext*> m_importedChildContexts;

  static QMutex m_localDeclarationsMutex;
  ///@warning: Whenever m_localDeclarations is read or written, m_localDeclarationsMutex must be locked.
  QList<Declaration*> m_localDeclarations;
  
  QList<Definition*> m_localDefinitions;
  QList<DUContext::UsingNS*> m_usingNamespaces;
  QList<Use*> m_uses;
  QList<Use*> m_orphanUses;
  DUContext* m_context;
  bool m_inSymbolTable : 1;
  bool m_anonymousInParent : 1; //Whether this context was added anonymously into the parent. This means that it cannot be found as child-context in the parent.
    /**
   * Adds a child context.
   *
   * \note Be sure to have set the text location first, so that
   * the chain is sorted correctly.
   */
  void addChildContext(DUContext* context);
  
  /**Removes the context from childContexts
   * @return Whether a context was removed
   * */
  bool removeChildContext(DUContext* context);

  void addImportedChildContext( DUContext * context );
  void removeImportedChildContext( DUContext * context );

  void addDeclaration(Declaration* declaration);
  
  /**Removes the declaration from localDeclarations
   * @return Whether a declaration was removed
   * */
  bool removeDeclaration(Declaration* declaration);

  void addUse(Use* use);
  void removeUse(Use* use);
};
}


#endif

//kate: space-indent on; indent-width 2; replace-tabs on; auto-insert-doxygen on; indent-mode cstyle;
