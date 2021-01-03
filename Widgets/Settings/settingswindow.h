#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialogButtonBox>
#include <QListWidget>
#include <QStackedWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QSettings>

#include "CmakeGenerator/cmagegeneratorwidget.h"
#include "git/gitwidget.h"

class SettingsWindow : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

private:

    QDialogButtonBox *ButtonBox;
    QListWidget *OptionsList;
    QStackedWidget *WidgetStack;
    QHBoxLayout *InnerLayout;
    QVBoxLayout *OuterLayout;

    CmageGeneratorWidget *cmake;
    GitWidget *git;

    QWidget *buildButtonBox();
    QLayout *buildForm();



signals:



private slots:
    /*
    void saveData();    // every widget save data function is called here [ save button ]
    void loadData();    // loading data from settings -> all subwidgets load functions [ in constructor init. ]
    */
};

#endif // SETTINGSWINDOW_H
