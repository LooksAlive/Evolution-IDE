#ifndef EVOLUTION_IDE_APPEARENCEWIDGET_H
#define EVOLUTION_IDE_APPEARENCEWIDGET_H

#include <QWidget>
#include <QFont>
#include <QLineEdit>

#include <QApplication>
#include <QFontComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QRadioButton>

#include <QVBoxLayout>
#include <QHBoxLayout>

class AppearenceWidget : public QWidget {
    Q_OBJECT
public:
    explicit AppearenceWidget(QWidget *parent = nullptr);
    ~AppearenceWidget() = default;

    void saveData();
    void loadData();


private:

    QFormLayout *Main_Layout;
    QHBoxLayout *theme_layout;
    QCheckBox *checkShowWhitespaces;
    QRadioButton *radioThemeDark;
    QRadioButton *radio_ThemeLight;
    QComboBox *comboFont;
    QSpinBox *spinFontSize;

    void createWindow();
    // always set at beginning, it is rewritten by registers(loadData, saveData)
    void setDefaultSettings();

    QFont font;  // no use, delete later
};




#endif //EVOLUTION_IDE_APPEARENCEWIDGET_H
