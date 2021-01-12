#include "AppearenceWidget.h"

AppearenceWidget::AppearenceWidget(QWidget *parent) : QWidget(parent){
    createWindow();
    setDefaultSettings();

    setLayout(MainLayout);
    setFixedSize(300, 300);
}


void AppearenceWidget::createWindow() {

    MainLayout = new QVBoxLayout(this);

    theme_layout = new QHBoxLayout();
    theme_tag = new QLabel(this);
    theme_tag->setText("Theme: ");
    themes = new QComboBox(this);
    themes->addItem("White");
    themes->addItem("Dark");
    theme_layout->addWidget(theme_tag);
    theme_layout->addWidget(themes);

    font_layout = new QHBoxLayout();
    base_font_tag = new QLabel(this);
    base_font_tag->setText("Font: ");
    base_font = new QLabel(this);
    font_layout->addWidget(base_font_tag);
    font_layout->addWidget(base_font);

    btn_change_font = new QToolButton(this);
    btn_change_font->setText("Change");
    font_layout->addWidget(btn_change_font);
    connect(btn_change_font, SIGNAL(clicked()), this, SLOT(slotChangeFont()));

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(2);
    MainLayout->addLayout(theme_layout);
    MainLayout->addLayout(font_layout);
}

void AppearenceWidget::setDefaultSettings() {
    themes->setCurrentText("Dark");
    QString temp = font.defaultFamily() + " " + font.weight();
    base_font->setText(temp);

    settings.setValue("Evolution/theme", "Dark");
    settings.setValue("Evolution/font_family", font.defaultFamily());
    settings.setValue("Evolution/font_weight", font.weight());
}


void AppearenceWidget::saveData() {
    settings.setValue("Evolution/theme", themes->currentText());
    settings.setValue("Evolution/font_family", font.defaultFamily());
    settings.setValue("Evolution/font_weight", font.weight());
}

void AppearenceWidget::loadData() {
    QString theme = settings.value("Evolution/theme").toString();
    QString font_family = settings.value("Evolution/font_family").toString();
    int weight = settings.value("Evolution/font_weight").toInt();

    themes->setCurrentText(theme);
    base_font->setText(font_family + " " + QString(weight));
}

void AppearenceWidget::slotChangeFont() {
    bool ok;
    font = QFontDialog::getFont(&ok, this);
    if(ok){
        saveData();
    }
}


