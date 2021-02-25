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
#include <QLabel>
#include <QListWidget>
#include <QWidget>


#include <QFormLayout>
#include <QHBoxLayout>
#include <QObject>
#include <QThread>
#include <QToolBar>
#include <QVBoxLayout>

#include <iostream>
#include <string>

class ClangBridge;
class PlainTextEdit;


// FIXME: will this event work ? do i have to create separate objects for every thread ???

class ClangHandler : public QObject {
    Q_OBJECT

public:
    explicit ClangHandler(ClangBridge *bridge, QObject *parent = nullptr);
    ~ClangHandler() = default;

    // nothing is required to be constantly updated, when the result appears, take them by signals, no hurry :)

    // what symbol should we try to find signature for
    std::string RequestedSymbol;

    std::string Signature;
    std::string Documentation;

    // refactoring / renaming
    std::string OldSymbol;
    std::string NewSymbol;

    // file format -> pointer to QTextDocument
    QTextDocument *document;

    // checks and tidy + fix it;  they would need to have connection here, since we operate with clang
    // and its related tools

    // definition request data
    struct DefinitionLoc {
        std::string filepath;
        int row;
        int col;
    };
    DefinitionLoc definition;

    // what can ge generate


private:
    ClangBridge *clangBridge;

public slots:
    void slotGetSignature();
    void slotCodeComplete();
    void slotRename();
    void slotCodeCheck();
    void slotClangTidyCheck();
    void slotFindReferences();
    void slotFormatFile();
    void slotGoToDefinition();
    void slotGenerate();

signals:
    void addSignature();
    void addCodeComplete();
    void renameFinished();
    void codeCheckFinished();
    void clangTidyFinished();
    void addReferences();
    void formatFileFinished();
    void goToDefinitionFinished();
    void generateFinished();
};


class CodeInfoDock : public QDockWidget {
    Q_OBJECT
public:
    explicit CodeInfoDock(QWidget *parent = nullptr);
    ~CodeInfoDock() = default;

    void setEditor(PlainTextEdit *editor) { edit = editor; }
    void setClang(ClangBridge *clang_bridge) { clang = clang_bridge; }

    ClangHandler *Handler;

    enum Action {
        GetSignature = 0,
        CodeComplete,
        Rename,
        CodeCheck,
        ClangTidyCheck,
        FindReferences,
        FormatFile,
        GoToDefinition,
        Generate// what could be generated will appear in widget list
    };

    void RunAction(const Action &action);


private:
    // generates, fixits, moving between errors requires instance as well as
    // edit requires this dock for data insertions
    PlainTextEdit *edit;
    // parsing
    ClangBridge *clang;

    QWidget *MainWidget;
    QToolBar *TitleBar;
    QLabel *classSignature;
    QLabel *functionSignature;

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

    QThread *signatureThread;
    QThread *codeCompleteThread;
    QThread *renameThread;
    QThread *codeCheckThread;
    QThread *clangTidyCheckThread;
    QThread *findReferencesThread;
    QThread *formatFileThread;
    QThread *goToDefinitionThread;
    QThread *generateThread;


    void connectThreads();

    // these will update widgets
private slots:
    void slotGetSignature();
    void slotCodeComplete();// in editor completer + kinda ours
    // necessary data would be grabbed from dock widgets or set directly into Handler
    void slotRename();
    // TODO: this two might directly set selection(warnings, errors)
    void slotCodeCheck();
    void slotClangTidyCheck();
    // this shall fill only dock
    void slotFindReferences();
    void slotFormatFile();
    void slotGoToDefinition();
    void slotGenerate();
};


#endif //EVOLUTION_IDE_CODEINFODOCK_H
