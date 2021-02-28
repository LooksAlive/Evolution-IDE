// have to be there, parsing error with some .inc files, edit was also not found
#include "CodeInfoDock.h"
#include "Widgets/FindReplace/findreplace.h"
#include "Widgets/PlainTextEdit/plaintextedit.h"// these 2 have to be there, in header raises error ref.
#include "icons/IconFactory.h"
#include "Clang/ClangBridge.h"

ClangHandler::ClangHandler(ClangBridge *bridge, QObject *parent) : QObject(parent), clangBridge(bridge) {
}

void ClangHandler::slotGetSignature() {
}
void ClangHandler::slotCodeComplete() {
}
void ClangHandler::slotRename() {
}
void ClangHandler::slotCodeCheck() {
}
void ClangHandler::slotClangTidyCheck() {
}
void ClangHandler::slotFindReferences() {
}
void ClangHandler::slotFormatFile() {
}
void ClangHandler::slotGoToDefinition() {
}
void ClangHandler::slotGenerate() {
}


CodeInfoDock::CodeInfoDock(QWidget *parent) : QDockWidget(parent) {
    setWindowTitle("Code Informations");
    setFeatures(AllDockWidgetFeatures);
    setVisible(false);
    createWindow();

    MainWidget = new QWidget(this);
    setMinimumHeight(50);
    MainWidget->setLayout(MainLayout);

    setWidget(MainWidget);
}

void CodeInfoDock::createWindow() {
    MainLayout = new QHBoxLayout();// this,  to be set for dock
    SignatureActionLayout = new QVBoxLayout();

    signature = new QLabel(this);
    documentation = new QLabel(this);
    TitleBar = new QToolBar(this);
    classSignature = new QLabel("class signature", this);
    functionSignature = new QLabel("function signature", this);
    ToolBar = new QToolBar(this);
    showReferences = new QToolButton(this);

    ToolBar->setOrientation(Qt::Vertical);
    ToolBar->setFixedWidth(30);
    showReferences = new QToolButton(this);
    showReferences->setToolTip("Show References");
    showReferences->setIcon(QIcon(IconFactory::Expand));
    auto *showRefactor = new QToolButton(this);
    showRefactor->setToolTip("Show Refactor");
    auto *showLinter = new QToolButton(this);
    showLinter->setToolTip("Show Linter");

    //connect(references, SIGNAL(itemDoubleClicked(const QModelIndex&)), this, SLOT(slotOpenFileReference(const QModelIndex&)));
    connect(showReferences, SIGNAL(clicked()), this, SLOT(slotShowReferencesDock()));
    connect(showRefactor, SIGNAL(clicked()), this, SLOT(slotShowRefactorDock()));
    connect(showLinter, SIGNAL(clicked()), this, SLOT(slotShowLinterDock()));

    ToolBar->addWidget(showReferences);
    ToolBar->addWidget(showRefactor);
    ToolBar->addWidget(showLinter);

    auto *spacer = new QWidget(this);// align to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    TitleBar->addWidget(classSignature);
    TitleBar->addSeparator();
    TitleBar->addSeparator();
    TitleBar->addSeparator();
    TitleBar->addSeparator();
    TitleBar->addSeparator();
    TitleBar->addWidget(functionSignature);
    TitleBar->addWidget(spacer);
    TitleBar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));
    TitleBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    TitleBar->setFixedHeight(30);
    //t_bar->setIconSize(QSize(25, 35));
    setTitleBarWidget(TitleBar);
    SignatureActionLayout->addWidget(signature);
    SignatureActionLayout->addWidget(documentation);

    MainLayout->addWidget(ToolBar);
    MainLayout->addLayout(SignatureActionLayout);
    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);

    //connectDocks();
}


void CodeInfoDock::connectThreads() {
    Handler = new ClangHandler(clang, this);

    // TODO: replace finished with our slots instead of creating signals etc.

    signatureThread = new QThread(this);
    connect(signatureThread, &QThread::started, Handler, &ClangHandler::slotGetSignature);
    connect(signatureThread, SIGNAL(finished()), this, SLOT(slotGetSignature()));
    //connect(Handler, SIGNAL(addSignature()), this, SLOT(slotGetSignature()));

    codeCompleteThread = new QThread(this);
    connect(codeCompleteThread, &QThread::started, Handler, &ClangHandler::slotCodeComplete);
    connect(codeCompleteThread, SIGNAL(finished()), this, SLOT(slotCodeComplete()));
    //connect(Handler, SIGNAL(addCodeComplete()), this, SLOT(slotCodeComplete()));

    renameThread = new QThread(this);
    connect(renameThread, &QThread::started, Handler, &ClangHandler::slotRename);
    connect(renameThread, SIGNAL(finished()), this, SLOT(slotRename()));
    //connect(Handler, SIGNAL(renameFinished()), this, SLOT(slotRename()));

    codeCheckThread = new QThread(this);
    connect(codeCheckThread, &QThread::started, Handler, &ClangHandler::slotCodeCheck);
    connect(codeCheckThread, SIGNAL(finished()), this, SLOT(slotCodeCheck()));
    //connect(Handler, SIGNAL(codeCheckFinished()), this, SLOT(slotCodeCheck()));

    clangTidyCheckThread = new QThread(this);
    connect(clangTidyCheckThread, &QThread::started, Handler, &ClangHandler::slotClangTidyCheck);
    connect(clangTidyCheckThread, SIGNAL(finished()), this, SLOT(slotClangTidyCheck()));
    //connect(Handler, SIGNAL(clangTidyFinished()), this, SLOT(slotClangTidyCheck()));

    findReferencesThread = new QThread(this);
    connect(findReferencesThread, &QThread::started, Handler, &ClangHandler::slotFindReferences);
    connect(findReferencesThread, SIGNAL(finished()), this, SLOT(slotFindReferences()));
    //connect(Handler, SIGNAL(addReferences()), this, SLOT(slotFindReferences()));

    formatFileThread = new QThread(this);
    connect(formatFileThread, &QThread::started, Handler, &ClangHandler::slotFormatFile);
    connect(formatFileThread, SIGNAL(finished()), this, SLOT(slotFormatFile()));
    //connect(Handler, SIGNAL(formatFileFinished()), this, SLOT(slotFormatFile()));

    goToDefinitionThread = new QThread(this);
    connect(goToDefinitionThread, &QThread::started, Handler, &ClangHandler::slotGoToDefinition);
    connect(goToDefinitionThread, SIGNAL(finished()), this, SLOT(slotGoToDefinition()));
    //connect(Handler, SIGNAL(goToDefinitionFinished()), this, SLOT(slotGoToDefinition()));

    generateThread = new QThread(this);
    connect(generateThread, &QThread::started, Handler, &ClangHandler::slotGenerate);
    connect(generateThread, SIGNAL(finished()), this, SLOT(slotGenerate()));
    //connect(Handler, SIGNAL(generateFinished()), this, SLOT(slotGenerate()));
}

void CodeInfoDock::connectDocks() {
    // Refactor
    connect(Refactor->Original, SIGNAL(clicked()), this, SLOT(slotRename()));// nullptr !!!!
}

void CodeInfoDock::runAction(const CodeInfoDock::Action &action) {
    switch (action) {
        case GetSignature:
            if (signatureThread->isRunning()) {
                return;
            }
            Handler->moveToThread(signatureThread);
            signatureThread->start();
            break;
        case CodeComplete:
            if (codeCompleteThread->isRunning()) {
                return;
            }
            Handler->moveToThread(codeCompleteThread);
            codeCompleteThread->start();
            break;
        case Rename:
            if (renameThread->isRunning()) {
                return;
            }
            Handler->moveToThread(renameThread);
            renameThread->start();
            break;
        case CodeCheck:
            if (codeCheckThread->isRunning()) {
                return;
            }
            Handler->moveToThread(codeCheckThread);
            codeCheckThread->start();
            break;
        case ClangTidyCheck:
            if (clangTidyCheckThread->isRunning()) {
                return;
            }
            Handler->moveToThread(clangTidyCheckThread);
            clangTidyCheckThread->start();
            break;
        case FindReferences:
            if (findReferencesThread->isRunning()) {
                return;
            }
            Handler->moveToThread(findReferencesThread);
            findReferencesThread->start();
            break;
        case FormatFile:
            if (formatFileThread->isRunning()) {
                return;
            }
            Handler->moveToThread(formatFileThread);
            formatFileThread->start();
            break;
        case GoToDefinition:
            if (goToDefinitionThread->isRunning()) {
                return;
            }
            Handler->moveToThread(goToDefinitionThread);
            goToDefinitionThread->start();
            break;
        case Generate:
            if (generateThread->isRunning()) {
                return;
            }
            Handler->moveToThread(generateThread);
            generateThread->start();
            break;

        default:
            break;
    }
}

void CodeInfoDock::updateDocks() {
    Linter->filePath->setText(edit->getFilePath());

    Refactor->Original->setText(edit->getWordUnderCursor());
}

void CodeInfoDock::slotOpenFileReference(const QModelIndex &) {
    // TODO: access Handler references data + also check if somewhere path of edit is matching to avoid reading files
    /*
    if(edit->getFilePath() == somefpth){
        // get position and set
        // TODO main Tab access required, append tab, do not change current edit
    }
    for (int i = 0; i <= references.size(); i++) {

    }
    */
}

void CodeInfoDock::slotShowReferencesDock() {
    References->setVisible(true);
    References->setFocus();
    References->raise();
    References->titleBarWidget()->setEnabled(false);
    // consider replacing or disabling title bar which is meant to be for search

    // fill view list ++ only file names; tool tip for item will be full path
    References->results->reset();

    //find_replace->results->rootNode;
    //find_replace->results->model;
    /*
    for (int i = 0; i <= references.size(); i++) {

    }
    */
}

/*
 * do not delete threads, we will use them again
*/

void CodeInfoDock::slotShowRefactorDock() {
    Refactor->setVisible(true);
    Refactor->setFocus();
    Refactor->raise();
}

void CodeInfoDock::slotShowLinterDock() {
    Linter->setVisible(true);
    Linter->setFocus();
    Linter->raise();
}

void CodeInfoDock::slotGetSignature() {
    //signatureThread->quit();
    signatureThread->terminate();
    signatureThread->wait();
    signature->setText(QString::fromStdString(Handler->Signature));
    documentation->setText(QString::fromStdString(Handler->Documentation));
}
void CodeInfoDock::slotCodeComplete() {
    //codeCompleteThread->quit();
    codeCompleteThread->terminate();
    codeCompleteThread->wait();
    //edit->setCompletionData(Handler->CompletionData);
}
void CodeInfoDock::slotRename() {
    //renameThread->quit();
    renameThread->terminate();
    renameThread->wait();
}
void CodeInfoDock::slotCodeCheck() {
    //codeCheckThread->quit();
    codeCheckThread->terminate();
    codeCheckThread->wait();
    Linter->addItem("item from result data", /*proper color*/ LinterDock::Warning);
}
void CodeInfoDock::slotClangTidyCheck() {
    //clangTidyCheckThread->quit();
    clangTidyCheckThread->terminate();
    clangTidyCheckThread->wait();
}
void CodeInfoDock::slotFindReferences() {
    //findReferencesThread->quit();
    findReferencesThread->terminate();
    findReferencesThread->wait();
}
void CodeInfoDock::slotFormatFile() {
    //formatFileThread->quit();
    formatFileThread->terminate();
    formatFileThread->wait();
}
void CodeInfoDock::slotGoToDefinition() {
    //goToDefinitionThread->quit();
    goToDefinitionThread->terminate();
    goToDefinitionThread->wait();
}
void CodeInfoDock::slotGenerate() {
    //generateThread->quit();
    generateThread->terminate();
    generateThread->wait();
}
