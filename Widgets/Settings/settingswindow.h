#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialogButtonBox>
#include <QListWidget>
#include <QStackedWidget>
#include <QDialog>
// #include <QTabWidget>
// #include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>


class SettingsWindow : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsWindow(QWidget *parent = nullptr);

private:

    QDialogButtonBox *ButtonBox;
    QListWidget *OptionsList;
    QStackedWidget *WidgetStack;
    QHBoxLayout *InnerLayout;
    QVBoxLayout *OuterLayout;

    QWidget *buildButtonBox();
    QLayout *buildForm();


signals:



private slots:



};

#endif // SETTINGSWINDOW_H
