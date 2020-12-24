#ifndef CONVERTER_H
#define CONVERTER_H

/*
    small widget to convert between hex, binary, base64, etc.
*/

#include <QWidget>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class Converter : public QWidget
{
    Q_OBJECT
public:
    explicit Converter(QWidget *parent = nullptr);

private:
    QComboBox *Option;
    QPlainTextEdit  *InputWindow;
    QPlainTextEdit  *OutputWindow;
    QLabel *Caption;
    QPushButton *convert_button;

    QLayout *BuildForm();
    void runConverter();

private slots:
    void slotConvert();


signals:

};

#endif // CONVERTER_H
