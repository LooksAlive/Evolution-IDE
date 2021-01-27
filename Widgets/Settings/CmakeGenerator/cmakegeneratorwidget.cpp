#include <QSettings>

#include "cmakegeneratorwidget.h"

CMakeGeneratorWidget::CMakeGeneratorWidget(QWidget *parent) : QWidget(parent){
    createMainWindow();
    QSettings settings("Evolution");
    bool def = settings.value("Evolution/SetDefaultSettings").toBool();
    if(def){
        setDefaultSettings();
    }
    manageSourceFiles();
    getCompilerVersion();

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(4);
    MainLayout->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    setLayout(MainLayout);

    //setFixedSize(300, 300);
}
CMakeGeneratorWidget::~CMakeGeneratorWidget(){}


void CMakeGeneratorWidget::createMainWindow() {
    MainLayout = new QFormLayout(this);

    generate_cmake = new QCheckBox(this);
    cmake_path = new QLineEdit(this);
    compiler = new QLineEdit(this);
    compile_flags = new QLineEdit(this);
    cpu_cores = new QLineEdit(this);
    source_files_list = new QListWidget(this);

    MainLayout->setContentsMargins(10, 10, 10, 10);
    MainLayout->setSpacing(5);
    setDefaultSettings();       // *********************************************************************

    MainLayout->addRow("Generate cmake: ", generate_cmake);
    MainLayout->addRow("Cmake path: ", cmake_path);
    MainLayout->addRow("Compiler ", compiler);
    MainLayout->addRow("Compile Flags: ", compile_flags);
    MainLayout->addRow("CPU Cores ", cpu_cores);
    MainLayout->addRow("Source Files", source_files_list);

}

void CMakeGeneratorWidget::getCompilerVersion() {
    // build in c functions for that
}




void CMakeGeneratorWidget::setDefaultSettings() {
    generate_cmake->setChecked(true);
    compiler->setText("clang++-10");
    compile_flags->setText("-O0 -g -Wall");
    cpu_cores->setText("3");
}

void CMakeGeneratorWidget::manageSourceFiles() {
    // if QSettings contain files -> they are iterated in filemanager, soo
    // here only saving and managing them
    // add 2 buttons: 1. add file with QFileDialog connection, 2. remove - and update list

    QSettings settings("Evolution");
    QStringList sources = settings.value("Evolution/sources").toStringList();
    for (int i = 0; i < sources.size(); i++) { // get files from registry
        source_files_list->addItem(new QListWidgetItem(sources[i]));
    }
}





void CMakeGeneratorWidget::loadData() {

    QSettings settings("Evolution");
    bool generate = settings.value("Evolution/generate_cmake").toBool();
    QString cmakePath = settings.value("Evolution/cmake_path").toString();
    QString co = settings.value("Evolution/compiler").toString();
    QString cf = settings.value("Evolution/compile_flags").toString();
    QString cr = settings.value("Evolution/cpu_cores").toString();

    generate_cmake->setCheckable(generate);
    cmake_path->setText(cmakePath);
    compiler->setText(co);
    compile_flags->setText(cf);
    cpu_cores->setText(cr);
}

void CMakeGeneratorWidget::saveData() {
    QSettings settings("Evolution");
    settings.setValue("Evolution/compiler", compiler->text());
    settings.setValue("Evolution/compile_flags", compile_flags->text());
    settings.setValue("Evolution/cpu_cores", cpu_cores->text());
    // later maybe file deletion update settings
}





