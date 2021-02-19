#include "NewSampleWindow.h"

NewSampleWindow::NewSampleWindow(QWidget *parent) : QWidget(parent) {}


void NewSampleWindow::newUserSampleWindow() {
    auto *layout = new QFormLayout();
    sampleName_input = new QLineEdit(this);
    sampleName_input->setPlaceholderText("Sample Name");
    layout->addRow(sampleName_input);

    setFixedSize(250, 50);
    setWindowFlags(Qt::Dialog);
    setWindowTitle("New sample");
    setCursor(cursor());// set into middle of app view
    setLayout(layout);
    show();

    connect(sampleName_input, SIGNAL(returnPressed()), this, SLOT(slotSampleNameRegister()));
}

void NewSampleWindow::slotSampleNameRegister() {
    newSampleName = sampleName_input->text();
    emit sampleNameRegisterDestroyed();
    close();
}