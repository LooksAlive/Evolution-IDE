#include "mainwindow.h"
#include "icons/IconFactory.h"
#include "ui_mainwindow.h"
#include "Clang/ClangBridge.h"

//#include "qconsole.h"

/*
keyword this always represents parent in which class it is declared, also
want to show exact widget into this section(mainwindow - app)
*/

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    //ui->setupUi(this);
    setAcceptDrops(true);
    setWindowIcon(QIcon(":/icons/icon.png"));
    setWindowTitle("Evolution IDE");
    setContentsMargins(0, 0, 0, 0);

    /* set all necessary widgets, features etc. */
    SetupTabWidget();
    SetupFileExplorer();
    SetupFileDocker();
    SetupCompileDock();
    SetupCodeInfoDock();
    SetupEducationDock();
    SetupMenuBar();
    SetupToolBar();


    SetupNodeView();
    SetupDebuggerView();
    SetupBinaryView();
    SetupHexView();
    SetupVerticalBar();

    CreateFile();
    SetupStatusBar();

    SetupDockWidgetsLayering();

    setCentralWidget(vertical_stack);
    Tabs->currentWidget()->setFocus();
    highlighter = new Highlighter(":/highlights/languages.xml", this);


    LoadRegisters();
}

MainWindow::~MainWindow() {
    delete ui;
}

/* drag and drop functions for file into window as it would be openned */
void MainWindow::dragEnterEvent(QDragEnterEvent *drag_event) {
    if (drag_event->mimeData()->hasUrls())
        drag_event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *drop_event) {
    QList<QUrl> url_list = drop_event->mimeData()->urls();
    // set dir into file explorer as default for project; handled in drop event
    foreach (QUrl url, url_list) {
        QString path = url.url(QUrl::RemoveScheme);
        OpenFile(path);
    }
}

void MainWindow::LoadRegisters() {
    QSettings settings("Evolution");

    QRect rect = settings.value("Evolution/MainWindowGeometry").toRect();
    if (!rect.isEmpty()) {
        setGeometry(rect);
    }

    /*
    QStringList tabs = settings.value("Evolution/opened_tabs").toStringList();
    auto positions = settings.value("Evolution/opened_tabs_cursor_positions").toList();
    if(!tabs.isEmpty()){
        for (int i = 0; i < tabs.size(); i++) { // one error, default tab has no path
            OpenFile(tabs[i]);
            (qobject_cast<PlainTextEdit *>(Tabs->widget(i)))->textCursor().setPosition(positions[i].toInt());
        }
    }
    */

    QString Project_Root_Dir = settings.value("Evolution/Project_Root_Dir").toString();
    if (!Project_Root_Dir.isEmpty()) {
        Explorer->setRootDirectory(Project_Root_Dir);
        file_manager.Project_Dir = Project_Root_Dir;
    }

    bool visible = false;

    // visible docks:
    visible = settings.value("Evolution/ExplorerVisibility").toBool();
    if (visible) {
        Explorer->setVisible(true);
    }
    visible = settings.value("Evolution/DockerVisibility").toBool();
    if (visible) {
        Docker->setVisible(true);
    }
    visible = settings.value("Evolution/ConsoleOutputVisibility").toBool();
    if (visible) {
        console_dock->setVisible(true);
    }
    visible = settings.value("Evolution/CodeInfoDockVisibility").toBool();
    if (visible) {
        codeInfoDock->setVisible(true);
    }
    visible = settings.value("Evolution/EducationVisibility").toBool();
    if (visible) {
        education->setVisible(true);
    }
    // i do not find search dock really necessary
}

void MainWindow::SetupVerticalBar() {
    vertical_bar = new QToolBar(this);
    vertical_bar->setWindowTitle("Windows");
    vertical_bar->setOrientation(Qt::Vertical);
    vertical_bar->setMovable(false);
    //vertical_bar->setSizePolicy(QSizePolicy::Fixed);
    vertical_bar->setFixedWidth(70);
    vertical_bar->setFloatable(false);
    vertical_bar->setAcceptDrops(false);
    vertical_bar->setIconSize(QSize(70, 35));

    //vertical_bar->setContentsMargins(10,10,10,10);
    vertical_bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    vertical_bar->setFocusPolicy(Qt::ClickFocus);

    vertical_stack = new QStackedWidget(this);
    vertical_stack->setParent(this);

    vertical_stack->insertWidget(0, Tabs);
    vertical_stack->insertWidget(3, nodeview);
    vertical_stack->insertWidget(1, binaryView);
    vertical_stack->insertWidget(2, debuggerView);
    vertical_stack->insertWidget(3, hexview);

    vertical_stack->setCurrentIndex(0);// start with editor

    vertical_bar->addAction(QIcon(IconFactory::EditorView), "Editor", this, SLOT(showEditorView()));
    vertical_bar->addSeparator();
    vertical_bar->addAction(QIcon(IconFactory::NodeView), "Node View", this, SLOT(showNodeView()));
    vertical_bar->addSeparator();
    vertical_bar->addAction(QIcon(IconFactory::HexView), "Hex Editor", this, SLOT(showHexView()));
    vertical_bar->addSeparator();
    vertical_bar->addAction(QIcon(IconFactory::DebuggerView), "Debugger", this, SLOT(showDebuggerView()));
    vertical_bar->addSeparator();
    vertical_bar->addAction(QIcon(IconFactory::BinaryView), "Binary View", this, SLOT(showBinaryView()));
    vertical_bar->addSeparator();
    // decompiler ... later :)
    //vertical_bar->addAction(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/hex.png"), "Decompiler View",  this, SLOT(showBinaryView()));
    //vertical_bar->addSeparator();

    addToolBar(Qt::LeftToolBarArea, vertical_bar);
}

void MainWindow::SetupStatusBar() {
    statusbar = new QStatusBar(this);
    progress_bar = new ProgressBar(this);
    btn_encoding = new QToolButton(this);
    btn_position = new QToolButton(this);

    statusbar->setContentsMargins(0, 0, 0, 0);
    statusbar->setFixedHeight(25);
    statusbar->setMouseTracking(true);
    btn_encoding->setText("UTF-8");
    btn_encoding->setFixedWidth(50);
    btn_encoding->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    btn_position->setFixedWidth(70);
    btn_position->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    connect(btn_position, SIGNAL(clicked()), this, SLOT(slotGoToLine()));

    statusbar->setWindowFlags(Qt::FramelessWindowHint);
    statusbar->setAttribute(Qt::WA_NoSystemBackground, true);
    statusbar->setAttribute(Qt::WA_TranslucentBackground, true);

    statusbar->showMessage("loading", 5000);
    statusbar->addPermanentWidget(progress_bar);// align right with permanent widget
    statusbar->addPermanentWidget(btn_position);
    statusbar->addPermanentWidget(btn_encoding);
    setStatusBar(statusbar);
}

// after line is set , field is empty
void MainWindow::slotTextPositionChanged() {
    // later consider creating new button here
    // What if on the changed tab is no cursor set -> remain last cursor position
    if (Tabs->isActiveWindow()) {
        QPoint point = currentWidget->getCursorPosition();
        QString pos = QString::number(point.x()) + ":" + QString::number(point.y());// row:col

        btn_position->setText(pos);
    } else {
        btn_position->setText("");
    }
}

void MainWindow::slotGoToLine() {
    auto *goTo = new GoToLineColumn(currentWidget, this);
    goTo->show();
    slotTextPositionChanged();// update  position
}

void MainWindow::SetupMenuBar() {

    menuBar = new QMenuBar(this);
    menuBar->setWindowTitle("Menu bar");
    //menuBar->setObjectName("Menubar");
    //menuBar->setGeometry(QRect(0, 0, 1000, 24));
    //menuBar->setFixedHeight(24);    // or none, still work well
    menuBar->setMouseTracking(true);
    //menuBar->setNativeMenuBar(false);
    menuBar->setTabletTracking(true);

    // if there will not be this param, will not show, or add geometry manually like above
    auto *fileMenu = new QMenu("File", this);
    auto *editMenu = new QMenu("Edit", this);
    auto *viewMenu = new QMenu("View", this);
    auto *DebugMenu = new QMenu("Debug", this);
    auto *AnalyzeMenu = new QMenu("Analyze", this);
    auto *EducationMenu = new QMenu("Education", this);
    auto *HelpMenu = new QMenu("Help", this);


    fileMenu->addAction(QIcon(IconFactory::NewFile), "New File", this, SLOT(CreateFile()), Qt::CTRL + Qt::Key_N);
    fileMenu->addAction(QIcon(IconFactory::OpenFile), "Open File", this, SLOT(OpenFile()), Qt::CTRL + Qt::Key_O);
    fileMenu->addSeparator();
    fileMenu->addAction(QIcon(IconFactory::SaveFile), "Save", this, SLOT(SaveFile()), Qt::CTRL + Qt::Key_S);
    fileMenu->addAction("Save As...", this, SLOT(SaveFileAs()), Qt::SHIFT + Qt::CTRL + Qt::Key_S);
    fileMenu->addAction(QIcon(IconFactory::SaveAllFiles), "Save All", this, SLOT(SaveAllFiles()));
    fileMenu->addSeparator();
    fileMenu->addAction("Close File", this, SLOT(CloseFile()), Qt::CTRL + Qt::Key_W);
    fileMenu->addAction("Close All", this, SLOT(CloseAllFiles()), Qt::SHIFT + Qt::CTRL + Qt::Key_W);
    fileMenu->addSeparator();
    fileMenu->addAction(QIcon(IconFactory::Settings), "Settings", this, SLOT(SetupSettingsWindow()), Qt::CTRL + Qt::Key_P);
    fileMenu->addSeparator();
    fileMenu->addAction("Restart", this, SLOT(slotRestart()));
    fileMenu->addAction(QIcon(IconFactory::ShutDown), "Exit", this, SLOT(CloseWindow()), Qt::CTRL + Qt::Key_Q);

    editMenu->addAction(QIcon(IconFactory::Copy), "Copy", this, SLOT(slotCopy()), Qt::CTRL + Qt::Key_C);
    editMenu->addAction(QIcon(IconFactory::Paste), "Paste", this, SLOT(slotPaste()), Qt::CTRL + Qt::Key_V);
    editMenu->addAction(QIcon(IconFactory::Cut), "Cut", this, SLOT(slotCut()), Qt::CTRL + Qt::Key_X);
    editMenu->addAction(QIcon(IconFactory::Undo), "Undo", this, SLOT(slotUndo()), Qt::CTRL + Qt::Key_Z);
    editMenu->addAction(QIcon(IconFactory::Redo), "Redo", this, SLOT(slotRedo()), Qt::CTRL + Qt::SHIFT + Qt::Key_X);
    editMenu->addAction("Remove All", this, SLOT(slotRemoveAll()), Qt::CTRL + Qt::Key_Backspace);
    editMenu->addAction(QIcon(IconFactory::Expand), "Expand", this, SLOT(slotExpand()));
    editMenu->addAction(QIcon(IconFactory::Collapse), "Collapse", this, SLOT(slotCollapse()));
    editMenu->addAction(QIcon(IconFactory::SelectAll), "Select All", this, SLOT(slotSelectAll()), Qt::CTRL + Qt::Key_A);
    editMenu->addSeparator();
    editMenu->addAction(QIcon(IconFactory::Comment), "toggle comment", this, SLOT(slotToggleComment()), Qt::CTRL + Qt::SHIFT + Qt::Key_C);
    editMenu->addAction("Format Code", this, SLOT(slotFormat()), Qt::CTRL + Qt::SHIFT + Qt::Key_F);

    viewMenu->addAction(Explorer->toggleViewAction());
    viewMenu->addAction(Docker->toggleViewAction());
    viewMenu->addAction(console_dock->toggleViewAction());
    viewMenu->addAction(find_replace->toggleViewAction());
    viewMenu->addAction(codeInfoDock->toggleViewAction());
    viewMenu->addSeparator();
    viewMenu->addAction(QIcon(IconFactory::FullScreen), "Full Screen", this, SLOT(slotFullScreen()));
    viewMenu->addAction("Converter", this, SLOT(SetupConverter()));

    DebugMenu->addAction(QIcon(IconFactory::StartDebug), "Start Debug", this, SLOT(slotStartDebug()), Qt::Key_F5);
    DebugMenu->addAction(QIcon(IconFactory::StopDebug), "Stop Debug", this, SLOT(slotStopDebug()));
    DebugMenu->addAction("Attach Process", this, SLOT(slotShowAttachToProcess()));
    DebugMenu->addAction(QIcon(IconFactory::RunToCursor), "Run to cursor", this, SLOT(slotRunToCursor()));
    DebugMenu->addAction(QIcon(IconFactory::Resume), "Continue Debug", this, SLOT(slotContinue()));
    DebugMenu->addSeparator();
    DebugMenu->addAction(QIcon(IconFactory::NextLine), "Step Over", this, SLOT(slotStepOver()), Qt::Key_F6);
    DebugMenu->addAction(QIcon(IconFactory::StepInto), "Step Into", this, SLOT(slotStepInto()), Qt::Key_F7);
    DebugMenu->addAction(QIcon(IconFactory::StepInstruction), "Step Instruction", this, SLOT(slotStepInstruction()));
    DebugMenu->addAction(QIcon(IconFactory::GetOutOfFunction), "Step Out", this, SLOT(slotStepOut()));
    DebugMenu->addSeparator();
    DebugMenu->addAction("Toggle Breakpoint", this, SLOT(slotToggleBreakPoint()), Qt::Key_F9);
    DebugMenu->addAction("Set breakpoint at line", this, SLOT(slotSetBreakpointAtLine()), Qt::SHIFT + Qt::Key_F9);
    DebugMenu->addAction("Show All Breakpoints", this, SLOT(slotShowBreakpointsList()));

    AnalyzeMenu->addAction("Run Clang-Tidy", this, SLOT(slotClangTidy()));
    AnalyzeMenu->addAction("Run Clang-Check", this, SLOT(slotClangCheck()));
    AnalyzeMenu->addAction("Document File", this, SLOT(slotClangDocGenerate()));
    AnalyzeMenu->addAction("Run Valgrind", this, SLOT(slotValgrind()));
    AnalyzeMenu->addAction("Run Gbd-Gui", this, SLOT(slotGdbGui()));

    EducationMenu->addAction(education->toggleViewAction());

    HelpMenu->addAction("About Evolution", this, SLOT(slotAbout()));

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
    menuBar->addMenu(viewMenu);
    menuBar->addMenu(DebugMenu);
    menuBar->addMenu(AnalyzeMenu);
    menuBar->addMenu(EducationMenu);
    menuBar->addMenu(HelpMenu);

    //qDebug() << menuBar->isVisible();
    setMenuBar(menuBar);
}

void MainWindow::SetupToolBar() {
    topToolBar = new QToolBar(this);
    topToolBar->setWindowTitle("Top ToolBar");
    topToolBar->setMovable(false);
    addToolBar(Qt::TopToolBarArea, topToolBar);

    // ":/icons/new_file.png"
    topToolBar->addAction(QIcon(IconFactory::NewFile), "Create New File", this, SLOT(CreateFile()));
    // ":/icons/open_file.png"
    topToolBar->addAction(QIcon(IconFactory::OpenFile), "Open File", this, SLOT(OpenFile()));
    // ":/icons/save_file.png"
    topToolBar->addAction(QIcon(IconFactory::SaveFile), "Save File", this, SLOT(SaveFile()));
    topToolBar->addAction(QIcon(IconFactory::SaveAllFiles), "Save All Files", this, SLOT(SaveAllFiles()));
    topToolBar->addAction(QIcon(IconFactory::Undo), "undo", this, SLOT(slotUndo()));
    topToolBar->addAction(QIcon(IconFactory::Redo), "redo", this, SLOT(slotRedo()));

    auto *spacer = new QWidget(this);// blank Widget to align other action to right
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    topToolBar->addWidget(spacer);
    topToolBar->addAction(QIcon(IconFactory::Build), "Build", this, SLOT(slotBuild()));
    topToolBar->addAction(QIcon(IconFactory::Run), "Run", this, SLOT(slotRun()));
    topToolBar->addAction(QIcon(IconFactory::Stop), "Stop Process", this, SLOT(slotStopProcess()));
}


void MainWindow::SetupDockWidgetsLayering() {
    setDockOptions(QMainWindow::AllowTabbedDocks | QMainWindow::AllowNestedDocks |
                   QMainWindow::GroupedDragging | QMainWindow::AnimatedDocks);
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
}

// switching views
void MainWindow::showEditorView() {
    ShowHiddenDockWidgets();
    vertical_stack->setCurrentWidget(Tabs);
}

void MainWindow::showNodeView() {
    // Will there be some dock visible ???
    HideAllDockWidgets();
    vertical_stack->setCurrentWidget(nodeview);
}

void MainWindow::showBinaryView() {
    HideAllDockWidgets();
    vertical_stack->setCurrentWidget(binaryView);
}

void MainWindow::showDebuggerView() {
    HideAllDockWidgets();

    vertical_stack->setCurrentWidget(debuggerView);

    QString path = currentWidget->getFilePath();
    // for now, starting with current file, later track    ;  not i only care for line
    // file_manager.current_full_filepath, 0
    //file_manager.executable_file_path = "/home/adam/Desktop/SKK/cmake-build/executable";
    debuggerView->setStartPosition(path.toLatin1().data(), currentWidget->getCursorPosition().y());
    //debuggerView->setExecutable(file_manager.executable_file_path.toStdString());
}

void MainWindow::showHexView() {
    HideAllDockWidgets();

    // Tabs->tabToolTip(Tabs->currentIndex());
    QString path = currentWidget->getFilePath();
    if (path != "") {
        hexview->open(path);
        vertical_stack->setCurrentWidget(hexview);
    } else {
        // not saved file, get data and convert right :)
        QString data = currentWidget->toPlainText();
        hexview->setText(data.toUtf8());
        vertical_stack->setCurrentWidget(hexview);
    }
}

void MainWindow::HideAllDockWidgets() {
    if (Explorer->isVisible()) {
        explorer_visible = true;
        Explorer->hide();
    }
    if (Docker->isVisible()) {
        docker_visible = true;
        Docker->hide();
    }
    if (console_dock->isVisible()) {
        console_dock_visible = true;
        console_dock->hide();
    }
    if (find_replace->isVisible()) {
        find_replace_visible = true;
        find_replace->hide();
    }
    if (codeInfoDock->isVisible()) {
        code_info_dock_visible = true;
        codeInfoDock->hide();
    }
}

void MainWindow::ShowHiddenDockWidgets() {
    if (explorer_visible) {
        Explorer->show();
    }
    if (docker_visible) {
        Docker->show();
    }
    if (console_dock_visible) {
        console_dock->show();
    }
    if (find_replace_visible) {
        find_replace->show();
    }
    if (code_info_dock_visible) {
        codeInfoDock->show();
    }
}


void MainWindow::SetupTabWidget() {
    Tabs = new Tab(this);
    connect(Tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(CloseFile(int)));
    connect(Tabs->tabBar(), SIGNAL(currentChanged(int)), this, SLOT(UpdateCurrentIndex(int)));
    connect(Tabs->tabBar(), SIGNAL(tabCloseRequested(int)), this, SLOT(UpdateCurrentIndexOnDelete(int)));
    connect(Tabs->tabBar(), SIGNAL(tabMoved(int, int)), this, SLOT(ChangeTabIndexInList(int, int)));
    connect(Tabs->AddNewTabButton, SIGNAL(clicked()), this, SLOT(CreateFile()));
}


void MainWindow::SetupFileExplorer() {
    Explorer = new FileExplorer(this);
    // Explorer->setRootDirectory(QDir::homePath());

    connect(Explorer->FileView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(OpenFile(QModelIndex)));
    addDockWidget(Qt::LeftDockWidgetArea, Explorer); /* show at left side; function for MainWindow */
    /*
    auto *dock = new QDockWidget(this);
    auto *console = new QConsole(this);
    dock->setWidget(console);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    dock->setVisible(true);
    */
}

void MainWindow::SetupFileDocker() {
    Docker = new FileDock(this);

    connect(Docker->DockerFileList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(UpdateCurrentIndex(QListWidgetItem *)));
    connect(Docker->DockerFileList, SIGNAL(currentRowChanged(int)), Tabs, SLOT(setCurrentIndex(int)));
    addDockWidget(Qt::LeftDockWidgetArea, Docker);
}


void MainWindow::SetupCompileDock() {
    console_dock = new ConsoleDock(this);
    find_replace = new FindReplaceWidget(Tabs, this);

    connect(new QShortcut(Qt::CTRL + Qt::Key_F, this), &QShortcut::activated, [=] { slotFind(); });

    addDockWidget(Qt::BottomDockWidgetArea, console_dock);
    addDockWidget(Qt::BottomDockWidgetArea, find_replace);

    tabifyDockWidget(console_dock, find_replace);
}

void MainWindow::slotFind() {
    find_replace->setVisible(true);// insted of toggleViewAction()
    find_replace->LineEditFind->setFocus();
    QString text = currentWidget->textCursor().selectedText();
    // find selected text
    if (!text.isEmpty()) {
        find_replace->LineEditFind->setText(text);
    }
    // find word under cursor, if no text is selected
    else {
        text = currentWidget->getWordUnderCursor();
        find_replace->LineEditFind->setText(text);
        find_replace->slotNext();
        // also selections, slotNext wont do that, bc. yet no temp string is set
        currentWidget->findStoreAndSelectAll(text);
    }
}

void MainWindow::SetupCodeInfoDock() {
    clangBridge = new ClangBridge();

    codeInfoDock = new CodeInfoDock(this);
    addDockWidget(Qt::BottomDockWidgetArea, codeInfoDock);
    tabifyDockWidget(console_dock, codeInfoDock);
    codeInfoDock->setEditor(currentWidget);
    codeInfoDock->setClang(clangBridge);
}


/* external windows */

void MainWindow::SetupSettingsWindow() {
    Settings = new SettingsWindow(this);
    Settings->show();
}

void MainWindow::SetupConverter() {
    converter = new Converter(this);
    converter->show();
}

void MainWindow::SetupEducationDock() {
    education = new Education(this);
    addDockWidget(Qt::RightDockWidgetArea, education);
    // has to be outside bc. i will use option to add a sample in editor so cannot import them cross

    connect(education->CppCodeSamples, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotOpenCppSample(QListWidgetItem *)));
    connect(education->CppUsersSamples, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotOpenCppUserSample(QListWidgetItem *)));
    // should there be a disconnection when main window is destructed ?
}

void MainWindow::SetupNodeView() {
    nodeview = new NodeView(this);
}

void MainWindow::SetupDebuggerView() {
    debuggerView = new DebuggerWidget(this);
    //addDockWidget(Qt::BottomDockWidgetArea, debuggerView->DebuggerDock);
}
void MainWindow::SetupBinaryView() {
    binaryView = new BinaryView(this);
}
void MainWindow::SetupHexView() {
    hexview = new HexView(this);
}
void MainWindow::showDecompilerView() {
}


/* File handling stuffs */

void MainWindow::CreateFile() {
    // tab
    auto *NewPlainText = new PlainTextEdit;
    int index = Tabs->addTab(NewPlainText, NEW_TAB_NAME);
    NewPlainText->setFilePath("");

    Tabs->setCurrentIndex(index);
    Tabs->setTabToolTip(index, "");
    Tabs->setTabWhatsThis(index, "No changes");

    currentWidget = qobject_cast<PlainTextEdit *>(Tabs->widget(index));
    currentWidget->setCodeInfo(codeInfoDock);
    currentWidget->setEducation(education);
    //currentWidget->setClang(clangBridge);
    // go to line/column
    connect(currentWidget, SIGNAL(cursorPositionChanged()), this, SLOT(slotTextPositionChanged()));
    connect(currentWidget, SIGNAL(textChanged()), this, SLOT(UpdateParameter()));
    // breakpoints
    // TODO: if i pass debugger widget by instance pointer, will it be faster ??
    connect(currentWidget->BreakpointArea, SIGNAL(breakPointCreated(const int &)), this, SLOT(slotCreateBreakPoint(const int &)));
    connect(currentWidget->BreakpointArea, SIGNAL(breakPointRemoved(const int &)), this, SLOT(slotDeleteBreakPoint(const int &)));

    // file dock
    auto *new_item = new QListWidgetItem;
    new_item->setText(Tabs->tabText(index));
    new_item->setToolTip(Tabs->tabToolTip(index));
    // new_item->setIcon(QIcon(IconFactory::Remove));
    Docker->DockerFileList->addItem(new_item);

    UpdateCurrentIndex(index);
}

/* Base operations in MenuBar
--------------------------------------------------------------------------*/


void MainWindow::OpenFile() {
    // #TODO:  do not work yet << setfilemode, to set dir into explorer from dialog
    auto *dialog = new QFileDialog(this);
    dialog->setFileMode(QFileDialog::ExistingFiles);
    dialog->setFileMode(QFileDialog::Directory);

    //QString filepath = dialog->getOpenFileName(this, "Open file", QDir::homePath());
    QString filepath = dialog->getExistingDirectory(this, "Open directory", QDir::homePath());


    if (filepath.isEmpty())
        return;

    OpenFile(filepath);
}

void MainWindow::OpenFile(const QString &filepath) {

    // directory operations
    if (QFileInfo(filepath).isDir()) {
        // set project path into file view
        Explorer->setRootDirectory(filepath);
        // set recursively all files in dir, set root project dir
        file_manager.getFilesRecursively(filepath);
        return;
    }

    auto *new_text_edit = new PlainTextEdit();
    new_text_edit->appendPlainText(file_manager.read(filepath));
    new_text_edit->setFilePath(filepath);
    new_text_edit->setFileExtension(file_manager.getFileExtension(file_manager.current_file_name));

    /* checks for duplicate file-openning and prevents it by opening identical tab twice */

    for (int i = 0; i < Tabs->count(); ++i)
        if (Tabs->tabToolTip(i) == file_manager.current_full_filepath) {
            Tabs->setCurrentIndex(i);
            return;
        }

    // ????????????????????????????????????????????????????????
    if (currentWidget->document()->isEmpty() &&
        Tabs->tabToolTip(Tabs->currentIndex()) == "" &&
        Tabs->tabText(Tabs->currentIndex()) == NEW_TAB_NAME) {

        DeleteTabFromList(Tabs->currentIndex());
        delete Tabs->widget(Tabs->currentIndex());
    }


    // tab
    // icons for tab, like in file view
    QFileIconProvider provider;
    int index = Tabs->addTab(new_text_edit, provider.icon(QFileInfo(filepath)), file_manager.current_file_name);
    Tabs->setCurrentIndex(index);
    Tabs->setTabToolTip(index, file_manager.current_full_filepath);
    Tabs->setTabWhatsThis(index, "No changes");
    connect(new_text_edit, SIGNAL(textChanged()), this, SLOT(UpdateParameter()));

    // file Docker
    auto *new_item = new QListWidgetItem();
    new_item->setText(Tabs->tabText(index));
    new_item->setToolTip(Tabs->tabToolTip(index));
    // new_item->setIcon(QIcon(IconFactory::Remove));
    Docker->DockerFileList->addItem(new_item);

    // setting up highlight
    if (highlighter->setExtension(file_manager.getFileExtension(file_manager.current_file_name))) {
        highlighter->setDocument(new_text_edit->document());
        highlighter->highlightBlock(new_text_edit->toPlainText());
    }

    Tabs->setTabWhatsThis(index, "No changes");
    UpdateCurrentIndex(index);// setting up selected item in opened_docs_dock
}


void MainWindow::SaveFile() {
    // new file created, but not saved yet
    if (Tabs->tabToolTip(Tabs->currentIndex()) == "") {
        // empty file, saving as
        CHANGES_IN_PROJECT = true;
        SaveFileAs();
        return;
    }
    CHANGES_IN_PROJECT = true;

    file_manager.write(Tabs->tabToolTip(Tabs->currentIndex()),// filepath, buffer
                       currentWidget->toPlainText().toUtf8());

    // ??????????????????????????????????????????
    Tabs->setTabWhatsThis(Tabs->currentIndex(), "No changes");
}

void MainWindow::SaveFileAs() {

    CHANGES_IN_PROJECT = true;

    //QString filename = Tabs->tabText(Tabs->currentIndex());
    QString filepath = QFileDialog::getSaveFileName(this, "Save as");
    if (filepath.isEmpty())
        return;

    file_manager.write(filepath, currentWidget->toPlainText().toUtf8());


    Tabs->tabBar()->setTabText(Tabs->currentIndex(), file_manager.current_file_name);
    Tabs->tabBar()->setTabToolTip(Tabs->currentIndex(), filepath);
    currentWidget->setFilePath(filepath);
    currentWidget->setFileExtension(file_manager.getFileExtension(file_manager.current_file_name));

    // setting up highlight
    if (highlighter->setExtension(file_manager.getFileExtension(file_manager.current_file_name))) {
        highlighter->setDocument(currentWidget->document());      // unsafe getting!
        highlighter->highlightBlock(currentWidget->toPlainText());// unsafe getting!
    }

    Tabs->setTabWhatsThis(Tabs->currentIndex(), "No changes");
}

void MainWindow::SaveAllFiles() {
    int current_index = Tabs->currentIndex();
    for (int i = 0; i <= Tabs->count(); i++) {// QTabWidget guarantees the consistency of indices?
        Tabs->setCurrentIndex(i);
        if (Tabs->tabWhatsThis(Tabs->currentIndex()) != "No changes")
            SaveFile();
    }
    Tabs->setCurrentIndex(current_index);
}

/* help function for tab index close action */
void MainWindow::CloseFile(int index_) {
    // TODO: here implement format_afterFileClosed, when closing file

    // untitled tab, first ask, before opening savedialog in savefile function
    if (Tabs->tabText(index_) == NEW_TAB_NAME && Tabs->tabWhatsThis(index_) != "No changes") {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Saving changes", "Save changes before closing?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            SaveFile();
        }
    } else if (Tabs->tabWhatsThis(index_) != "No changes") {
        if (ALWAYS_SAVE) {
            SaveFile();
        }
    }

    // breakpoints
    disconnect(currentWidget->BreakpointArea, SIGNAL(breakPointCreated(const int &)), this, SLOT(slotCreateBreakPoint(const int &)));
    disconnect(currentWidget->BreakpointArea, SIGNAL(breakPointRemoved(const int &)), this, SLOT(slotDeleteBreakPoint(const int &)));
    // go to line/column
    disconnect(currentWidget, SIGNAL(cursorPositionChanged()), this, SLOT(slotTextPositionChanged()));
    delete Tabs->widget(index_);
    DeleteTabFromList(index_);

    if (!Tabs->count())
        CreateFile();
    Tabs->currentWidget()->setFocus();
}

void MainWindow::CloseFile() {
    CloseFile(Tabs->currentIndex());
}

void MainWindow::CloseAllFiles() {
    bool some_changes = false;
    for (int i = 0; i <= Tabs->count(); i++) {// QTabWidget guarantees the consistency of indices?
        if (Tabs->tabWhatsThis(i) != "No changes") {
            some_changes = true;
            break;
        }
    }
    if (some_changes && !ALWAYS_SAVE) {
        QMessageBox::StandardButton reply = QMessageBox::question(
                this, "Save all changes", "Save all changes before closing?",
                QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
            SaveAllFiles();
    } else if (ALWAYS_SAVE) {
        SaveAllFiles();
    }
    while (Tabs->count() > 0)
        delete Tabs->widget(0);

    Docker->DockerFileList->clear();

    CreateFile();
    Tabs->currentWidget()->setFocus();
}

/* close all files, prevent memory leak */
void MainWindow::CloseWindow() {
    QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Exit",
            "Are you sure, you want to exit Evolution-IDE ?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }
    // reopen project not closed tabs later.
    QStringList opened_tabs;
    QList<int> tabs_cursor_positions;
    for (int i = 0; i <= Tabs->count(); i++) {
        if (Tabs->tabText(i) != "untitled") {// default tab has no path, since is not saved
            opened_tabs.push_back(Tabs->tabToolTip(i));
            //tabs_cursor_positions.push_back(qobject_cast<PlainTextEdit*>(Tabs->widget(i))->textCursor().position());
        }
    }
    QSettings settings("Evolution");
    settings.setValue("Evolution/opened_tabs", opened_tabs);
    //settings.setValue("Evolution/opened_tabs_cursor_positions", QVariant::fromValue(tabs_cursor_positions));
    // save window geometry, load in constructor
    settings.setValue("Evolution/MainWindowGeometry", geometry());

    // docks visible
    settings.setValue("Evolution/ExplorerVisibility", Explorer->isVisible());
    settings.setValue("Evolution/DockerVisibility", Docker->isVisible());
    settings.setValue("Evolution/ConsoleOutputVisibility", console_dock->isVisible());
    settings.setValue("Evolution/CodeInfoDockVisibility", codeInfoDock->isVisible());
    settings.setValue("Evolution/EducationVisibility", education->isVisible());

    CloseAllFiles();
    QApplication::quit();
}

/* X --> close app - virtual func. */
void MainWindow::closeEvent(QCloseEvent *) {
    CloseWindow();
}

void ::MainWindow::UpdateParameter() {
    /* highlight bad support (always changed) */
    QString file = Tabs->tabBar()->tabText(Tabs->currentIndex());
    QString file_extension = QFileInfo(file).suffix();
    if (!file_extension.isEmpty()) {
        if (highlighter->setExtension(file_extension)) {
            Tabs->setTabWhatsThis(Tabs->currentIndex(), "Changed");
            return;
        }
    }
    Tabs->setTabWhatsThis(Tabs->currentIndex(), "Changed");
    disconnect(sender(), SIGNAL(textChanged()), this, SLOT(UpdateParameter()));
}


void MainWindow::OpenFile(QModelIndex file_index) {
    if (!Explorer->FileModel->isDir(file_index)) {
        OpenFile(Explorer->FileModel->filePath(file_index));

        currentWidget = qobject_cast<PlainTextEdit *>(Tabs->widget(Tabs->currentIndex()));
        currentWidget->setCodeInfo(codeInfoDock);
        currentWidget->setEducation(education);
        //currentWidget->setClang(clangBridge);
        connect(currentWidget, SIGNAL(cursorPositionChanged()), this, SLOT(slotTextPositionChanged()));
        // breakpoints
        connect(currentWidget->BreakpointArea, SIGNAL(breakPointCreated(const int &)), this, SLOT(slotCreateBreakPoint(const int &)));
        connect(currentWidget->BreakpointArea, SIGNAL(breakPointRemoved(const int &)), this, SLOT(slotDeleteBreakPoint(const int &)));
    }
}


/* tab functions ;   also move into separate file with tabs
--------------------------------------------------------------------------*/

void MainWindow::DeleteTabFromList(int index) {
    QListWidgetItem *temp_item = Docker->DockerFileList->takeItem(index);
    delete temp_item;
}

void MainWindow::ChangeTabIndexInList(int old_index, int new_index) {
    QListWidgetItem *first_item = Docker->DockerFileList->takeItem(old_index);
    Docker->DockerFileList->insertItem(new_index, first_item);
}

void MainWindow::UpdateCurrentIndex(QListWidgetItem *current_item) {
    int index = current_item->listWidget()->row(current_item);
    currentWidget = qobject_cast<PlainTextEdit *>(Tabs->widget(index));
    Tabs->setCurrentIndex(index);
}

void MainWindow::UpdateCurrentIndex(int new_selection_index) {
    Docker->DockerFileList->setCurrentRow(new_selection_index);
    currentWidget = qobject_cast<PlainTextEdit *>(Tabs->widget(new_selection_index));


    // + highlight update
    QString file = Tabs->tabBar()->tabText(new_selection_index);
    QString file_extension = QFileInfo(file).suffix();
    if (!file_extension.isEmpty()) {
        if (highlighter->setExtension(file_extension)) {
            highlighter->setDocument(currentWidget->document());
            highlighter->highlightBlock(currentWidget->toPlainText());
        }
    }
}


void MainWindow::slotBuild() {

    // clear terminal window
    //console_dock->slotClearConsole();
    // deactivate action build

    bool cmake = true;
    /*
    QString exec = file_manager.Project_Dir +"/" + "executable";
    if(!CHANGES_IN_PROJECT && QDir::exists(exec)){
        return;
    }
    */

    if (cmake) {// set option flags to some default
        CmakeGenerator generator;
        generator.setProjectName("executable");
        generator.setCompiler("clang++");
        generator.setCompileFlags("-O0 -g ");
        for (int i = 0; i < file_manager.source_files.size(); i++) {
            generator.addSourceFile((file_manager.source_files[i].toStdString()));
        }
        generator.createCmakeLists(file_manager.Project_Dir.toStdString());
        CommandLineExecutor::Build(cmake, file_manager.Project_Dir.toStdString(), console_dock->ConsoleOutput);

        // non static function need also an object to call it

    } else {
        std::vector<std::string> sources;
        sources.reserve(10);
        for (int i = 0; i < file_manager.source_files.size(); i++) {
            sources.push_back(file_manager.source_files[i].toStdString());
        }
    }

    QSettings settings("Evolution");
    settings.setValue("Evolution/executable_path/", file_manager.Project_Dir + "/executable");

    console_dock->ConsoleOutput->appendPlainText("Build done");
    qDebug() << "build done";
}

void MainWindow::slotRun() {

    // clear terminal window
    //console_dock->slotClearConsole();

    bool cmake = true;
    if (CHANGES_IN_PROJECT) {
        slotBuild();
    }

    // TODO: if currentWidget has "" filepath -> dry run, set up temp dir for only 1 file

    if (cmake) {
        // file_manager.executable_file_path;
        CommandLineExecutor::Execute(cmake, file_manager.Project_Dir.toStdString() + "/cmake-build/" + "executable", console_dock->ConsoleOutput);
    } else {
        //executor.setExecutableName("executable", file_manager.Project_Dir.toStdString());
        //int pid = executor.getPid();
        //QString process = QString("launched ") + QString("executable,  PID:") + QString::number(pid);
        //console_dock->setRawOutput(process);
    }
}
void MainWindow::slotClangFormat() {
    CommandLineExecutor executor;
    std::vector<std::string> sources;
    console_dock->ConsoleOutput->appendPlainText(QString::fromStdString(CommandLineExecutor::ClangFormat(sources, file_manager.clang_format_path.toStdString())));
}
void MainWindow::slotClangTidy() {
    CommandLineExecutor executor;
    std::vector<std::string> sources;
    console_dock->ConsoleOutput->appendPlainText(QString::fromStdString(CommandLineExecutor::ClangTidy(sources)));
}
void MainWindow::slotClangCheck() {
    CommandLineExecutor executor;
    std::vector<std::string> sources;
    console_dock->ConsoleOutput->appendPlainText(QString::fromStdString(CommandLineExecutor::ClangCheck(sources)));
}
void MainWindow::slotValgrind() {
    CommandLineExecutor executor;
    console_dock->ConsoleOutput->appendPlainText(QString::fromStdString(CommandLineExecutor::Valgrind(std::string())));
}
void MainWindow::slotClangDocGenerate() {
    // clang-doc
}
void MainWindow::slotGdbGui() {
    CommandLineExecutor::OpenGdbGui(std::string());
}


void MainWindow::UpdateCurrentIndexOnDelete(int) {
    /* (Relies on fact that after deletion current tab is always (count() - 1)th tab) */
    Docker->DockerFileList->setCurrentRow(Docker->DockerFileList->count() - 1);
}

// text operations
void MainWindow::slotCopy() {
    currentWidget->copy();
}

void MainWindow::slotCut() {
    currentWidget->cut();
}

void MainWindow::slotUndo() {
    currentWidget->undo();
}
void MainWindow::slotRedo() {
    currentWidget->redo();
}

void MainWindow::slotSelectAll() {
    currentWidget->selectAll();
}

void MainWindow::slotPaste() {
    currentWidget->paste();
}

void MainWindow::slotRemoveAll() {
    currentWidget->clear();
}

void MainWindow::slotExpand() {
    currentWidget->slotExpand();
}

void MainWindow::slotCollapse() {
    currentWidget->slotCollapse();
}

void MainWindow::slotToggleComment() {
    currentWidget->toggleComment();
}

void MainWindow::slotFormat() {
}


void MainWindow::slotFullScreen() {
    if (isFullScreen()) {
        showNormal();
    } else {
        showFullScreen();
    }
    // exit also with ESC
}

void MainWindow::slotAbout() {
    const QString about = "Evolution IDE is an Integrated Development Environment mainly aimed for C & C++ . \n"
                          "It is Open source. If you Want to use It, you can read Readme.md file with all features \n"
                          "\n"
                          "Newly contain Educational system, which could provide convenient code samples with brief \n"
                          "comments. There are also provided many stuffs i personally always want to know quicker. \n"
                          "If you are interested, you can provide your own sample. \n"
                          "Web:  \n"
                          "Github: https://github.com/adamko222/Evolution-IDE. \n";

    QMessageBox::information(this, "About Evolution", about, QMessageBox::Close);
}

void MainWindow::slotStopProcess() {
    // executable is in registers -> no need to set it here, will be called in function
    int pid = CommandLineExecutor::getPid(std::string());
    if (pid != 0) {
        CommandLineExecutor::killProcess(0);
        console_dock->ConsoleOutput->appendPlainText("Process killed , PID: " + QString::number(pid));
    } else {
        console_dock->ConsoleOutput->appendPlainText("No Process attached");
    }
}

void MainWindow::slotStartDebug() {
    if (!debuggerView->isVisible()) {
        showDebuggerView();
        // wait some time
        // unistd.h   -> linux header
        sleep(1);
        debuggerView->start();
    } else {
        debuggerView->start();
    }
}

void MainWindow::slotStopDebug() {
    debuggerView->slotStopDebug();
}

void MainWindow::slotContinue() {
    debuggerView->slotStopDebug();
}

void MainWindow::slotRunToCursor() {
    debuggerView->slotRunToCursor();
}

void MainWindow::slotStepOver() {
    debuggerView->slotStepOver();
}

void MainWindow::slotStepInto() {
    debuggerView->slotStepInto();
}

void MainWindow::slotStepInstruction() {
    debuggerView->slotStepInstruction();
}

void MainWindow::slotStepOut() {
    debuggerView->slotStepOut();
}

// this might go only through containBlock and decide to create or remove
void MainWindow::slotToggleBreakPoint() {
    QString filename = currentWidget->getFilePath();
    int line = currentWidget->getCursorPosition().x();
    // in edit
    bool created = currentWidget->toggleBreakPoint(line);


    // in debugger
    if (!filename.isEmpty()) {
        // created
        if (created) {
            debuggerView->createBreakpoint(filename.toLatin1().data(), line);
        }
        // removed
        else {
            debuggerView->removeBreakpoint(filename.toLatin1().data(), line);
        }
    }
}

void MainWindow::slotCreateBreakPoint(const int &line) {
    QString filename = currentWidget->getFilePath();
    if (!filename.isEmpty()) {
        debuggerView->createBreakpoint(filename.toLatin1().data(), line);
    }
}

void MainWindow::slotDeleteBreakPoint(const int &line) {
    QString filename = currentWidget->getFilePath();
    if (!filename.isEmpty()) {
        debuggerView->removeBreakpoint(filename.toLatin1().data(), line);
    }
}

void MainWindow::slotSetBreakpointAtLine() {
    QString filename = currentWidget->getFilePath();
    int line = currentWidget->getCursorPosition().x();

    // in debugger
    if (!filename.isEmpty()) {
        // in edit
        bool created = currentWidget->toggleBreakPoint(line);
        // created
        if (created) {
            debuggerView->showSetManualBreakPoint(filename);
        }
        // removed meaningless
    }
}

void MainWindow::slotShowBreakpointsList() {
    debuggerView->showBreakPointsList();
}

void MainWindow::slotShowAttachToProcess() {
    debuggerView->showTaskManager();
}

void MainWindow::slotRestart() {
    // find how to
}

void MainWindow::slotOpenCppSample(QListWidgetItem *item) {
    // represents sample to take
    int index = item->listWidget()->currentRow();
    int num_files = education->cpp_code_samples[index].fileNames.size();

    if (num_files == 1) {
        auto *edit = new PlainTextEdit;
        edit->setPlainText(education->cpp_code_samples[index].content[0]);// 0 -> only 1 file
        Tabs->addTab(edit, education->cpp_code_samples[index].fileNames[0]);
        Tabs->setCurrentIndex(Tabs->count());// since we add new tab at the end
        // ensure that we can also make all functions accessible from sample
        currentWidget = edit;
    }
    // 2 and more files
    if (num_files > 1) {
        for (int i = 0; i <= num_files; i++) {
            auto *edit = new PlainTextEdit;
            edit->setPlainText(education->cpp_code_samples[index].content[i]);
            Tabs->addTab(edit, education->cpp_code_samples[index].fileNames[i]);
            Tabs->setCurrentIndex(Tabs->count());// since we add new tab at the end
            currentWidget = edit;
        }
    }

    // set icon that it was opened + add its index into registry
    //item->setIcon(QIcon(IconFactory::Done));

    // also solve current widget in tab here !!!
    // take care of opening the same sample twice and more
}

void MainWindow::slotOpenCppUserSample(QListWidgetItem *item) {
    int index = item->listWidget()->currentRow();
    QString content = education->cpp_user_samples[index];

    auto *edit = new PlainTextEdit;
    edit->setPlainText(content);
    Tabs->addTab(edit, item->text());
    Tabs->setCurrentIndex(Tabs->count());// since we add new tab at the end
    currentWidget = edit;
}