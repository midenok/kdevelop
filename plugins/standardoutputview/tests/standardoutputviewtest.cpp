/*
    Copyright (C) 2011  Silvère Lestang <silvere.lestang@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <QAction>
#include <QStackedWidget>
#include <QStandardItem>
#include <QItemDelegate>
#include <QTreeView>

#include <qtest_kde.h>
#include <kdebug.h>
#include <ktabwidget.h>

#include <tests/testcore.h>
#include <tests/autotestshell.h>
#include <sublime/view.h>
#include <sublime/controller.h>
#include <sublime/area.h>
#include <sublime/tooldocument.h>
#include <interfaces/iplugincontroller.h>


#include "standardoutputviewtest.h"
#include "../standardoutputview.h"
#include "../outputwidget.h"
#include "../toolviewdata.h"

namespace KDevelop
{
    class IUiController;
}

class QAbstractItemDelegate;
class QStandardItemModel;

QTEST_KDEMAIN(StandardOutputViewTest, GUI)

const QString StandardOutputViewTest::toolviewTitle = "my_toolview";

void StandardOutputViewTest::initTestCase()
{
    KDevelop::AutoTestShell::init();
    m_testCore = new KDevelop::TestCore();
    m_testCore->initialize(KDevelop::Core::Default);
    
    m_controller = dynamic_cast<KDevelop::UiController*>(m_testCore->uiController());
    
    QTest::qWait(500);
    
    m_stdOutputView = 0;
    KDevelop::IPluginController* plugin_controller = m_testCore->pluginController();
    
    QList<KDevelop::IPlugin*> plugins = plugin_controller->loadedPlugins();
    foreach(KDevelop::IPlugin* plugin, plugins) {
        if(plugin_controller->pluginInfo(plugin).pluginName() == "KDevStandardOutputView")
           m_stdOutputView =  dynamic_cast<StandardOutputView*>(plugin);
    }
    Q_ASSERT(m_stdOutputView);
}

void StandardOutputViewTest::cleanupTestCase()
{
     m_testCore->cleanup();
     delete m_testCore;
}

OutputWidget* StandardOutputViewTest::toolviewPointer(QString toolviewTitle)
{
    QList< Sublime::View* > views = m_controller->activeArea()->toolViews();
    //QList< Sublime::Area* > areas = m_controller->allAreas();
    foreach(Sublime::View* view, views) {
        Sublime::ToolDocument *doc = dynamic_cast<Sublime::ToolDocument*>(view->document());
        if(doc)
        {
            if(doc->title() == toolviewTitle) {
                return dynamic_cast<OutputWidget*>(view->widget());
            }
        }
    }
    return 0;
}

void StandardOutputViewTest::testRegisterAndRemoveToolView()
{    
    toolviewId = m_stdOutputView->registerToolView(toolviewTitle, KDevelop::IOutputView::HistoryView);
    QVERIFY(toolviewPointer(toolviewTitle));
    
    m_stdOutputView->removeToolView(toolviewId);
    QVERIFY(!toolviewPointer(toolviewTitle));
}

void StandardOutputViewTest::testActions()
{
    toolviewId = m_stdOutputView->registerToolView(toolviewTitle, KDevelop::IOutputView::MultipleView, KIcon());
    OutputWidget* outputWidget = toolviewPointer(toolviewTitle);
    QVERIFY(outputWidget);
    
    QList<QAction*> actions = outputWidget->actions();
    QCOMPARE(actions.takeFirst()->text(), QString("Select &All"));
    QCOMPARE(actions.takeFirst()->text(), QString("&Copy"));
    
    m_stdOutputView->removeToolView(toolviewId);
    QVERIFY(!toolviewPointer(toolviewTitle));
    
    toolviewId = m_stdOutputView->registerToolView(toolviewTitle, KDevelop::IOutputView::HistoryView, 
                                                   KIcon(), KDevelop::IOutputView::ShowItemsButton | KDevelop::IOutputView::AddFilterAction);
    outputWidget = toolviewPointer(toolviewTitle);
    QVERIFY(outputWidget);
    
    actions = outputWidget->actions();
    QCOMPARE(actions.takeFirst()->text(), QString("Previous"));
    QCOMPARE(actions.takeFirst()->text(), QString("Next"));
    QCOMPARE(actions.takeFirst()->text(), QString("Select activated Item"));
    QCOMPARE(actions.takeFirst()->text(), QString("Focus when selecting Item"));
    QCOMPARE(actions.takeFirst()->text(), QString("Select &All"));
    QCOMPARE(actions.takeFirst()->text(), QString("&Copy"));
    QCOMPARE(actions.takeFirst()->text(), QString(""));
    QCOMPARE(actions.takeFirst()->text(), QString("Filter"));
    
    //TODO test added actions
    
    m_stdOutputView->removeToolView(toolviewId);
    QVERIFY(!toolviewPointer(toolviewTitle));
}

void StandardOutputViewTest::testRegisterAndRemoveOutput()
{
    toolviewId = m_stdOutputView->registerToolView(toolviewTitle, KDevelop::IOutputView::MultipleView, KIcon());
    OutputWidget* outputWidget = toolviewPointer(toolviewTitle);
    QVERIFY(outputWidget);
    
    for(int i = 0; i < 5; i++)
    {
        outputId[i] = m_stdOutputView->registerOutputInToolView(toolviewId, "output" + i);
    }
    for(int i = 0; i < 5; i++)
    {
        QCOMPARE(outputWidget->data->outputdata.value(outputId[i])->title, QString("output" + i));
        QCOMPARE(outputWidget->tabwidget->tabText(i), QString("output" + i));
    }
    for(int i = 0; i < 5; i++)
    {
        m_stdOutputView->removeOutput(outputId[i]);
        QVERIFY(!outputWidget->data->outputdata.contains(outputId[i]));
    }
    QCOMPARE(outputWidget->tabwidget->count(), 0);
    
    m_stdOutputView->removeToolView(toolviewId);
    QVERIFY(!toolviewPointer(toolviewTitle));
    
    toolviewId = m_stdOutputView->registerToolView(toolviewTitle, KDevelop::IOutputView::HistoryView, 
                                                   KIcon(), KDevelop::IOutputView::ShowItemsButton | KDevelop::IOutputView::AddFilterAction);
    outputWidget = toolviewPointer(toolviewTitle);
    QVERIFY(outputWidget);
    
    for(int i = 0; i < 5; i++)
    {
        outputId[i] = m_stdOutputView->registerOutputInToolView(toolviewId, "output" + i);
    }
    for(int i = 0; i < 5; i++)
    {
        QCOMPARE(outputWidget->data->outputdata.value(outputId[i])->title, QString("output" + i));
        //QCOMPARE(outputWidget->stackwidget->currentIndex(), i);
    }
    for(int i = 0; i < 5; i++)
    {
        m_stdOutputView->removeOutput(outputId[i]);
        QVERIFY(!outputWidget->data->outputdata.contains(outputId[i]));
    }
    QCOMPARE(outputWidget->stackwidget->count(), 0);
    
    m_stdOutputView->removeToolView(toolviewId);
    QVERIFY(!toolviewPointer(toolviewTitle));
}

void StandardOutputViewTest::testSetModelAndDelegate()
{
    toolviewId = m_stdOutputView->registerToolView(toolviewTitle, KDevelop::IOutputView::MultipleView, KIcon());
    OutputWidget* outputWidget = toolviewPointer(toolviewTitle);
    QVERIFY(outputWidget);
    
    outputId[0] = m_stdOutputView->registerOutputInToolView(toolviewId, "output");
    QAbstractItemModel* model = new QStandardItemModel();
    m_stdOutputView->setModel(outputId[0], model, KDevelop::IOutputView::KeepOwnership);
    QAbstractItemDelegate* delegate = new QItemDelegate();
    m_stdOutputView->setDelegate(outputId[0], delegate, KDevelop::IOutputView::KeepOwnership);
    
    QCOMPARE(outputWidget->views.value(outputId[0])->model(), model);
    QCOMPARE(outputWidget->views.value(outputId[0])->itemDelegate(), delegate);
    
    m_stdOutputView->removeToolView(toolviewId);
    QVERIFY(!toolviewPointer(toolviewTitle));
    
    QVERIFY(!model->parent()); // they don't have any parent, so parent() == 0x0
    QVERIFY(!delegate->parent());
    
    toolviewId = m_stdOutputView->registerToolView(toolviewTitle, KDevelop::IOutputView::HistoryView, KIcon());
    outputWidget = toolviewPointer(toolviewTitle);
    QVERIFY(outputWidget);
    
    outputId[0] = m_stdOutputView->registerOutputInToolView(toolviewId, "output");
    m_stdOutputView->setModel(outputId[0], model, KDevelop::IOutputView::TakeOwnership);
    m_stdOutputView->setDelegate(outputId[0], delegate, KDevelop::IOutputView::TakeOwnership);
    
    QCOMPARE(outputWidget->views.value(outputId[0])->model(), model);
    QCOMPARE(outputWidget->views.value(outputId[0])->itemDelegate(), delegate);

    QVERIFY(model->parent()); // they have a parent (the outputdata), so parent() != 0x0
    QVERIFY(delegate->parent());
    
    m_stdOutputView->removeToolView(toolviewId);
    QVERIFY(!toolviewPointer(toolviewTitle));
    
}