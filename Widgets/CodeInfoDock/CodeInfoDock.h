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
 * this window will be wider completer view for names +++++
 * also second part will be clang-tidy and official linter list

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


class ClangBridge;
class PlainTextEdit;

class CodeInfoDock : public QDockWidget{
    Q_OBJECT

public:
    explicit CodeInfoDock(QWidget *parent = nullptr);
    ~CodeInfoDock() = default;
    
    void setEditor(PlainTextEdit *editor){edit = editor;}
    void setClang(ClangBridge *clang_bridge){clang = clang_bridge;}


private:
    // generates, fixits, moving between errors requires instance as well as
    // edit requires this dock for data insertions
    PlainTextEdit *edit;
    // parsing
    ClangBridge *clang;

    QWidget *MainWidget;

    QHBoxLayout *MainLayout;
    QVBoxLayout *SignatureActionLayout;
    QVBoxLayout *CompleterLinterLayout;

    QLabel *signature;
    QLabel *documentation;
    // represents where function, var, instance is called
    QListWidget *calls;
    // double click, consider TreeView, since there could be more actions available
    QListWidget *actions;

    QListWidget *completer;
    QListWidget *linter;

    void createWindow();


private slots:

};


#endif //EVOLUTION_IDE_CODEINFODOCK_H
