#include "Installer.h"
#include "DependencyInstaller.h"
#include "commandlineexecutor.h"

Installer::Installer(QWidget *parent) : QWidget(parent) {
    createWindow();
}

void Installer::createWindow() {
    MainLayout = new QFormLayout();

    // fill them with which commands
    QString path;
    path = QString::fromStdString(CommandLineExecutor::ExecuteSimpleCommand("which clang"));
    clang = new QLineEdit(path, this);
    path = QString::fromStdString(CommandLineExecutor::ExecuteSimpleCommand("which gcc"));
    gcc = new QLineEdit(path, this);
    path = QString::fromStdString(CommandLineExecutor::ExecuteSimpleCommand("which llvm"));
    llvm = new QLineEdit(path, this);
    path = QString::fromStdString(CommandLineExecutor::ExecuteSimpleCommand("which gdb"));
    gdb = new QLineEdit(path, this);
    path = QString::fromStdString(CommandLineExecutor::ExecuteSimpleCommand("which lldb"));
    lldb = new QLineEdit(path, this);
    path = QString::fromStdString(CommandLineExecutor::ExecuteSimpleCommand("which qt5"));
    qt5_default = new QLineEdit(path, this);
    path = QString::fromStdString(CommandLineExecutor::ExecuteSimpleCommand("which python3"));
    python3 = new QLineEdit(path, this);

    MainLayout->addRow("clang", clang);
    MainLayout->addRow("gcc", gcc);
    MainLayout->addRow("llvm", llvm);
    MainLayout->addRow("gdb", gdb);
    MainLayout->addRow("lldb", lldb);
    MainLayout->addRow("Qt5-default", qt5_default);
    MainLayout->addRow("python3", python3);

    setLayout(MainLayout);
}


void Installer::saveData() {
}
void Installer::loadData() {
}
