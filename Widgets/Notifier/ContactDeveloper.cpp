#include "ContactDeveloper.h"

ContactDeveloper::ContactDeveloper(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::Dialog);

    createWindow();
    setLayout(MainLayout);
}

void ContactDeveloper::createWindow() {
    MainLayout = new QFormLayout();
    Description = new QLabel(this);
    Subject = new QLineEdit(this);
    Content = new QPlainTextEdit(this);
    Send = new QPushButton(this);
    Status = new QLabel(this);

    Description->setText("Send me a message by gmail smtp server.");
    Subject->setPlaceholderText("Subject");
    Send->setText("Send");
    Content->setPlaceholderText("Write a message here");
    Content->setLineWrapMode(QPlainTextEdit::NoWrap);

    MainLayout->addRow(Description);
    MainLayout->addRow(Subject);
    MainLayout->addRow(Content);
    MainLayout->addRow(Send);
    MainLayout->addRow(Status);

    Notifier = new SmtpMailNotifier(this);

    connect(Send, SIGNAL(pressed()), this, SLOT(slotSendMessage()));
}

void ContactDeveloper::slotSendMessage() {
    if (!Content->toPlainText().isEmpty()) {
        Notifier->sendMessage(Content->toPlainText(), Subject->text());
        Status->setText("Pending...");
    } else {
        Status->setText("Please add some message to send at first.");
    }
    connect(Notifier->reply, SIGNAL(finished()), this, SLOT(slotUpdateStatus()));
}

void ContactDeveloper::slotUpdateStatus() {
    Status->setText("ServerReply finished" + Notifier->reply->responseText());
    Notifier->reply->deleteLater();
}
