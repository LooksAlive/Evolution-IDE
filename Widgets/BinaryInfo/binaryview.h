#ifndef BINARYINFO_H
#define BINARYINFO_H

#include <QWidget>

class BinaryView : public QWidget
{
    Q_OBJECT
public:
    explicit BinaryView(QWidget *parent = nullptr);
    ~BinaryView() = default;   // change later

signals:

};

#endif // BINARYINFO_H
