#include "NewWindow.h"


NewWindow::NewWindow(QWidget *parent) : QWidget(parent){
    dir_Window();
    //CppOrH_Window();
    //CppAndH_Window();
    setMaximumSize(300, 200);
}

void NewWindow::dir_Window() {
    dir_layout = new QHBoxLayout();
    dir_tag = new QLabel();
    dir_input = new QLineEdit();

    dir_tag->setText("directory: ");

    dir_layout->addWidget(dir_tag);
    dir_layout->addWidget(dir_input);

    connect(dir_input, SIGNAL(returnPressed()), this, SLOT(slotDir()));

    setLayout(dir_layout);
}

void NewWindow::CppOrH_Window() {
    CppOrH_layout = new QHBoxLayout();
    fileName_tag = new QLabel();
    fileName_input = new QLineEdit();

    fileName_tag->setText("File name: ");

    CppOrH_layout->addWidget(fileName_tag);
    CppOrH_layout->addWidget(fileName_input);

    connect(fileName_input, SIGNAL(returnPressed()), this, SLOT(slotCppOrH()));

    setLayout(CppOrH_layout);
}

void NewWindow::CppAndH_Window() {
    CppAndH_layout = new QHBoxLayout();
    FileName_tag = new QLabel();
    FileName_input = new QLineEdit();

    FileName_tag->setText("File name: ");

    CppAndH_layout->addWidget(FileName_tag);
    CppAndH_layout->addWidget(FileName_input);

    connect(FileName_input, SIGNAL(returnPressed()), this, SLOT(slotCppAndH()));

    setLayout(CppAndH_layout);
}


void NewWindow::slotDir() {
    new_dirname = dir_input->text();
}

void NewWindow::slotCppOrH() {
    new_cppname = fileName_input->text() + ".cpp";
    new_hname = fileName_input->text() + ".h";
}

void NewWindow::slotCppAndH() {
    new_cppname = FileName_input->text() + ".cpp";
    new_hname = FileName_input->text() + ".h";
}


