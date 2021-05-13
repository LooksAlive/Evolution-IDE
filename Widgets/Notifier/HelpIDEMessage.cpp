#include "icons/IconFactory.h"

#include "HelpIDEMessage.h"

HelpIDEMessage::HelpIDEMessage(QWidget *parent) : QWidget(parent) {
    createWindow();

    setFixedSize(250, 100);
    setVisible(false);
}

void HelpIDEMessage::createWindow() {
    MainLayout = new QVBoxLayout(this);
    MainMessaage = new QTextBrowser(this);
    contact = new QPushButton(this);
    close = new QPushButton(this);
    topBar = new QToolBar(this);

    topBar->setOrientation(Qt::Horizontal);
    topBar->setFixedHeight(25);
    topBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    auto *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    topBar->addWidget(spacer);
    topBar->addWidget(close);

    close->setIcon(QIcon(IconFactory::Remove));
    // connect(close, SIGNAL(pressed()), this, SLOT(hide()));

    contact->setText("Contact Me");

    //auto *spacer = new QWidget(this);
    //spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    MainMessaage->setPlainText("Help with ideas or development. Contact me here.");
    MainMessaage->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    MainMessaage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    MainMessaage->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    MainLayout->addWidget(topBar);
    MainLayout->addWidget(MainMessaage);
    //MainLayout->addWidget(spacer);
    MainLayout->addWidget(contact);

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);
}
