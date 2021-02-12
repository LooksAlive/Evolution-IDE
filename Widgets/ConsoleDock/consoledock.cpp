#include <QString>
#include <iostream>
#include <QDebug>
#include <QShortcut>

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
void ConsoleDock::BuildConsole()
{
    MainLayout = new QHBoxLayout();
    tool_bar = new QToolBar(this);
    title_bar = new QToolBar(this);
    ConsoleOutput = new QPlainTextEdit(this);
    ConsoleOutput->setReadOnly(true);   // for now
    ConsoleOutput->appendPlainText("This is Evolution-IDE");

    tool_bar->addAction(QIcon(IconFactory::ScrollUp), "Scroll Up", this, SLOT(slotScrollUp()));
    tool_bar->addAction(QIcon(IconFactory::ScrollDown), "Scroll Down", this, SLOT(slotScrollDown()));
    tool_bar->addAction(QIcon(IconFactory::TrashBin), "Clear", this, SLOT(slotClearConsole()));

    tool_bar->setOrientation(Qt::Vertical);
    tool_bar->setMovable(false);
    tool_bar->setFixedWidth(35);
    tool_bar->setFloatable(false);
    tool_bar->setAcceptDrops(false);
    tool_bar->setIconSize(QSize(35, 25));
    tool_bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    tool_bar->setContentsMargins(0, 0, 0, 0);

    title_bar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));

    title_bar->setMovable(false);
    title_bar->setFixedHeight(35);
    title_bar->setFloatable(false);
    title_bar->setAcceptDrops(false);
    title_bar->setIconSize(QSize(25, 35));
    title_bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    MainLayout->addWidget(tool_bar);
    MainLayout->addWidget(ConsoleOutput);

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);
}

void ConsoleDock::setRawOutput(const QString &raw){
    ConsoleOutput->appendPlainText(raw);
}

void ConsoleDock::slotClearConsole(){
    ConsoleOutput->clear();
}

void ConsoleDock::slotScrollUp(){
    QPoint pos = ConsoleOutput->viewport()->pos();
    ConsoleOutput->scroll(0, pos.y() - 5);
}

void ConsoleDock::slotScrollDown(){
    QPoint pos = ConsoleOutput->viewport()->pos();
    ConsoleOutput->scroll(0, pos.y() + 5);
}
