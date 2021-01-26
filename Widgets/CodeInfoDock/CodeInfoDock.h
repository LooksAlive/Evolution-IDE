#ifndef EVOLUTION_IDE_CODEINFODOCK_H
#define EVOLUTION_IDE_CODEINFODOCK_H

#include <QDockWidget>
#include <QWidget>
#include <QFormLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

class CodeInfoDock : public QDockWidget{
    Q_OBJECT

public:
    explicit CodeInfoDock(QWidget *parent = nullptr);
    ~CodeInfoDock() = default;


private:
    QWidget *MainWidget;

    void createWindow();


private slots:

};


#endif //EVOLUTION_IDE_CODEINFODOCK_H
