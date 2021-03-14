// have to be there, parsing error with some .inc files, edit was also not found
#include "CodeInfoDock.h"
#include "Widgets/FindReplace/findreplace.h"
#include "Widgets/PlainTextEdit/plaintextedit.h"// these 2 have to be there, in header raises error ref.
#include "icons/IconFactory.h"
#include "Clang/ClangBridge.h"

ClangHandler::ClangHandler(ClangBridge *bridge, QObject *parent) : QObject(parent), clangBridge(bridge) {
}

void ClangHandler::slotGetSignature() {
    //clangBridge->signatureHelp(currentFile, position);
}
void ClangHandler::slotCodeComplete() {
    //clangBridge->codeComplete(currentFile, position, clangBridge->codeCompleteOptions);
}

void ClangHandler::slotRename() {
    //clangBridge->rename(currentFile, position, NewSymbol, clangBridge->renameOptions);
}

void ClangHandler::slotUpdatePreamble() {
    //clangBridge->updatePreamble();
}

void ClangHandler::slotFindReferences() {
    //clangBridge->findReferences(currentFile, position, 30); // limit
}

void ClangHandler::slotFormatFile() {
    //clangBridge->formatCode(currentFile, *content, {tooling::Range(0, (*content).size())}); // whole file
}

void ClangHandler::slotGoToDefinition() {
    //clangBridge->locateSymbolAt(currentFile, position);
}

void ClangHandler::slotGenerate() {
}

void ClangHandler::slotGetHighlights() {
    //clangBridge->semanticHighlights(currentFile);
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

CodeInfoDock::~CodeInfoDock() {
    /*
    signatureThread->deleteLater();
    codeCompleteThread->deleteLater();
    renameThread->deleteLater();
    codeCheckThread->deleteLater();
    clangTidyCheckThread->deleteLater();
    findReferencesThread->deleteLater();
    formatFileThread->deleteLater();
    goToDefinitionThread->deleteLater();
    generateThread->deleteLater();
    */
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

    connectThreads();
}


void CodeInfoDock::connectThreads() {
    Handler = new ClangHandler(clang);
    signatureWorker = new ClangHandler(clang);
    definitionWorker = new ClangHandler(clang);
    codeCompleteWorker = new ClangHandler(clang);
    renameWorker = new ClangHandler(clang);
    preambleUpdateWorker = new ClangHandler(clang);
    findReferencesWorker = new ClangHandler(clang);
    formatFileWorker = new ClangHandler(clang);
    generateWorker = new ClangHandler(clang);
    highlightWorker = new ClangHandler(clang);

    signatureThread = new QThread(this);
    connect(signatureThread, &QThread::started, signatureWorker, &ClangHandler::slotGetSignature);
    connect(signatureThread, SIGNAL(finished()), this, SLOT(slotGetSignature()));
    signatureWorker->moveToThread(signatureThread);

    codeCompleteThread = new QThread(this);
    connect(codeCompleteThread, &QThread::started, codeCompleteWorker, &ClangHandler::slotCodeComplete);
    connect(codeCompleteThread, SIGNAL(finished()), this, SLOT(slotCodeComplete()));
    codeCompleteWorker->moveToThread(codeCompleteThread);

    renameThread = new QThread(this);
    connect(renameThread, &QThread::started, renameWorker, &ClangHandler::slotRename);
    connect(renameThread, SIGNAL(finished()), this, SLOT(slotRename()));
    renameWorker->moveToThread(renameThread);

    updatePreambleThread = new QThread(this);
    connect(updatePreambleThread, &QThread::started, preambleUpdateWorker, &ClangHandler::slotUpdatePreamble);
    connect(updatePreambleThread, SIGNAL(finished()), this, SLOT(slotUpdatePreamble()));
    preambleUpdateWorker->moveToThread(updatePreambleThread);

    findReferencesThread = new QThread(this);
    connect(findReferencesThread, &QThread::started, findReferencesWorker, &ClangHandler::slotFindReferences);
    connect(findReferencesThread, SIGNAL(finished()), this, SLOT(slotFindReferences()));
    findReferencesWorker->moveToThread(findReferencesThread);

    formatFileThread = new QThread(this);
    connect(formatFileThread, &QThread::started, formatFileWorker, &ClangHandler::slotFormatFile);
    connect(formatFileThread, SIGNAL(finished()), this, SLOT(slotFormatFile()));
    formatFileWorker->moveToThread(formatFileThread);

    goToDefinitionThread = new QThread(this);
    connect(goToDefinitionThread, &QThread::started, definitionWorker, &ClangHandler::slotGoToDefinition);
    connect(goToDefinitionThread, SIGNAL(finished()), this, SLOT(slotGoToDefinition()));
    definitionWorker->moveToThread(goToDefinitionThread);

    generateThread = new QThread(this);
    connect(generateThread, &QThread::started, generateWorker, &ClangHandler::slotGenerate);
    connect(generateThread, SIGNAL(finished()), this, SLOT(slotGenerate()));
    generateWorker->moveToThread(generateThread);

    highlightsThread = new QThread(this);
    connect(highlightsThread, &QThread::started, highlightWorker, &ClangHandler::slotGetHighlights);
    connect(highlightsThread, SIGNAL(finished()), this, SLOT(slotGetHighlights()));
    highlightWorker->moveToThread(highlightsThread);
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
            Refactor->show();
            renameThread->start();
            break;
        case CodeCheck:
            if (updatePreambleThread->isRunning()) {
                return;
            }
            preambleUpdateWorker->updateChecks = true;
            updatePreambleThread->start();
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
            *formatFileWorker->content = edit->toPlainText().toLatin1().data();
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
        case Highlights:
            if (highlightsThread->isRunning()) {
                return;
            }
            highlightsThread->start();
            break;

        default:
            break;
    }
}

void CodeInfoDock::updateDocks() {
    Linter->filePath->setText(edit->getFilePath());

    Refactor->Original->setText(edit->getWordUnderCursor());
    Refactor->Replacement->setText(edit->getWordUnderCursor());
}

void CodeInfoDock::slotShowReferencesDock() {
    References->setVisible(true);
    References->setFocus();
    References->raise();
    References->titleBarWidget()->setEnabled(false);
    // consider replacing or disabling title bar which is meant to be for search

    References->results->clear();
    References->selections.clear();
    References->MultifileSearchResults.clear();
    References->SearchFilesPaths.clear();

    // spaces are critical here, we do not want to add some " "
    const QString Stag = "<span ";
    const QString Style = "style=color:orange;";
    const QString Mtag = " >";
    const QString Etag = "</span>";


    FileDirManager file_manager;
    for (unsigned int i = 0; i < Handler->referencesLocation.size(); i++) {
        const QString filePath = QString::fromStdString(Handler->referencesLocation[i].filepath);
        const QString content = file_manager.read(filePath);
        qDebug() << content;
        if (!References->preview) {
            qDebug() << "!preview----------";
        }
        References->preview->setPlainText(content);
        // needed for findStoreAndSelectAll[0] item which is filepath
        References->preview->setFilePath(filePath);

        // FIXME: 1 reference 1 data struct --> try to merge 1 file references ! , also filePaths...
        const std::vector<QPoint> pos{{Handler->referencesLocation[i].x, Handler->referencesLocation[i].y}};
        References->MultifileSearchResults.push_back(pos);
        References->SearchFilesPaths.push_back(filePath);

        // search all files, help with PlainTextEdit methods by changing preview
        // we are interested only in line content for y position clang spit out.

        // add top item
        auto *file = new QTreeWidgetItem();
        file->setText(0, QFileInfo(filePath).fileName());
        file->setToolTip(0, filePath);
        References->results->addTopLevelItem(file);

        QString row_col = QString::number(Handler->referencesLocation[i].y) + ":" +
                          QString::number(Handler->referencesLocation[i].x);
        QString line_content = References->preview->getLineContent(Handler->referencesLocation[i].y, false);
        // Playing with positions :)
        const QString start = line_content.mid(0, Handler->referencesLocation[i].x - 2);
        const QString end = line_content.mid(Handler->referencesLocation[i].x - 1); // till end

        line_content = start + Stag + Style + Mtag + "WHAT??" + Etag + end;
        auto *posItem = new QTreeWidgetItem();
        posItem->setText(0, line_content + " &nbsp;&nbsp; " + row_col);
        file->addChild(posItem);

    }

    References->preview->clear();
    References->results->expandAll();
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
    /*
    //signatureThread->quit();
    signatureThread->terminate();
    signatureThread->wait();
    std::string label = clang->signatureHelpResults.signatures[0].label + "(";
    for(const auto& param : clang->signatureHelpResults.signatures[0].parameters) {
        label += " " + param.labelString;
    }
    label += ")";

    signature->setText(QString::fromStdString(label));
    documentation->setText(QString::fromStdString(clang->signatureHelpResults.signatures[0].documentation));
    */
}
void CodeInfoDock::slotCodeComplete() {
    /*
    //codeCompleteThread->quit();
    codeCompleteThread->terminate();
    codeCompleteThread->wait();
    std::vector<std::string> data;
    data.reserve(15);
    for(const auto& comp : clang->codeCompleteResults.Completions) {
        data.push_back(comp.Name);
    }
    //clang->codeCompleteResults.Completions[0].Name;
    edit->completer->setItems(data);
    */
}

void CodeInfoDock::slotRename() {
    /*
    //renameThread->quit();
    renameThread->terminate();
    renameThread->wait();
    Refactor->Original->clear();
    Refactor->Replacement->clear();
    */
}

void CodeInfoDock::slotUpdatePreamble() {
    /*
    //codeCheckThread->quit();
    updatePreambleThread->terminate();
    updatePreambleThread->wait();

    Handler->forcePreambleUpdate = false;
    Handler->updateChecks = false;

    // update list with checks
    for(const auto& diag : clang->PData->Diags) {
        const QString filePath = QString::fromStdString(diag.AbsFile.getValue());
        const std::string content = diag.Message + "  " + diag.Category + " " + diag.Name;
        // TODO: fixes are there, but does everyone has fixes ? and diag.Source

        Linter->addItem(QString::fromStdString(content), filePath, diag.Severity);

    }
    */
}
void CodeInfoDock::slotFindReferences() {
    //findReferencesThread->quit();
    findReferencesThread->terminate();
    findReferencesThread->wait();
    // show and fill dock
    slotShowReferencesDock();
}
void CodeInfoDock::slotFormatFile() {
    //formatFileThread->quit();
    formatFileThread->terminate();
    formatFileThread->wait();
}
void CodeInfoDock::slotGoToDefinition() {
    /*
    //goToDefinitionThread->quit();
    goToDefinitionThread->terminate();
    goToDefinitionThread->wait();
    const std::string filepath = clang->locatedDefDecl[0].Definition->uri.uri();
    const int line = clang->locatedDefDecl[0].Definition->range.start.line;
    const int x = clang->locatedDefDecl[0].Definition->range.start.character;

    std::cout << filepath;
    */
}

void CodeInfoDock::slotGenerate() {
    //generateThread->quit();
    generateThread->terminate();
    generateThread->wait();
}

void CodeInfoDock::slotGetHighlights() {
    //generateThread->quit();
    highlightsThread->terminate();
    highlightsThread->wait();
    //edit->
    // we have to track what we already have highlighted
    //clang->highlightingTokens[0].R.contains(clang::clangd::Position());
    for (const auto &token : clang->highlightingTokens) {
        switch (token.Kind) {
            case HighlightingKind::Variable:
                // highlight token with token.R  ... range
                break;
                // and so on
        }
    }
}

