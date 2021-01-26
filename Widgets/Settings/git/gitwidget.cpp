#include "gitwidget.h"
#include <QSettings>

GitWidget::GitWidget(QWidget *parent) : QWidget(parent){
    createWindow();
}

void GitWidget::createWindow() {
    Main_Layout = new QFormLayout();

    user = new QLineEdit(this);
    email = new QLineEdit(this);
    link = new QLineEdit(this);

    Main_Layout->addRow("user", user);
    Main_Layout->addRow("email", email);
    Main_Layout->addRow("link", link);

    setLayout(Main_Layout);
}



void GitWidget::loadData() {
    QSettings settings("Evolution");

    user->setText("me");
    email->setText("my email");
    link->setText("my link");
}

void GitWidget::saveData() {

}
