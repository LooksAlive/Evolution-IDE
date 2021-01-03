#ifndef CMAGEGENERATORWIDGET_H
#define CMAGEGENERATORWIDGET_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QWidget>
#include <QSettings>

#include "cmakegenerator.h"

/*

 cannot parse cmake --> no api for that -> only generating purposes

  2 parts:
    1. cmake -> view all files in project, generation options like
    precompiled headers, preprocess whole project into 1 file, ...
    2. line execution
  #TODO: option -j -> cores + all of options defined in cmakegenerator.h
*/


class CmageGeneratorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CmageGeneratorWidget(QWidget *parent = nullptr);
    ~CmageGeneratorWidget();

    void loadSettings();
    void saveSettings();




private:
    // cmake
    QString flags = "";
    QLabel *cores;

    QLabel *cmake_part;
    QLabel *cmake_flags;

    QLabel *console_part;
    QLabel *console_flags;


signals:

};

#endif // CMAGEGENERATORWIDGET_H
