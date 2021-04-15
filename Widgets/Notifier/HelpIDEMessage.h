#ifndef HELPIDEMESSAGE_H
#define HELPIDEMESSAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

class HelpIDEMessage : public QWidget
{
    Q_OBJECT
public:
    explicit HelpIDEMessage(QWidget *parent = nullptr);
    ~HelpIDEMessage() = default;


    QPushButton *contact;

private:
    void createWindow();
    QHBoxLayout *MainLayout;
    QLabel *MainMessaage;
    QPushButton *close;



};

#endif // HELPIDEMESSAGE_H
