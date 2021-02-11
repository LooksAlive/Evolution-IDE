#ifndef EVOLUTION_IDE_CODEINFODOCK_H
#define EVOLUTION_IDE_CODEINFODOCK_H

/*
 * first(vertical) is signature label for word text cursor in on
 * some include hierarchy map and usages within a list to click and move there
 * + added some shortcuts as tooltip for user
 
 * second(vertical) is suppposed action to perform with expression like:
 * generate something, but aslo go to definition + code snippets

 * third (horizontal, next to vertical) :: sometimes called functions are named 
 * differently for ex. (set, add), (append, insert), (delete, remove) ... 
 * this window will be wider completer view for names

 * ++ some timer to at least seconds for now 

*/

#include <QDockWidget>
#include <QWidget>
#include <QLabel>
#include <QListWidget>


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
    
    QHBoxLayout *MainLayout;
    QVBoxLayout *SignatureActionLayout;
    
    QLabel *signature;
    // represents where function, var, instance is called
    QListWidget *calls;
    // double click, consider TreeView, since there could be more actions available
    QListWidget *actions;
    
    QListWidget *completer;

    void createWindow();


private slots:

};


#endif //EVOLUTION_IDE_CODEINFODOCK_H
