#ifndef CMAGEGENERATORWIDGET_H
#define CMAGEGENERATORWIDGET_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QCheckBox>
#include <QWidget>
#include <QSettings>
#include <QString>
#include <QStringList>

#include "cmakegenerator.h"

/*

 cannot parse cmake --> no api for that -> only generating purposes

  2 parts:
    1. cmake -> view all files in project, generation options like
    precompiled headers, preprocess whole project into 1 file, ...
    2. line execution
  #TODO: option -j -> cores + all of options defined in cmakegenerator.h
*/


class CMakeGeneratorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CMakeGeneratorWidget(QWidget *parent = nullptr);
    ~CMakeGeneratorWidget() = default;

    void loadData();
    void saveData();


    // later checkbox for debug and release option and flags associated with it

private:
    void createMainWindow();

    // always set at beginning, it is rewritten by registers(loadData, saveData)
    void setDefaultSettings();
    void manageSourceFiles();
    void getCompilerVersion();

    // CmakeGenerator generator;

    QFormLayout *MainLayout;

    QCheckBox *generate_cmake;
    QLineEdit *cmakelists_path;
    QLineEdit *cmake_build_dir;
    //QLineEdit *cmake_build_temp_dir;    // when only 1 file not saved in editor -> compiling...
    QLineEdit *compiler;
    QLineEdit *compile_flags;
    QLineEdit *cpu_cores;
    QListWidget *source_files_list;

    /*
    // libs, include paths, other options for generating cmake code from known data (preprocessing,pre-compiled libs...)
    QLabel *libraries_tag;
    QListWidget *libraries;
    QLabel *include_paths_tag;
    QListWidget *include_paths;
    */

};

#endif // CMAGEGENERATORWIDGET_H
