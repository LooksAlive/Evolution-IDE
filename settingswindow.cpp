#include "settingswindow.h"



SettingsWindow::SettingsWindow(QWidget *parent) : QWidget(parent)
{
    OuterTab = new QTabWidget(this);
    GeneralTab = new QTabWidget(this);
    //OuterLayout = new QGridLayout();
    changelater = new QPlainTextEdit();

    // features

    SettingsWindow::setWindowTitle("Settings");
    SettingsWindow::setMinimumSize(400, 250);

    OuterTab->setMovable(false);
    OuterTab->setTabsClosable(false);
    OuterTab->setUsesScrollButtons(true);
    OuterTab->setTabPosition(QTabWidget::West);
    style = new CustomTabStyle();
    OuterTab->tabBar()->setStyle(style);

    OuterTab->addTab(GeneralTab, "General");
    OuterTab->addTab(new QTabWidget(), "OtherStuffs");

    GeneralTab->addTab(changelater, "1");
    GeneralTab->addTab(new QPlainTextEdit(), "2");
    GeneralTab->addTab(new QPlainTextEdit(), "3");

    // for exmpl.
    // OuterTab->addTab(GeneralTab, "OtherStuffs");

    //OuterLayout->addWidget(OuterTab);
    //OuterLayout->addWidget(GeneralTab);
    //OuterTab->setLayout(OuterLayout);


}
