/* This file is part of KDevelop
    Copyright (C) 2006 Hamish Rodda <rodda@kde.org>

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
// kate: indent-width 2;

#include "declarationbuilder.h"

#include <ktexteditor/smartrange.h>

#include "cppeditorintegrator.h"
#include "name_compiler.h"
#include "tokens.h"
#include "parsesession.h"
#include "definition.h"
#include "symboltable.h"
#include "forwarddeclaration.h"

using namespace KTextEditor;

DeclarationBuilder::DeclarationBuilder (ParseSession* session)
  : DeclarationBuilderBase(session)
{
}

DeclarationBuilder::DeclarationBuilder (CppEditorIntegrator* editor)
  : DeclarationBuilderBase(editor)
{
}

TopDUContext* DeclarationBuilder::buildDeclarations(const KUrl& url, AST *node, QList<DUContext*>* includes)
{
  TopDUContext* top = buildContexts(url, node, includes);

  Q_ASSERT(m_accessPolicyStack.isEmpty());
  Q_ASSERT(m_functionDefinedStack.isEmpty());

  return top;
}

void DeclarationBuilder::visitFunctionDeclaration(FunctionDefinitionAST* node)
{
  parseStorageSpecifiers(node->storage_specifiers);
  parseFunctionSpecifiers(node->function_specifiers);

  m_functionDefinedStack.push(node->start_token);

  DeclarationBuilderBase::visitFunctionDeclaration(node);

  m_functionDefinedStack.pop();

  popSpecifiers();
}

void DeclarationBuilder::visitSimpleDeclaration(SimpleDeclarationAST* node)
{
  parseStorageSpecifiers(node->storage_specifiers);
  parseFunctionSpecifiers(node->function_specifiers);

  m_functionDefinedStack.push(0);

  DeclarationBuilderBase::visitSimpleDeclaration(node);

  m_functionDefinedStack.pop();

  popSpecifiers();
}

void DeclarationBuilder::visitDeclarator (DeclaratorAST* node)
{
  if (node->parameter_declaration_clause) {
    openDeclaration(node->id, node, true);

    /*if (!node->type_specifier) {
      // TODO detect identifiers not equal to classname
      if (currentDeclaration()->identifier().toString().startsWith('~'))
        static_cast<ClassFunctionDeclaration*>(currentDeclaration())->setDestructor(true);
      else
        static_cast<ClassFunctionDeclaration*>(currentDeclaration())->setConstructor(true);
    }*/

    applyFunctionSpecifiers();

  } else {
    openDefinition(node->id, node);
  }

  applyStorageSpecifiers();

  DeclarationBuilderBase::visitDeclarator(node);

  if (node->parameter_declaration_clause) {
    if (!m_functionDefinedStack.isEmpty() && m_functionDefinedStack.top() && node->id) {
      QualifiedIdentifier id = identifierForName(node->id);
      if (id.count() > 1) {
        KTextEditor::Cursor pos = m_editor->findPosition(m_functionDefinedStack.top(), KDevEditorIntegrator::FrontEdge);

        //kDebug() << k_funcinfo << "Searching for declaration of " << id << endl;

        QList<Declaration*> declarations = currentContext()->findDeclarations(id, pos, lastType());
        foreach (Declaration* dec, declarations) {
          if (dec->isForwardDeclaration())
            continue;

          Declaration* oldDec = currentDeclaration();
          abortDeclaration();
          Definition* def = new Definition(oldDec->takeRange(), dec, currentContext());
          delete oldDec;
          dec->setDefinition(def);

          // Resolve forward declarations
          foreach (Declaration* forward, declarations) {
            if (forward->isForwardDeclaration())
              forward->forwardDeclaration()->setResolved(dec);
          }
          return;
        }
      }
    }
  }

  closeDeclaration();
}



ForwardDeclaration * DeclarationBuilder::openForwardDeclaration(NameAST * name, AST * range)
{
  return static_cast<ForwardDeclaration*>(openDeclaration(name, range, false, true));
}

Declaration* DeclarationBuilder::openDefinition(NameAST* name, AST* rangeNode, bool isFunction)
{
  Declaration* dec = openDeclaration(name, rangeNode, isFunction);
  dec->setDeclarationIsDefinition(true);
  return dec;
}

Declaration* DeclarationBuilder::openDeclaration(NameAST* name, AST* rangeNode, bool isFunction, bool isForward)
{
  Declaration::Scope scope = Declaration::GlobalScope;
  switch (currentContext()->type()) {
    case DUContext::Namespace:
      scope = Declaration::NamespaceScope;
      break;
    case DUContext::Class:
      scope = Declaration::ClassScope;
      break;
    case DUContext::Function:
      scope = Declaration::LocalScope;
      break;
    default:
      break;
  }

  Range* prior = m_editor->currentRange();
  Range* range = m_editor->createRange(name ? static_cast<AST*>(name) : rangeNode);
  m_editor->exitCurrentRange();
  Q_ASSERT(m_editor->currentRange() == prior);

  Declaration* declaration;
  if (isForward) {
    declaration = new ForwardDeclaration(range, scope, currentContext());

  } else if (isFunction) {
    declaration = new ClassFunctionDeclaration(range, currentContext());
    if (!m_functionDefinedStack.isEmpty())
      declaration->setDeclarationIsDefinition(m_functionDefinedStack.top());

  } else if (scope == Declaration::ClassScope) {
    declaration = new ClassMemberDeclaration(range, currentContext());

  } else {
    declaration = new Declaration(range, scope, currentContext());
  }

  if (name) {
    QualifiedIdentifier id = identifierForName(name);

    // FIXME this can happen if we're defining a staticly declared variable
    //Q_ASSERT(m_nameCompiler->identifier().count() == 1);
    Q_ASSERT(!id.isEmpty());
    declaration->setIdentifier(id.last());
  }

  if (currentContext()->type() == DUContext::Class)
    static_cast<ClassMemberDeclaration*>(declaration)->setAccessPolicy(currentAccessPolicy());

  m_declarationStack.push(declaration);

  return declaration;
}

void DeclarationBuilder::closeDeclaration()
{
  if (lastType())
    currentDeclaration()->setType(lastType());

  //kDebug() << k_funcinfo << "Mangled declaration: " << currentDeclaration()->mangledIdentifier() << endl;

  switch (currentContext()->type()) {
    case DUContext::Global:
    case DUContext::Namespace:
    case DUContext::Class:
      SymbolTable::self()->addDeclaration(currentDeclaration());
      break;
    default:
      break;
  }

  m_declarationStack.pop();
}

void DeclarationBuilder::abortDeclaration()
{
  m_declarationStack.pop();
}

void DeclarationBuilder::visitClassSpecifier(ClassSpecifierAST *node)
{
  openDefinition(node->name, node);

  int kind = m_editor->parseSession()->token_stream->kind(node->class_key);
  if (kind == Token_struct || kind == Token_union)
    m_accessPolicyStack.push(Cpp::Public);
  else
    m_accessPolicyStack.push(Cpp::Private);

  DeclarationBuilderBase::visitClassSpecifier(node);

  closeDeclaration();

  m_accessPolicyStack.pop();
}

void DeclarationBuilder::visitElaboratedTypeSpecifier(ElaboratedTypeSpecifierAST* node)
{
  bool openedDeclaration = false;

  if (node->name) {
    QualifiedIdentifier id = identifierForName(node->name);
    KTextEditor::Cursor pos = m_editor->findPosition(node->start_token, KDevEditorIntegrator::FrontEdge);

    QList<Declaration*> declarations = currentContext()->findDeclarations(id, pos);
    Declaration* actual = 0;
    foreach (Declaration* declaration, declarations)
      if (!declaration->isForwardDeclaration()) {
        actual = declaration;
        break;
      }

    if (!actual) {
      int kind = m_editor->parseSession()->token_stream->kind(node->type);
      // Create forward declaration
      switch (kind) {
        case Token_class:
        case Token_struct:
        case Token_union:
          openForwardDeclaration(node->name, node);
          openedDeclaration = true;
          break;
        case Token_enum:
        case Token_typename:
          // TODO what goes here...?
          break;
      }
    }
  }

  DeclarationBuilderBase::visitElaboratedTypeSpecifier(node);

  if (openedDeclaration)
    closeDeclaration();
}


void DeclarationBuilder::visitAccessSpecifier(AccessSpecifierAST* node)
{
  if (node->specs) {
    const ListNode<std::size_t> *it = node->specs->toFront();
    const ListNode<std::size_t> *end = it;
    do {
      int kind = m_editor->parseSession()->token_stream->kind(it->element);
      switch (kind) {
        case Token_signals:
        case Token_slots:
        case Token_k_dcop:
        case Token_k_dcop_signals:
          break;
        case Token_public:
          setAccessPolicy(Cpp::Public);
          break;
        case Token_protected:
          setAccessPolicy(Cpp::Protected);
          break;
        case Token_private:
          setAccessPolicy(Cpp::Private);
          break;
      }

      it = it->next;
    } while (it != end);
  }

  DeclarationBuilderBase::visitAccessSpecifier(node);
}

void DeclarationBuilder::parseStorageSpecifiers(const ListNode<std::size_t>* storage_specifiers)
{
  ClassMemberDeclaration::StorageSpecifiers specs = 0;

  if (storage_specifiers) {
    const ListNode<std::size_t> *it = storage_specifiers->toFront();
    const ListNode<std::size_t> *end = it;
    do {
      int kind = m_editor->parseSession()->token_stream->kind(it->element);
      switch (kind) {
        case Token_friend:
          specs |= ClassMemberDeclaration::FriendSpecifier;
          break;
        case Token_auto:
          specs |= ClassMemberDeclaration::AutoSpecifier;
          break;
        case Token_register:
          specs |= ClassMemberDeclaration::RegisterSpecifier;
          break;
        case Token_static:
          specs |= ClassMemberDeclaration::StaticSpecifier;
          break;
        case Token_extern:
          specs |= ClassMemberDeclaration::ExternSpecifier;
          break;
        case Token_mutable:
          specs |= ClassMemberDeclaration::MutableSpecifier;
          break;
      }

      it = it->next;
    } while (it != end);
  }

  m_storageSpecifiers.push(specs);
}

void DeclarationBuilder::parseFunctionSpecifiers(const ListNode<std::size_t>* function_specifiers)
{
  ClassFunctionDeclaration::FunctionSpecifiers specs = 0;

  if (function_specifiers) {
    const ListNode<std::size_t> *it = function_specifiers->toFront();
    const ListNode<std::size_t> *end = it;
    do {
      int kind = m_editor->parseSession()->token_stream->kind(it->element);
      switch (kind) {
        case Token_inline:
          specs |= ClassFunctionDeclaration::InlineSpecifier;
          break;
        case Token_virtual:
          specs |= ClassFunctionDeclaration::VirtualSpecifier;
          break;
        case Token_explicit:
          specs |= ClassFunctionDeclaration::ExplicitSpecifier;
          break;
      }

      it = it->next;
    } while (it != end);
  }

  m_functionSpecifiers.push(specs);
}

void DeclarationBuilder::popSpecifiers()
{
  m_functionSpecifiers.pop();
  m_storageSpecifiers.pop();
}

void DeclarationBuilder::applyStorageSpecifiers()
{
  if (!m_storageSpecifiers.isEmpty())
    if (ClassMemberDeclaration* member = dynamic_cast<ClassMemberDeclaration*>(currentDeclaration()))
      member->setStorageSpecifiers(m_storageSpecifiers.top());
}

void DeclarationBuilder::applyFunctionSpecifiers()
{
  if (!m_functionSpecifiers.isEmpty()) {
    Q_ASSERT(dynamic_cast<ClassFunctionDeclaration*>(currentDeclaration()));
    ClassFunctionDeclaration* function = static_cast<ClassFunctionDeclaration*>(currentDeclaration());
    function->setFunctionSpecifiers(m_functionSpecifiers.top());
  }
}
