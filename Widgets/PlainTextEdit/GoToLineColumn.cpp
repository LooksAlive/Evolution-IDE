#include "GoToLineColumn.h"

GoToLineColumn::GoToLineColumn(PlainTextEdit *textEdit, QWidget *parent) : QWidget(parent), edit(textEdit){
    setWindowFlags(Qt::Dialog);
    createWindow();
}


void GoToLineColumn::createWindow() {
    MainLayout = new QFormLayout();
    line_input = new QLineEdit(this);
    line_input->setText("1");   // get current pos here by constructor
    MainLayout->addRow("Line  ", line_input);
    connect(line_input, SIGNAL(returnPressed()), this, SLOT(storeData()));
    setLayout(MainLayout);
}

void GoToLineColumn::storeData() {
    int line = line_input->text().toInt();
    edit->setCursorAtLine(line);
    close();
    deleteLater();
}





