/***************************************************************************
 *   Copyright (C) 1999, 2000 by Bernd Gehrmann                            *
 *   bernd@kdevelop.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qvbox.h>
#include <qwhatsthis.h>
#include <kdebug.h>
#include <klocale.h>
#include <kaction.h>
#include <kiconloader.h>
#include <kdialogbase.h>

#include "projectspace.h"
#include "grepview.h"
#include "grepwidget.h"
#include "grepconfigwidget.h"
#include "main.h"
#include "KDevComponentManager.h"
#include "kdeveditormanager.h"


GrepView::GrepView(QObject *parent, const char *name)
    : KDevComponent(parent, name)
{
    setInstance(GrepFactory::instance());
    setXMLFile("kdevgrepview.rc");

    m_widget = 0;
}


GrepView::~GrepView()
{
    delete m_widget;
}

ProjectSpace* GrepView::projectSpace(){
	return static_cast<ProjectSpace*>(componentManager()->component("ProjectSpace"));
}

KDevEditorManager* GrepView::editorManager(){
	return static_cast<KDevEditorManager*>(componentManager()->component("KDevEditorManager"));
}

void GrepView::setupGUI()
{
    kdDebug(9001) << "Building GrepWidget" << endl;
    
    m_widget = new GrepWidget(this);
    m_widget->setIcon(SmallIcon("find"));
    m_widget->setCaption(i18n("Grep"));
    QWhatsThis::add(m_widget, i18n("Grep\n\n"
                                   "This window contains the output of a grep "
                                   "command. Clicking on an item in the list "
                                   "will automatically open the corresponding "
                                   "source file and set the cursor to the line "
                                   "with the match."));

    emit embedWidget(m_widget, OutputView, i18n("Grep"), i18n("grep output view"));

    KAction *action;
    action = new KAction(i18n("Grep"), CTRL+ALT+Key_F, m_widget, SLOT(showDialog()),
                         actionCollection(), "grep");
    action->setStatusText( i18n("Opens the search in files dialog to search "
                               "for expressions over several files") );
    action->setWhatsThis( i18n("Search in files\n\n"
                               "Opens the Search in files dialog "
                               "to enter an expression to look up "
                               "over several files. Matches will be "
                               "displayed, you can switch to a match "
                               "directly.") );
}


void GrepView::slotConfigWidgetRequested(KDialogBase *dlg)
{
    QVBox *vbox = dlg->addVBoxPage(i18n("Grep"));
    (void) new GrepConfigWidget(vbox, "grep config widget");
}


void GrepView::slotStopButtonClicked()
{
    kdDebug(9001) << "GrepView::stopButtonClicked()" << endl;
    m_widget->killJob();
}


void GrepView::slotProjectSpaceOpened()
{
    kdDebug(9001) << "GrepView::projectSpaceOpened()" << endl;
    m_widget->setProjectSpace(projectSpace());

#if 0
    //at the moment GrepView writes only data that are user depended,
    // find the "GrepView" tag
    QDomDocument doc = pProjectSpace->readUserDocument();
    QDomNodeList grepList = doc.elementsByTagName("GrepView");
    QDomElement grepElement = grepList.item(0).toElement();
    kdDebug(9001) << "GrepView::readProjectSpaceUserConfig: Value: " << grepElement.attribute("test") << endl;
#endif
}


void GrepView::slotProjectSpaceClosed()
{
#if 0
    kdDebug(9001) << "GrepView::writeProjectSpaceUserConfig" << endl;
    QDomDocument doc = pProjectSpace->readUserDocument();
    QDomElement rootElement = doc.documentElement();
    QDomElement elem = doc.createElement("GrepView");
    elem.setAttribute("test", "value");
    rootElement.appendChild( elem );
#endif
}

#include "grepview.moc"
