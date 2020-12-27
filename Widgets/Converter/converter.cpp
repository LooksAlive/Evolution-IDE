#include "converter.h"

Converter::Converter(QWidget *parent) : QWidget(parent)
{
    setLayout(BuildForm());
    setMinimumSize(300, 300);

    connect(convert_button, &QPushButton::clicked, this, &Converter::slotConvert);
}

Converter::~Converter(){
    delete Option;
    delete InputWindow;
    delete OutputWindow;
    delete Caption;
    delete convert_button;
    // delete BuildForm();  // ???
}

QLayout *Converter::BuildForm(){

    QVBoxLayout *layout = new QVBoxLayout();
    Option = new QComboBox();
    InputWindow = new QPlainTextEdit();
    OutputWindow = new QPlainTextEdit();
    Caption = new QLabel();
    convert_button = new QPushButton();

    Option->addItem("Text");
    Option->addItem("Hex");
    Option->addItem("Base64");

    Option->addItem("Encode Sha256");
    Option->addItem("Decode Sha256");

    Caption->setText("Convert & Encode & Decode");
    convert_button->setText("Convert");

    layout->addWidget(Caption);
    layout->addWidget(InputWindow);
    layout->addWidget(Option);
    layout->addWidget(OutputWindow);
    layout->addWidget(convert_button);

    return layout;
}

void Converter::runConverter(){
    QString input = InputWindow->toPlainText();
    QString output = "Not able to convert :(";
    // determine option in deep
    QString opt = Option->currentText();
    if(opt == "Text"){
        OutputWindow->setPlainText(input); // for now one
    }
    if(opt == "Hex"){
        output = input.toLatin1().toHex();
        OutputWindow->setPlainText(output);
        /*
        QByteArray temp = QByteArray::fromHex(input.toLatin1());
        output = QString(temp);
        OutputWindow->setPlainText(output);
        */
    }
    if(opt == "Base64"){
        output = input.toLatin1().toBase64();
        OutputWindow->setPlainText(output);
        /*
        QByteArray temp = QByteArray::fromBase64(input.toLatin1());
        output = QString(temp);
        OutputWindow->setPlainText(output);
        */
    }
    /*
    if(opt == "Encode Sha256"){

    }
    if(opt == "ecode Sha256"){

    }
    */
}


void Converter::slotConvert(){
    runConverter();
}
