#ifndef EVOLUTION_IDE_APPEARENCEWIDGET_H
#define EVOLUTION_IDE_APPEARENCEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QToolButton>
#include <QFontDialog>
#include <QFont>
#include <QLineEdit>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include "EnvironmentSettings.h"

class AppearenceWidget : public QWidget {

public:
    explicit AppearenceWidget(QWidget *parent = nullptr);
    ~AppearenceWidget() = default;

    void saveData();
    void loadData();


private:

    void createWindow();

    void setDefaultSettings();

    QVBoxLayout *MainLayout;

    QHBoxLayout *theme_layout;
    QLabel *theme_tag;
    QComboBox *themes;

    QHBoxLayout *font_layout;
    QLabel *base_font_tag;   // font family name & height
    QLabel *base_font;
    QToolButton *btn_change_font;

    QFont font;

private slots:

    void slotChangeFont();


};




#endif //EVOLUTION_IDE_APPEARENCEWIDGET_H
