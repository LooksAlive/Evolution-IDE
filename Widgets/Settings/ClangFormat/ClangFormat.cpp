#include "ClangFormat.h"
#include <QSettings>

ClangFormat::ClangFormat(QWidget *parent) : QWidget(parent) {
    createWindow();
    setDefaultSettings();
}
void ClangFormat::createWindow() {
    MainLayout = new QFormLayout();
    style = new QComboBox(this);
    afterFileClosed = new QCheckBox("Format when a file is closed", this);

    style->addItem("LLVM");
    style->addItem("Google");
    style->addItem("Chromium");
    style->addItem("Mozilla");
    style->addItem("WebKit");
    style->addItem("Microsoft");
    style->addItem("GNU");
    afterFileClosed->setCheckable(true);

    MainLayout->addRow("format style", style);
    MainLayout->addRow(afterFileClosed);

    setLayout(MainLayout);
}
void ClangFormat::setDefaultSettings() {
    style->setCurrentText("LLVM");
    afterFileClosed->setChecked(false);
}


void ClangFormat::saveData() {
    QSettings settings("Evolution");
    settings.setValue("Evolution/format_style", style->currentText());
    settings.setValue("Evolution/format_afterFileClosed", afterFileClosed->isChecked());
}

void ClangFormat::loadData() {
    QSettings settings("Evolution");
    QString _style = settings.value("Evolution/format_style").toString();
    bool _afterFileClosed = settings.value("Evolution/format_afterFileClosed").toBool();

    style->setCurrentText(_style);
    afterFileClosed->setChecked(_afterFileClosed);
}
