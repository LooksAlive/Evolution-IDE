#include <QString>
#include <iostream>
#include <QDebug>
#include <QShortcut>
#include <QScrollBar>

#include "icons/IconFactory.h"
#include "consoledock.h"
/* enum QTabBar::Shape */

ConsoleDock::ConsoleDock(QWidget *parent) : QDockWidget(parent)
{
    /* QDockWidget inherited functions */
    setWindowTitle("Console");
    setVisible(false);
    setFeatures(AllDockWidgetFeatures);
    setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);

    // escape shortcut  -> will close the window
    connect(new QShortcut(Qt::Key_Escape, this), &QShortcut::activated, [=] {this->setVisible(false);});

    BuildConsole();
    setTitleBarWidget(title_bar);

    auto *window = new QWidget(this);
    window->setLayout(MainLayout);
    window->setContentsMargins(0, 0, 0, 0);

    setWidget(window);

}

// change to QListWidget most probably, bc. of specific widget shows its own data and do not know
// how to do that with tabs, at least yet
void ConsoleDock::BuildConsole() {
    MainLayout = new QHBoxLayout();
    tool_bar = new QToolBar(this);
    title_bar = new QToolBar(this);
    ConsoleOutput = new QPlainTextEdit(this);
    ConsoleOutput->setReadOnly(true);// for now
    ConsoleOutput->appendPlainText("This is Evolution-IDE");
    ConsoleOutput->setUpdatesEnabled(true);

    run = new QToolButton(this);
    run->setToolTip("Run");
    run->setIcon(QIcon(IconFactory::Run));
    rerun = new QToolButton(this);
    rerun->setToolTip("Rerun");
    rerun->setIcon(QIcon(IconFactory::Run));
    stop = new QToolButton(this);
    stop->setToolTip("Stop");
    stop->setIcon(QIcon(IconFactory::Stop));
    tool_bar->addWidget(run);
    tool_bar->addWidget(rerun);
    tool_bar->addWidget(stop);
    tool_bar->addAction(QIcon(IconFactory::ScrollUp), "Scroll Up", this, SLOT(slotScrollUp()));
    tool_bar->addAction(QIcon(IconFactory::ScrollDown), "Scroll Down", this, SLOT(slotScrollDown()));
    tool_bar->addAction(QIcon(IconFactory::TrashBin), "Clear", this, SLOT(slotClearConsole()));

    tool_bar->setOrientation(Qt::Vertical);
    tool_bar->setMovable(false);
    tool_bar->setFixedWidth(35);
    tool_bar->setFloatable(false);
    tool_bar->setAcceptDrops(false);
    //tool_bar->setIconSize(QSize(35, 25));
    tool_bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    tool_bar->setContentsMargins(0, 0, 0, 0);

    title_bar->setMovable(false);
    title_bar->setFixedHeight(30);
    title_bar->setFloatable(false);
    title_bar->setAcceptDrops(false);
    //title_bar->setIconSize(QSize(25, 35));
    title_bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    auto *spacer = new QWidget(this);// align title_bar to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    title_bar->addWidget(spacer);
    title_bar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));


    MainLayout->addWidget(tool_bar);
    MainLayout->addWidget(ConsoleOutput);

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);
}

void ConsoleDock::slotClearConsole() const {
    ConsoleOutput->clear();
}

void ConsoleDock::slotScrollUp() const {
    for (int i = 0; i <= 5; i++) {
        ConsoleOutput->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
    }
}

void ConsoleDock::slotScrollDown() const {
    for (int i = 0; i <= 5; i++) {
        ConsoleOutput->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
    }
}
