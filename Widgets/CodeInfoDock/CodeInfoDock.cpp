// have to be there, parsing error with some .inc files, edit was also not found
#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "icons/IconFactory.h"
#include "Clang/ClangBridge.h"
#include <QToolBar>

#include "CodeInfoDock.h"

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
    setMinimumHeight(150);
    MainWidget->setLayout(MainLayout);

    setWidget(MainWidget);
}

void CodeInfoDock::createWindow() {
    MainLayout = new QHBoxLayout();// this,  to be set for dock
    SignatureActionLayout = new QVBoxLayout();
    CompleterLinterLayout = new QVBoxLayout();

    signature = new QLabel(this);
    documentation = new QLabel(this);
    calls = new QListWidget(this);
    actions = new QListWidget(this);
    TitleBar = new QToolBar(this);
    classSignature = new QLabel("class signature", this);
    functionSignature = new QLabel("function signature", this);

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
    SignatureActionLayout->addWidget(calls);
    SignatureActionLayout->addWidget(actions);

    completer = new QListWidget(this);
    linter = new QListWidget(this);
    CompleterLinterLayout->addWidget(completer);
    CompleterLinterLayout->addWidget(linter);

    MainLayout->addLayout(SignatureActionLayout);
    MainLayout->addLayout(CompleterLinterLayout);
    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);
}


void CodeInfoDock::connectThreads() {
    Handler = new ClangHandler(clang, this);

    signatureThread = new QThread(this);
    connect(signatureThread, &QThread::started, Handler, &ClangHandler::slotGetSignature);
    connect(signatureThread, &QThread::finished, signatureThread, &QObject::deleteLater);
    connect(Handler, SIGNAL(addSignature()), this, SLOT(slotGetSignature()));
    Handler->moveToThread(signatureThread);

    codeCompleteThread = new QThread(this);
    connect(codeCompleteThread, &QThread::started, Handler, &ClangHandler::slotCodeComplete);
    connect(codeCompleteThread, &QThread::finished, codeCompleteThread, &QObject::deleteLater);
    connect(Handler, SIGNAL(addCodeComplete()), this, SLOT(slotCodeComplete()));
    Handler->moveToThread(codeCompleteThread);

    renameThread = new QThread(this);
    connect(renameThread, &QThread::started, Handler, &ClangHandler::slotRename);
    connect(renameThread, &QThread::finished, renameThread, &QObject::deleteLater);
    connect(Handler, SIGNAL(renameFinished()), this, SLOT(slotRename()));
    Handler->moveToThread(renameThread);

    codeCheckThread = new QThread(this);
    connect(codeCheckThread, &QThread::started, Handler, &ClangHandler::slotCodeCheck);
    connect(codeCheckThread, &QThread::finished, codeCheckThread, &QObject::deleteLater);
    connect(Handler, SIGNAL(codeCheckFinished()), this, SLOT(slotCodeCheck()));
    Handler->moveToThread(codeCheckThread);

    clangTidyCheckThread = new QThread(this);
    connect(clangTidyCheckThread, &QThread::started, Handler, &ClangHandler::slotClangTidyCheck);
    connect(clangTidyCheckThread, &QThread::finished, clangTidyCheckThread, &QObject::deleteLater);
    connect(Handler, SIGNAL(clangTidyFinished()), this, SLOT(slotClangTidyCheck()));
    Handler->moveToThread(clangTidyCheckThread);

    findReferencesThread = new QThread(this);
    connect(findReferencesThread, &QThread::started, Handler, &ClangHandler::slotFindReferences);
    connect(findReferencesThread, &QThread::finished, findReferencesThread, &QObject::deleteLater);
    connect(Handler, SIGNAL(addReferences()), this, SLOT(slotFindReferences()));
    Handler->moveToThread(findReferencesThread);

    formatFileThread = new QThread(this);
    connect(formatFileThread, &QThread::started, Handler, &ClangHandler::slotFormatFile);
    connect(formatFileThread, &QThread::finished, formatFileThread, &QObject::deleteLater);
    connect(Handler, SIGNAL(formatFileFinished()), this, SLOT(slotFormatFile()));
    Handler->moveToThread(formatFileThread);

    goToDefinitionThread = new QThread(this);
    connect(goToDefinitionThread, &QThread::started, Handler, &ClangHandler::slotGoToDefinition);
    connect(goToDefinitionThread, &QThread::finished, goToDefinitionThread, &QObject::deleteLater);
    connect(Handler, SIGNAL(goToDefinitionFinished()), this, SLOT(slotGoToDefinition()));
    Handler->moveToThread(goToDefinitionThread);

    generateThread = new QThread(this);
    connect(generateThread, &QThread::started, Handler, &ClangHandler::slotGenerate);
    connect(generateThread, &QThread::finished, generateThread, &QObject::deleteLater);
    connect(Handler, SIGNAL(generateFinished()), this, SLOT(slotGenerate()));
    Handler->moveToThread(generateThread);
}


void CodeInfoDock::RunAction(const CodeInfoDock::Action &action) {
    switch (action) {
        case GetSignature:
            if (signatureThread->isRunning()) {
                return;
            }
            signatureThread->start();
            break;
        case CodeComplete:
            if (codeCompleteThread->isRunning()) {
                return;
            }
            codeCompleteThread->start();
            break;
        case Rename:
            if (renameThread->isRunning()) {
                return;
            }
            renameThread->start();
            break;
        case CodeCheck:
            if (codeCheckThread->isRunning()) {
                return;
            }
            codeCheckThread->start();
            break;
        case ClangTidyCheck:
            if (clangTidyCheckThread->isRunning()) {
                return;
            }
            clangTidyCheckThread->start();
            break;
        case FindReferences:
            if (findReferencesThread->isRunning()) {
                return;
            }
            findReferencesThread->start();
            break;
        case FormatFile:
            if (formatFileThread->isRunning()) {
                return;
            }
            formatFileThread->start();
            break;
        case GoToDefinition:
            if (goToDefinitionThread->isRunning()) {
                return;
            }
            goToDefinitionThread->start();
            break;
        case Generate:
            if (generateThread->isRunning()) {
                return;
            }
            generateThread->start();
            break;

        default:
            break;
    }
}


void CodeInfoDock::slotGetSignature() {
    signature->setText(QString::fromStdString(Handler->Signature));
    documentation->setText(QString::fromStdString(Handler->Documentation));
}
void CodeInfoDock::slotCodeComplete() {
    //edit->setCompletionData(Handler->CompletionData);
}
void CodeInfoDock::slotRename() {
}
void CodeInfoDock::slotCodeCheck() {
}
void CodeInfoDock::slotClangTidyCheck() {
}
void CodeInfoDock::slotFindReferences() {
}
void CodeInfoDock::slotFormatFile() {
}
void CodeInfoDock::slotGoToDefinition() {
}
void CodeInfoDock::slotGenerate() {
}
