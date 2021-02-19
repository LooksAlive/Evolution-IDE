#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

/*
 * is created when slot is called in menu, not in app init stage
*/

#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSettings>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "Appearence/AppearenceWidget.h"
#include "ClangFormat/ClangFormat.h"
#include "CmakeGenerator/cmakegeneratorwidget.h"
#include "Debugger/DebuggerSettings.h"
#include "Installer/Installer.h"
#include "git/gitwidget.h"

class SettingsWindow : public QDialog {
    Q_OBJECT
public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow() = default;

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
    ClangFormat *clangFormat;
    Installer *installer;

    QHBoxLayout *buttonsLayout;
    QPushButton *btn_save;
    QPushButton *btn_close;

    void createButtons();
    QLayout *buildForm();


signals:


private slots:

    void slotSaveData();    // every widget save data function is called here [ save button ]
    void slotLoadData();    // loading data from settings -> all subwidgets load functions [ in constructor init. ]

};

#endif // SETTINGSWINDOW_H
