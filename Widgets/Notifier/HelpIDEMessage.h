#ifndef HELPIDEMESSAGE_H
#define HELPIDEMESSAGE_H

#include <QWidget>
#include <QTextBrowser>
#include <QToolBar>
#include <QPushButton>
#include <QVBoxLayout>

class HelpIDEMessage : public QWidget
{
    Q_OBJECT
public:
    explicit HelpIDEMessage(QWidget *parent = nullptr);
    ~HelpIDEMessage() = default;


    QPushButton *contact;
    QPushButton *close;

    enum InfoType {
        Contact = 0,
        Update
    };

private:
    void createWindow();
    QVBoxLayout *MainLayout;

    QToolBar *topBar;

    QTextBrowser *MainMessaage;

};

#endif // HELPIDEMESSAGE_H
