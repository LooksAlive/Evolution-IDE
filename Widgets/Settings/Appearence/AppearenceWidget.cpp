#include <QSettings>

#include "AppearenceWidget.h"

AppearenceWidget::AppearenceWidget(QWidget *parent) : QWidget(parent) {
    createWindow();

    setDefaultSettings();

    //setLayout(MainLayout); // layout
    //setFixedSize(300, 300);
}


void AppearenceWidget::createWindow() {

    Main_Layout = new QFormLayout();
    theme_layout = new QHBoxLayout();

    comboFont = new QFontComboBox(this);
    spinFontSize = new QSpinBox(this);
    checkShowWhitespaces = new QCheckBox(this);
    radioThemeDark = new QRadioButton("Dark", this);
    radio_ThemeLight = new QRadioButton("Light", this);

    spinFontSize->setMinimum(10);
    spinFontSize->setSingleStep(1);

    theme_layout->addWidget(radio_ThemeLight);
    theme_layout->addWidget(radioThemeDark);

    Main_Layout->addRow("Theme", theme_layout);
    Main_Layout->addRow("Editor Font", comboFont);
    Main_Layout->addRow("Editor Font Size", spinFontSize);
    Main_Layout->addRow("Show Whitespaces?", checkShowWhitespaces);

    setLayout(Main_Layout);
}

void AppearenceWidget::setDefaultSettings() {
    comboFont->setCurrentText(font.defaultFamily());
    spinFontSize->setValue(10);
    radio_ThemeLight->setChecked(true);
    // add WhiteSpaces

    QSettings settings("Evolution");
    settings.setValue("Evolution/theme", "Light");// default theme
    settings.setValue("Evolution/font_family", font.defaultFamily());
    settings.setValue("Evolution/font_weight", font.weight());
}


void AppearenceWidget::saveData() {
    QSettings settings("Evolution");

    if (radioThemeDark->isChecked()) {
        settings.setValue("Evolution/theme", "Dark");
    } else {
        settings.setValue("Evolution/theme", "Light");
    }
    settings.setValue("Evolution/font_family", comboFont->currentText());
    settings.setValue("Evolution/font_weight", spinFontSize->value());
    // add WhiteSpaces
}

void AppearenceWidget::loadData() {
    QSettings settings("Evolution");
    QString theme = settings.value("Evolution/theme").toString();
    QString font_family = settings.value("Evolution/font_family").toString();
    int weight = settings.value("Evolution/font_weight").toInt();

    comboFont->setCurrentText(font_family);
    spinFontSize->setValue(weight);

    if (theme == "Dark") {
        radioThemeDark->setChecked(true);
    } else {
        radio_ThemeLight->setChecked(true);
    }
    // add WhiteSpaces
}


