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

#include "documentrange.h"

using namespace KTextEditor;

DocumentRange::DocumentRange(const KUrl& document, const Cursor& start, const Cursor& end)
  : Range(start, end)
  , m_document(document)
{
}

DocumentRange::DocumentRange(const KUrl& document, const Range& range)
  : Range(range)
  , m_document(document)
{
}

DocumentRange::DocumentRange(const DocumentRange& copy)
  : Range(copy)
  , m_document(copy.document())
{
}

const KUrl& DocumentRange::document() const
{
  return m_document;
}

void DocumentRange::setDocument(const KUrl& document)
{
  m_document = document;
}

// kate: indent-width 2;
