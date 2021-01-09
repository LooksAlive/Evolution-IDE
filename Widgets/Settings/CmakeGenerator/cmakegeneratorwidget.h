#ifndef CMAGEGENERATORWIDGET_H
#define CMAGEGENERATORWIDGET_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
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


class CMakeGeneratorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CMakeGeneratorWidget(QWidget *parent = nullptr);
    ~CMakeGeneratorWidget();

    void loadData();
    void saveData();


// later checkbox for debug and release option and flags associated with it

private:

    void createMainWindow();

    void setDefaultSettings();  // new project, no registry values
    void manageSourceFiles();
    void getCompilerVersion();

    CmakeGenerator generator;

    QVBoxLayout *MainLayout;

    QHBoxLayout *compiler_layout;
    QLabel *compiler_tag;
    QLineEdit *compiler;
    QHBoxLayout *compile_flags_layout;
    QLabel *compile_flags_tag;
    QLineEdit *compile_flags;
    QHBoxLayout *cpu_cores_layout;
    QLabel *cpu_cores_tag;
    QLineEdit *cpu_cores;

    QHBoxLayout *source_files_layout;
    QLabel *source_files_tag;
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
