#ifndef CONVERTER_H
#define CONVERTER_H

/*
    small widget to convert between hex, binary, base64, etc.
*/

#include <QWidget>

class Converter : public QWidget
{
    Q_OBJECT
public:
    explicit Converter(QWidget *parent = nullptr);

signals:

};

#endif // CONVERTER_H
