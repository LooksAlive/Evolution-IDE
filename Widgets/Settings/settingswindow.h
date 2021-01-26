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

#include "CmakeGenerator/cmakegeneratorwidget.h"
#include "git/gitwidget.h"
#include "Appearence/AppearenceWidget.h"
#include "Debugger/DebuggerSettings.h"

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

    CMakeGeneratorWidget *cmake;
    GitWidget *git;
    AppearenceWidget *appearence;
    DebuggerSettings *debugger;

    QWidget *buildButtonBox();
    QLayout *buildForm();



signals:



private slots:

    void slotSaveData();    // every widget save data function is called here [ save button ]
    void slotLoadData();    // loading data from settings -> all subwidgets load functions [ in constructor init. ]

};

#endif // SETTINGSWINDOW_H
