/*
  Copyright 2007 Roberto Raggi <roberto@kdevelop.org>
  Copyright 2007 Hamish Rodda <rodda@kde.org>
  Copyright 2011 Alexander Dymo <adymo@kdevelop.org>

  Permission to use, copy, modify, distribute, and sell this software and its
  documentation for any purpose is hereby granted without fee, provided that
  the above copyright notice appear in all copies and that both that
  copyright notice and this permission notice appear in supporting
  documentation.

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
  KDEVELOP TEAM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
  AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "idealbuttonbarwidget.h"
#include "mainwindow.h"
#include "idealdockwidget.h"
#include "ideallayout.h"
#include "idealtoolbutton.h"
#include "document.h"
#include "view.h"

#include <KLocalizedString>
#include <KSharedConfig>
#include <KConfigGroup>

#include <QBoxLayout>
#include <QApplication>

using namespace Sublime;

class ToolViewAction : public QAction
{
    Q_OBJECT

public:
    ToolViewAction(IdealDockWidget *dock, QObject* parent) : QAction(parent), m_dock(dock)
    {
        setCheckable(true);

        const QString title = dock->view()->document()->title();
        setIcon(dock->windowIcon());
        setToolTip(i18n("Toggle '%1' tool view.", title));
        setText(title);

        //restore toolview shortcut config
        KConfigGroup config = KSharedConfig::openConfig()->group("UI");
        QStringList shortcutStrings = config.readEntry(QStringLiteral("Shortcut for %1").arg(title), QStringList());
        setShortcuts({ QKeySequence::fromString(shortcutStrings.value(0)), QKeySequence::fromString(shortcutStrings.value(1)) });

        dock->setWindowTitle(title);
        dock->view()->widget()->installEventFilter(this);
        refreshText();
    }

    IdealDockWidget *dockWidget() const
    {
        Q_ASSERT(m_dock);
        return m_dock;
    }

    IdealToolButton* button() { return m_button; }
    void setButton(IdealToolButton* button) {
        m_button = button;
        refreshText();
    }

    QString id() {
        return m_dock->view()->document()->documentSpecifier();
    }

private:
    bool eventFilter(QObject * watched, QEvent * event) override
    {
        if (watched == m_dock->view()->widget() && event->type() == QEvent::EnabledChange) {
            refreshText();
        }

        return QObject::eventFilter(watched, event);
    }

    void refreshText()
    {
        const auto widget = m_dock->view()->widget();
        const QString title = m_dock->view()->document()->title();
        setText(widget->isEnabled() ? title : QStringLiteral("(%1)").arg(title));
    }

    QPointer<IdealDockWidget> m_dock;
    QPointer<IdealToolButton> m_button;
};

IdealButtonBarWidget::IdealButtonBarWidget(Qt::DockWidgetArea area,
        IdealController *controller, Sublime::MainWindow *parent)
    : QWidget(parent)
    , m_area(area)
    , m_controller(controller)
    , m_corner(nullptr)
    , m_showState(false)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setToolTip(i18nc("@info:tooltip", "Right click to add new tool views."));

    if (area == Qt::BottomDockWidgetArea)
    {
        QBoxLayout *statusLayout = new QBoxLayout(QBoxLayout::RightToLeft, this);
        statusLayout->setMargin(0);

        IdealButtonBarLayout *l = new IdealButtonBarLayout(orientation());
        statusLayout->addLayout(l);

        m_corner = new QWidget(this);
        QBoxLayout *cornerLayout = new QBoxLayout(QBoxLayout::LeftToRight, m_corner);
        cornerLayout->setMargin(0);
        cornerLayout->setSpacing(0);
        statusLayout->addWidget(m_corner);
        statusLayout->addStretch(1);
    }
    else
        (void) new IdealButtonBarLayout(orientation(), this);
}

QAction* IdealButtonBarWidget::addWidget(IdealDockWidget *dock,
                                         Area *area, View *view)
{
    if (m_area == Qt::BottomDockWidgetArea || m_area == Qt::TopDockWidgetArea)
        dock->setFeatures( dock->features() | IdealDockWidget::DockWidgetVerticalTitleBar );

    dock->setArea(area);
    dock->setView(view);
    dock->setDockWidgetArea(m_area);

    auto action = new ToolViewAction(dock, this);
    addAction(action);

    return action;
}

QWidget* IdealButtonBarWidget::corner()
{
    return m_corner;
}

void IdealButtonBarWidget::addAction(QAction* qaction)
{
    QWidget::addAction(qaction);

    auto action = dynamic_cast<ToolViewAction*>(qaction);
    if (!action || action->button()) {
      return;
    }

    bool wasEmpty = isEmpty();

    IdealToolButton *button = new IdealToolButton(m_area);
    //apol: here we set the usual width of a button for the vertical toolbars as the minimumWidth
    //this is done because otherwise when we remove all the buttons and re-add new ones we get all
    //the screen flickering. This is solved by not defaulting to a smaller width when it's empty
    int w = button->sizeHint().width();
    if (orientation() == Qt::Vertical && w > minimumWidth()) {
        setMinimumWidth(w);
    }

    action->setButton(button);
    button->setDefaultAction(action);

    Q_ASSERT(action->dockWidget());

    connect(action, &QAction::toggled, this, static_cast<void(IdealButtonBarWidget::*)(bool)>(&IdealButtonBarWidget::showWidget));
    connect(button, &IdealToolButton::customContextMenuRequested,
            action->dockWidget(), &IdealDockWidget::contextMenuRequested);

    addButtonToOrder(button);
    applyOrderToLayout();

    if (wasEmpty) {
        emit emptyChanged();
    }
}

void IdealButtonBarWidget::removeAction(QAction* widgetAction)
{
    QWidget::removeAction(widgetAction);

    auto action = static_cast<ToolViewAction*>(widgetAction);
    action->button()->deleteLater();
    delete action;

    if (layout()->isEmpty()) {
        emit emptyChanged();
    }
}

bool IdealButtonBarWidget::isEmpty()
{
    return actions().isEmpty();
}

bool IdealButtonBarWidget::isShown()
{
    const auto actions = this->actions();
    return std::any_of(actions.cbegin(), actions.cend(),
                       [](const QAction* action){ return action->isChecked(); });
}

void IdealButtonBarWidget::saveShowState()
{
    m_showState = isShown();
}

bool IdealButtonBarWidget::lastShowState()
{
    return m_showState;
}

QString IdealButtonBarWidget::id(const IdealToolButton* button) const
{
    foreach (QAction* a, actions()) {
        auto tva = dynamic_cast<ToolViewAction*>(a);
        if (tva && tva->button() == button) {
            return tva->id();
        }
    }

    return QString();
}

IdealToolButton* IdealButtonBarWidget::button(const QString& id) const
{
    foreach (QAction* a, actions()) {
        auto tva = dynamic_cast<ToolViewAction*>(a);
        if (tva && tva->id() == id) {
            return tva->button();
        }
    }

    return nullptr;
}

void IdealButtonBarWidget::addButtonToOrder(const IdealToolButton* button)
{
    QString buttonId = id(button);
    if (!m_buttonsOrder.contains(buttonId)) {
        if (m_area == Qt::BottomDockWidgetArea) {
            m_buttonsOrder.push_front(buttonId);
        }
        else {
            m_buttonsOrder.push_back(buttonId);
        }
    }
}

void IdealButtonBarWidget::loadOrderSettings(const KConfigGroup& configGroup)
{
    m_buttonsOrder = configGroup.readEntry(QStringLiteral("(%1) Tool Views Order").arg(m_area), QStringList());
    applyOrderToLayout();
}

void IdealButtonBarWidget::saveOrderSettings(KConfigGroup& configGroup)
{
    takeOrderFromLayout();
    configGroup.writeEntry(QStringLiteral("(%1) Tool Views Order").arg(m_area), m_buttonsOrder);
}

bool IdealButtonBarWidget::isLocked() const
{
    KConfigGroup config = KSharedConfig::openConfig()->group("UI");
    return config.readEntry(QStringLiteral("Toolview Bar (%1) Is Locked").arg(m_area), false);
}

void IdealButtonBarWidget::applyOrderToLayout()
{
    // If widget already have some buttons in the layout then calling loadOrderSettings() may leads
    // to situations when loaded order does not contains all existing buttons. Therefore we should
    // fix this with using addToOrder() method.
    for (int i = 0; i < layout()->count(); ++i) {
        if (auto button = dynamic_cast<IdealToolButton*>(layout()->itemAt(i)->widget())) {
            addButtonToOrder(button);
            layout()->removeWidget(button);
        }
    }

    foreach(const QString& id, m_buttonsOrder) {
        if (auto b = button(id)) {
            layout()->addWidget(b);
        }
    }
}

void IdealButtonBarWidget::takeOrderFromLayout()
{
    m_buttonsOrder.clear();
    for (int i = 0; i < layout()->count(); ++i) {
        if (auto button = dynamic_cast<IdealToolButton*>(layout()->itemAt(i)->widget())) {
            m_buttonsOrder += id(button);
        }
    }
}

Qt::Orientation IdealButtonBarWidget::orientation() const
{
    if (m_area == Qt::LeftDockWidgetArea || m_area == Qt::RightDockWidgetArea)
        return Qt::Vertical;

    return Qt::Horizontal;
}

Qt::DockWidgetArea IdealButtonBarWidget::area() const
{
    return m_area;
}

void IdealButtonBarWidget::showWidget(bool checked)
{
    Q_ASSERT(parentWidget() != nullptr);

    QAction *action = qobject_cast<QAction *>(sender());
    Q_ASSERT(action);

    showWidget(action, checked);
}

void IdealButtonBarWidget::showWidget(QAction *action, bool checked)
{
    auto widgetAction = static_cast<ToolViewAction*>(action);

    IdealToolButton* button = widgetAction->button();
    Q_ASSERT(button);

    if (checked) {
        if ( !QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) ) {
            // Make sure only one widget is visible at any time.
            // The alternative to use a QActionCollection and setting that to "exclusive"
            // has a big drawback: QActions in a collection that is exclusive cannot
            // be un-checked by the user, e.g. in the View -> Tool Views menu.
            foreach(QAction *otherAction, actions()) {
                if ( otherAction != widgetAction && otherAction->isChecked() )
                    otherAction->setChecked(false);
            }
        }

        m_controller->lastDockWidget[m_area] = widgetAction->dockWidget();
    }

    m_controller->showDockWidget(widgetAction->dockWidget(), checked);
    widgetAction->setChecked(checked);
    button->setChecked(checked);
}

IdealDockWidget * IdealButtonBarWidget::widgetForAction(QAction *_action) const
{
    return static_cast<ToolViewAction*>(_action)->dockWidget();
}

#include "idealbuttonbarwidget.moc"
