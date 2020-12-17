#include "settingswindow.h"
#include <QFont>
#include <QFontDialog>
#include <QGridLayout>
#include <QVBoxLayout>


SettingsWindow::SettingsWindow(QWidget *parent) : QWidget(parent)
{
    QFont font;
    font.setFamily("Console");

    QGridLayout *q = new QGridLayout();
}
