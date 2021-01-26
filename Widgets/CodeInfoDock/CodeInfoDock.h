#ifndef EVOLUTION_IDE_CODEINFODOCK_H
#define EVOLUTION_IDE_CODEINFODOCK_H

#include <QDockWidget>
#include <QWidget>

class CodeInfoDock : public QDockWidget{
    Q_OBJECT

public:
    explicit CodeInfoDock(QWidget *parent = nullptr);
    ~CodeInfoDock() = default;

    void buildCodeInfo();

private:
    QWidget *MainWidget;


private slots:

};


#endif //EVOLUTION_IDE_CODEINFODOCK_H
