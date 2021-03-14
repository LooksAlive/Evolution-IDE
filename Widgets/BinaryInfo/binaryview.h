#ifndef BINARYINFO_H
#define BINARYINFO_H

#include <QWidget>
//#include <QVBoxLayout>

class BinaryView : public QWidget {
Q_OBJECT
public:
    explicit BinaryView(QWidget *parent = nullptr);

    ~BinaryView() = default;   // change later



private:


    void createWindow();


private slots:


signals:

};

#endif // BINARYINFO_H
