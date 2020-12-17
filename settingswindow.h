#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QTabWidget>
#include <QGridLayout>
// #include <QVBoxLayout>
#include <QPlainTextEdit>

#include "customtabstyle.h"


class SettingsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWindow(QWidget *parent = nullptr);


private:
    QTabWidget *OuterTab;
    QTabWidget *GeneralTab;
    QGridLayout *OuterLayout;
    QPlainTextEdit *changelater;
    CustomTabStyle *style;

signals:



private slots:



};

#endif // SETTINGSWINDOW_H
