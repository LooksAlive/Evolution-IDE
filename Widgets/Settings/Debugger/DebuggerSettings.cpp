#include "DebuggerSettings.h"
#include <QSettings>

DebuggerSettings::DebuggerSettings(QWidget *parent) : QWidget(parent){
    createWindow();
}

void DebuggerSettings::createWindow() {
    Main_Layout = new QFormLayout();
    executable = new QLineEdit(this);

    Main_Layout->addRow("executable", executable);

    setLayout(Main_Layout);
}




void DebuggerSettings::saveData() {
    QSettings settings("Evolution");
    QString exec_file = executable->text();

    settings.setValue("Evolution/executable_path", exec_file);
}

void DebuggerSettings::loadData() {
    QSettings settings("Evolution");
    QString exec_file = settings.value("Evolution/executable_path").toString();

    executable->setText(exec_file);
}