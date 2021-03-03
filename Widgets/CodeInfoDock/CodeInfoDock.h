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
#include <QWidget>


#include <QFormLayout>
#include <QHBoxLayout>
#include <QObject>
#include <QTextDocument>
#include <QThread>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

#include <iostream>
#include <string>

#include "LinterDock.h"
#include "RefactoringDock.h"

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
    struct LinterLocation {
        std::string filepath;
        int row;
        int col;
    };
    std::vector<LinterLocation> linterLocations;

    // definition request data
    struct DefinitionLoc {
        std::string filepath;
        int row;
        int col;
    };
    DefinitionLoc definition;

    // what can ge generate

    // references
    struct ReferencesLocation {
        std::string filepath;
        int row;
        int col;
    };

    std::vector<ReferencesLocation> referencesLocation;

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

class FindReplaceWidget;

/*
 * this dock shows code info and also manages 
*/

class CodeInfoDock : public QDockWidget {
    Q_OBJECT
public:
    explicit CodeInfoDock(QWidget *parent = nullptr);
    ~CodeInfoDock();

    // outside in MainWindow because it suppose to be a dock
    void setEditor(PlainTextEdit *editor) { edit = editor; }
    void setClang(ClangBridge *clang_bridge) { clang = clang_bridge; }
    // for references using same dock as for search with a couple of changes
    void setSearch(FindReplaceWidget *ref) { References = ref; }
    void setLinter(LinterDock *linter) { Linter = linter; }
    void setRefactor(RefactoringDock *refactor) { Refactor = refactor; }

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

    void runAction(const Action &action);
    // updated widgets according to editor cursor
    void updateDocks();

    QToolButton *showReferences;
    // represents where function, var, instance is called


    // generates, fixits, moving between errors requires instance as well as
    // edit requires this dock for data insertions
    PlainTextEdit *edit;
    // parsing
    ClangBridge *clang;
    // references
    FindReplaceWidget *References;
    // code check, clang-tidy
    LinterDock *Linter;
    // refactoring
    RefactoringDock *Refactor;

private:
    QToolBar *ToolBar;
    QWidget *MainWidget;
    QToolBar *TitleBar;

    QLabel *classSignature;
    QLabel *functionSignature;
    // list of different parameters (polymorphism)
    //QLabel *PolymorphicSignatures;
    //QLabel *classHierarchy;
    //QLabel *includeHierarchy;

    QHBoxLayout *MainLayout;
    QVBoxLayout *SignatureActionLayout;

    QLabel *signature;
    QLabel *documentation;
    // double click, consider TreeView, since there could be more actions available

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
    void connectDocks();

    // these will update widgets
private slots:

    void slotShowReferencesDock();
    void slotShowRefactorDock();
    void slotShowLinterDock();

    void slotOpenFileReference(const QModelIndex &);

    // action ended related slots, gather data, update GUI + proper actions
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
