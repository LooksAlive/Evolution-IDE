#ifndef EVOLUTION_IDE_CONTACTDEVELOPER_H
#define EVOLUTION_IDE_CONTACTDEVELOPER_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>

#include "SmtpMailNotifier.h"

class ContactDeveloper : public QWidget {
Q_OBJECT
public:
    explicit ContactDeveloper(QWidget *parent = nullptr);

    ~ContactDeveloper() = default;


private:
    void createWindow();

    QFormLayout *MainLayout;
    QLabel *Description;
    QLineEdit *Subject;
    QPushButton *Send;
    QPlainTextEdit *Content;
    QLabel *Status;

    SmtpMailNotifier *Notifier;

private slots:

    void slotSendMessage();

    void slotUpdateStatus();


};


#endif //EVOLUTION_IDE_CONTACTDEVELOPER_H
