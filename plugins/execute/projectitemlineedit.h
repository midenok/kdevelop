/***************************************************************************
 *   Copyright 2008 Aleix Pol <aleixpol@gmail.com>                         *
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

#ifndef PROJECTITEMLINEEDIT_H
#define PROJECTITEMLINEEDIT_H

#include <QCompleter>
#include <QStringList>
#include <KLineEdit>

namespace KDevelop { class ProjectModel; }

class ProjectItemCompleter : public QCompleter
{
    Q_OBJECT
    public:
        ProjectItemCompleter(KDevelop::ProjectModel* model, QObject* parent=0);
        
        QString separator() const { return sep; }
        QStringList splitPath(const QString &path) const;
        QString pathFromIndex(const QModelIndex& index) const;
        
    private:
        KDevelop::ProjectModel* mModel;
        QChar sep;
};

class ProjectItemLineEdit : public KLineEdit
{
    Q_OBJECT
    public:
        ProjectItemLineEdit(QWidget* parent=0);
        
    public Q_SLOTS:
        void updated(const QString& newText);
        void correctnessChange(bool correct);
        
    Q_SIGNALS:
        void correctnessChanged(bool isCorrect);
};

#endif
