#include "icons/IconFactory.h"

#include "HelpIDEMessage.h"

HelpIDEMessage::HelpIDEMessage(QWidget *parent) : QWidget(parent) {
    createWindow();

    setFixedHeight(30);
    setMinimumWidth(200);
    setVisible(false);
}

void HelpIDEMessage::createWindow() {
    MainLayout = new QHBoxLayout(this);
    MainMessaage = new QLabel(this);
    contact = new QPushButton(this);
    close = new QPushButton(this);

    close->setIcon(QIcon(IconFactory::Remove));
    connect(close, SIGNAL(pressed()), this, SLOT(hide()));

    contact->setText("Contact Me");

    //auto *spacer = new QWidget(this);
    //spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    MainMessaage->setText("Help with ideas or development. Contact me here.");

    MainLayout->addWidget(MainMessaage);
    //MainLayout->addWidget(spacer);
    MainLayout->addWidget(contact, 0, Qt::AlignRight);
    MainLayout->addWidget(close, 0, Qt::AlignRight);


    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);
}
