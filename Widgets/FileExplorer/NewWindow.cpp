#include "NewWindow.h"


NewWindow::NewWindow(QWidget *parent) : QWidget(parent){}

void NewWindow::dir_Window() {
    dir_layout = new QFormLayout();
    dir_input = new QLineEdit(this);
    dir_layout->addRow("Directory: ", dir_input);

    setFixedSize(250, 50);
    setWindowFlags(Qt::Dialog);
    setWindowTitle("New directory");
    setCursor(cursor());  // set into middle of app view
    setLayout(dir_layout);
    show();

    connect(dir_input, SIGNAL(returnPressed()), this, SLOT(slotDir()));
}

void NewWindow::slotDir() {
    new_dirname = dir_input->text();
    emit PartialWidgetDestroyed();
    close();
}

void NewWindow::file_Window() {
    file_layout = new QFormLayout();
    file_input = new QLineEdit(this);
    file_layout->addRow("File name: ", file_input);

    setFixedSize(250, 50);
    setWindowFlags(Qt::Dialog);
    setWindowTitle("New File");
    setCursor(cursor());  // set into middle of app view
    setLayout(file_layout);
    show();

    connect(file_input, SIGNAL(returnPressed()), this, SLOT(slotFile()));
    setLayout(file_layout);
}

void NewWindow::slotFile() {
    new_file = file_input->text();
    emit PartialWidgetDestroyed();
    close();
}

void NewWindow::C_CPP_CLASS_Window() {
    C_CPP_CLASS_layout = new QFormLayout();
    ClassName_input = new QLineEdit(this);
    CPP_tag = new QLabel(this);
    H_tag = new QLabel(this);
    CPP_tag->setText(".cpp");   // initial text
    H_tag->setText(".h");       // initial text
    C_CPP_CLASS_layout->addRow("Class name: ", ClassName_input);
    C_CPP_CLASS_layout->addRow(CPP_tag);
    C_CPP_CLASS_layout->addRow(H_tag);

    connect(ClassName_input, SIGNAL(textChanged(const QString&)), this, SLOT(slotC_CPP_CLASS_LABEL_UPDATE()));
    setFixedSize(250, 100);
    setWindowFlags(Qt::Dialog);;
    setWindowTitle("New Class: ");
    setCursor(cursor());  // set into middle of app view
    setLayout(C_CPP_CLASS_layout);
    show();

    connect(ClassName_input, SIGNAL(returnPressed()), this, SLOT(slotC_CPP_CLASS()));
    setLayout(C_CPP_CLASS_layout);
}

void NewWindow::slotC_CPP_CLASS_LABEL_UPDATE(){
    const QString temp = ClassName_input->text();
    CPP_tag->setText(temp + ".cpp");
    H_tag->setText(temp + ".h");
}

void NewWindow::slotC_CPP_CLASS() {
    new_cpp = ClassName_input->text() + ".cpp";
    new_h = ClassName_input->text() + ".h";
    emit PartialWidgetDestroyed();
    close();
}

void NewWindow::rename_Window(const QString &old) {
    rename_layout = new QFormLayout();
    rename_input = new QLineEdit(this);
    old_tag = new QLabel(this);
    old_tag->setText(old);
    rename_layout->addRow("Old name: ", old_tag);
    rename_layout->addRow("New name: ", rename_input);

    setFixedSize(250, 100);
    setWindowFlags(Qt::Dialog);
    setWindowTitle("New Name: ");
    setCursor(cursor());  // set into middle of app view
    setLayout(rename_layout);
    show();

    connect(rename_input, SIGNAL(returnPressed()), this, SLOT(slotRename()));
    setLayout(rename_layout);
}

void NewWindow::slotRename() {
    new_name = rename_input->text();
    emit PartialWidgetDestroyed();
    close();
}


QString NewWindow::getDir() const{
    return new_dirname;
}

QString NewWindow::getCPP() const {
    return new_cpp;
}

QString NewWindow::getH() const {
    return new_h;
}

QString NewWindow::getNewName() const {
    return new_name;
}

QString NewWindow::getName() const {
    return new_file;
}




