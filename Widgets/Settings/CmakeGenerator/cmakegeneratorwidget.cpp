#include "cmakegeneratorwidget.h"

CMakeGeneratorWidget::CMakeGeneratorWidget(QWidget *parent) : QWidget(parent){
    createMainWindow();
    setDefaultSettings();
    manageSourceFiles();
    getCompilerVersion();

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(4);
    MainLayout->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    setLayout(MainLayout);

    setFixedSize(300, 300);
}
CMakeGeneratorWidget::~CMakeGeneratorWidget(){}


void CMakeGeneratorWidget::createMainWindow() {
    MainLayout = new QVBoxLayout(this);

    compiler_layout = new QHBoxLayout();
    compiler_tag = new QLabel(this);
    compiler_tag->setText("Compiler: ");
    compiler = new QLineEdit(this);
    compiler->setMaximumWidth(250);
    compiler_layout->addWidget(compiler_tag);
    compiler_layout->addWidget(compiler);

    compile_flags_layout = new QHBoxLayout();
    compile_flags_tag = new QLabel(this);
    compile_flags_tag->setText("Compile Flags: ");
    compile_flags = new QLineEdit(this);
    compile_flags->setMaximumWidth(250);
    compile_flags_layout->addWidget(compile_flags_tag);
    compile_flags_layout->addWidget(compile_flags);

    cpu_cores_layout = new QHBoxLayout();
    cpu_cores_tag = new QLabel(this);
    cpu_cores_tag->setText("CPU Cores: ");
    cpu_cores = new QLineEdit(this);
    cpu_cores->setMaximumWidth(250);
    cpu_cores_layout->addWidget(cpu_cores_tag);
    cpu_cores_layout->addWidget(cpu_cores);

    source_files_layout = new QHBoxLayout();
    source_files_tag = new QLabel(this);
    source_files_tag->setText("Source Files");
    source_files_list = new QListWidget(this);
    source_files_list->setMaximumSize(500, 200);
    source_files_layout->addWidget(source_files_tag);
    source_files_layout->addWidget(source_files_list);

    MainLayout->addLayout(compiler_layout);
    MainLayout->addLayout(compile_flags_layout);
    MainLayout->addLayout(cpu_cores_layout);
    MainLayout->addLayout(source_files_layout);

}

void CMakeGeneratorWidget::getCompilerVersion() {
    // build in c functions for that
}






void CMakeGeneratorWidget::setDefaultSettings() {
    compiler->setText("clang++-10");
    compile_flags->setText("-O0 -g -Wall");
    cpu_cores->setText("3");
}

void CMakeGeneratorWidget::manageSourceFiles() {
    // if QSettings contain files -> they are iterated in filemanager, soo
    // here only saving and managing them
    // add 2 buttons: 1. add file with QFileDialog connection, 2. remove - and update list

    QStringList sources = settings.value("Evolution/sources").toStringList();
    for (int i = 0; i < sources.size(); i++) { // get files from registry
        source_files_list->addItem(new QListWidgetItem(sources[i]));
    }
}





void CMakeGeneratorWidget::loadData() {

    QString co = settings.value("Evolution/compiler").toString();
    QString cf = settings.value("Evolution/compile_flags").toString();
    QString cr = settings.value("Evolution/cpu_cores").toString();

    compiler->setText(co);
    compile_flags->setText(cf);
    cpu_cores->setText(cr);
}

void CMakeGeneratorWidget::saveData() {
    settings.setValue("Evolution/compiler", compiler->text());
    settings.setValue("Evolution/compile_flags", compile_flags->text());
    settings.setValue("Evolution/cpu_cores", cpu_cores->text());
    // later maybe file deletion update settings
}





