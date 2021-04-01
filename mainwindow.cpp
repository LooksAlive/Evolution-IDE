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

    highlighter = new Highlighter(":/highlights/languages.xml", this);

    /* set all necessary widgets, features etc. */
    SetupTabWidget();
    SetupFileExplorer();
    SetupFileDocker();
    SetupCompileDock();
    SetupCodeInfoDock();
    SetupEducationDock();
    SetupGitDock();

    SetupNodeView();
    SetupDebuggerView();
    SetupBinaryView();
    SetupHexView();
    SetupVerticalBar();

    SetupMenuBar();
    SetupToolBar();

    //CreateFile();
    SetupStatusBar();
    SetupTagsReminder();

    SetupDockWidgetsLayering();

    setCentralWidget(vertical_stack);
    // Tabs->setFocus();
    // vertical_stack->setCurrentWidget(Tabs);
    // equivalent to :
    showEditorView();
    // showInvitationScreen();

    //Tabs->currentWidget()->setFocus();
    executor = new CommandLineExecutor(this);

    LoadRegisters();
}

MainWindow::~MainWindow() {
    //delete ui;
}

/* drag and drop functions for file into window as it would be opened */
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

    const QRect rect = settings.value("Evolution/MainWindowGeometry").toRect();
    if (!rect.isEmpty()) {
        setGeometry(rect);
    }


    const QStringList opened_tabs = settings.value("Evolution/opened_tabs").toStringList();
    // FIXME: getting position (int) from list bc. no idea how to set QList<int> to registers
    const QStringList positions = settings.value("Evolution/opened_tabs_cursor_positions").toStringList();
    if (!opened_tabs.isEmpty()) {
        for (int i = 0; i < opened_tabs.size(); i++) { // one error, default tab has no path
            if (OpenFile(opened_tabs[i])) {
                QTextCursor cursor = currentWidget->textCursor();
                cursor.setPosition(positions[i].toInt());
                currentWidget->setTextCursor(cursor);
            }
            // TODO: status to inform user that read failed, or just let it be, file might have been removed by user...
        }
    }

    // invitanion screen is on ...
    showEditorView();       // switch from Invitation Screen which is default


    const QString Project_Root_Dir = settings.value("Evolution/Project_Root_Dir").toString();
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

    vertical_bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    vertical_bar->setFocusPolicy(Qt::ClickFocus);

    vertical_stack = new QStackedWidget(this);
    vertical_stack->setParent(this);

    vertical_stack->insertWidget(0, Tabs);
    vertical_stack->insertWidget(1, nodeView);
    vertical_stack->insertWidget(2, hexView);
    vertical_stack->insertWidget(3, binaryView);
    vertical_stack->insertWidget(4, Tabs);             // debuggerBridge
    vertical_stack->insertWidget(5, invitation_screen);// helpful, do not belong to this stack, but is there

    vertical_stack->setCurrentIndex(0);// start with editor

    // FIXME: show as text or tooltip ????

    editorViewButton = new QToolButton(vertical_bar);
    editorViewButton->setCheckable(true);
    editorViewButton->setChecked(false);
    editorViewButton->setIcon(QIcon(IconFactory::EditorView));
    editorViewButton->setText("Editor");
    connect(editorViewButton, SIGNAL(clicked()), this, SLOT(showEditorView()));

    nodeViewButton = new QToolButton(vertical_bar);
    nodeViewButton->setCheckable(true);
    nodeViewButton->setChecked(false);
    nodeViewButton->setIcon(QIcon(IconFactory::NodeView));
    nodeViewButton->setText("Node View");
    connect(nodeViewButton, SIGNAL(clicked()), this, SLOT(showNodeView()));

    hexViewButton = new QToolButton(vertical_bar);
    hexViewButton->setCheckable(true);
    hexViewButton->setChecked(false);
    hexViewButton->setIcon(QIcon(IconFactory::HexView));
    hexViewButton->setText("Hex Editor");
    connect(hexViewButton, SIGNAL(clicked()), this, SLOT(showHexView()));

    debuggerViewButton = new QToolButton(vertical_bar);
    debuggerViewButton->setCheckable(true);
    debuggerViewButton->setChecked(false);
    debuggerViewButton->setIcon(QIcon(IconFactory::DebuggerView));
    debuggerViewButton->setText("Debugger");
    connect(debuggerViewButton, SIGNAL(clicked()), this, SLOT(showDebuggerView()));

    binaryViewButton = new QToolButton(vertical_bar);
    binaryViewButton->setCheckable(true);
    binaryViewButton->setChecked(false);
    binaryViewButton->setIcon(QIcon(IconFactory::BinaryView));
    binaryViewButton->setText("Binary View");
    connect(binaryViewButton, SIGNAL(clicked()), this, SLOT(showBinaryView()));

    vertical_bar->addWidget(editorViewButton);
    vertical_bar->addSeparator();
    vertical_bar->addWidget(nodeViewButton);
    vertical_bar->addSeparator();
    vertical_bar->addWidget(hexViewButton);
    vertical_bar->addSeparator();
    vertical_bar->addWidget(debuggerViewButton);
    vertical_bar->addSeparator();
    vertical_bar->addWidget(binaryViewButton);
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
    current_function = new QLabel(this);
    current_file_path = new QLabel(this);
    locate_open_file = new QLineEdit(this);
    locate_completer = new QCompleter(this);
    btn_comment_tags = new QToolButton(this);

    statusbar->layout()->setContentsMargins(0, 0, 0, 0);
    statusbar->setFixedHeight(25);
    statusbar->setMouseTracking(true);
    btn_encoding->setText("UTF-8");
    btn_encoding->setFixedWidth(50);
    btn_encoding->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    btn_encoding->setToolTip("Encoding");
    btn_position->setFixedWidth(70);
    btn_position->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    btn_position->setToolTip("Position");
    connect(btn_position, SIGNAL(clicked()), this, SLOT(slotGoToLine()));
    btn_comment_tags->setFixedWidth(70);
    btn_comment_tags->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    btn_comment_tags->setText("Tags");
    btn_comment_tags->setToolTip("Show comment tags");
    connect(btn_comment_tags, &QToolButton::clicked, this, [=]() { tagReminder->show(); });
    current_function->setText("current function   ");
    locate_open_file->setCompleter(locate_completer);
    // locate_open_file->setMinimumWidth(100);
    locate_open_file->setMaximumWidth(200);
    locate_open_file->setPlaceholderText("look up file");
    locate_completer->setModel(new QStringListModel(file_manager.source_files_names, locate_completer));
    locate_completer->setMaxVisibleItems(12);
    locate_completer->setCompletionMode(QCompleter::PopupCompletion);
    locate_completer->setFilterMode(Qt::MatchContains);
    connect(locate_completer, SIGNAL(activated(const QString &)), this, SLOT(slotOpenLocateFile(const QString &)));

    statusbar->setWindowFlags(Qt::FramelessWindowHint);
    statusbar->setAttribute(Qt::WA_NoSystemBackground, true);
    statusbar->setAttribute(Qt::WA_TranslucentBackground, true);

    // statusbar->showMessage("loading", 5000);
    statusbar->addWidget(locate_open_file); // to left
    statusbar->addWidget(btn_comment_tags);
    statusbar->addPermanentWidget(current_file_path);
    statusbar->addPermanentWidget(current_function);
    statusbar->addPermanentWidget(btn_position);
    statusbar->addPermanentWidget(btn_encoding);
    statusbar->addPermanentWidget(progress_bar);// align right with permanent widget
    setStatusBar(statusbar);
}

void MainWindow::slotOpenLocateFile(const QString &filepath) {
    for (const auto &path : file_manager.source_files) {
        if (QFileInfo(path).fileName() == filepath) {
            OpenFile(path);
            return;
        }
    }
}

// after line is set , field is empty
void MainWindow::slotCursorPositionChanged() {
    // later consider creating new button here
    // What if on the changed tab is no cursor set -> remain last cursor position
    if (Tabs->isVisible()) {
        const QPoint point = currentWidget->getCursorPosition();
        const QString pos = QString::number(point.y()) + ":" + QString::number(point.x());// row:col

        btn_position->setText(pos);
    } else {
        btn_position->setText("");
    }
}

void MainWindow::slotGoToLine() {
    auto *goTo = new GoToLineColumn(currentWidget, this);
    goTo->show();
    slotCursorPositionChanged();// update  position
}

void MainWindow::SetupMenuBar() {

    menuBar = new QMenuBar(this);
    menuBar->setWindowTitle("Menu bar");
    //menuBar->setObjectName("Menubar");
    //menuBar->setGeometry(QRect(0, 0, 1000, 24));
    //menuBar->setFixedHeight(24);    // or none, still work well
    menuBar->setMouseTracking(true);
    //menuBar->setNativeMenuBar(false);

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
    fileMenu->addAction("Restart", this, SLOT(slotRestart()), Qt::CTRL + Qt::SHIFT + Qt::Key_R);
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
    viewMenu->addAction(Linter->toggleViewAction());
    viewMenu->addAction(Refactor->toggleViewAction());
    viewMenu->addAction(gitDock->toggleViewAction());
    viewMenu->addAction(debuggerDock->toggleViewAction());
    viewMenu->addAction(debuggerWatchDock->toggleViewAction());
    viewMenu->addSeparator();
    viewMenu->addAction(QIcon(IconFactory::FullScreen), "Full Screen", this, SLOT(slotFullScreen()), Qt::Key_F11);
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
    contactDeveloper = new ContactDeveloper(this);
    HelpMenu->addAction("Contact Developer", this, [=]() { contactDeveloper->show(); });

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
    topToolBar->addSeparator();

    searchBox = new SearchBox(Tabs, this);
    searchBox->setFixedHeight(topToolBar->height());
    connect(searchBox->more, SIGNAL(clicked()), this, SLOT(slotShowFindReplaceDock()));
    //topToolBar->addWidget(searchBox);

    auto *spacer = new QWidget(this);// blank Widget to align other action to right
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    topToolBar->addWidget(spacer);
    topToolBar->addAction(QIcon(IconFactory::Build), "Build", this, SLOT(slotBuild()));
    topToolBar->addAction(QIcon(IconFactory::Run), "Run", this, SLOT(slotRun()));
    topToolBar->addAction(QIcon(IconFactory::Stop), "Stop Process", this, SLOT(slotStopProcess()));
    // TODO: open git manager dock, to certain sections, update its data, status bar...
    topToolBar->addAction(QIcon(IconFactory::Pull), "Pull", this, SLOT(slotPullRequest()));
    topToolBar->addAction(QIcon(IconFactory::Push), "Commit", this, SLOT(slotCommitRequest()));
    topToolBar->addAction(QIcon(IconFactory::Commit), "Push", this, SLOT(slotPushRequest()));
}

void MainWindow::slotShowFindReplaceDock() {
    find_replace->setVisible(true);
    find_replace->LineEditFind->setText(searchBox->lineEdit->text());
    // should also look up -> to fill list of results, bc. they are already selected or not (if user pressed find next)
    // let it be like this,, but at least lets set focus
    find_replace->LineEditFind->setFocus();
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
    uncheckAllVerticalTabButtons();
    editorViewButton->setChecked(true);

    // empty tab has count -1 probably :)  --> also show only invitation, not empty tab !
    if (Tabs->count() <= 0) {
        // invitation screen is still ON, or we could move from somewhere else, to be sure
        showInvitationScreen();
        debuggerDock->hide();
        debuggerWatchDock->hide();
        ShowHiddenDockWidgets();
        return;
    }

    ShowHiddenDockWidgets();
    // might be visible anyway, if debug view was...
    debuggerDock->hide();
    debuggerWatchDock->hide();
    vertical_stack->setCurrentWidget(Tabs);
}

void MainWindow::showNodeView() {
    uncheckAllVerticalTabButtons();
    nodeViewButton->setChecked(true);

    // Will there be some dock visible ???
    HideAllDockWidgets();
    vertical_stack->setCurrentWidget(nodeView);
}

void MainWindow::showBinaryView() {
    uncheckAllVerticalTabButtons();
    binaryViewButton->setChecked(true);

    HideAllDockWidgets();
    vertical_stack->setCurrentWidget(binaryView);
}

void MainWindow::showDebuggerView() {
    uncheckAllVerticalTabButtons();
    debuggerViewButton->setChecked(true);

    HideAllDockWidgets();
    debuggerDock->setVisible(true);
    debuggerWatchDock->setVisible(true);

    vertical_stack->setCurrentWidget(Tabs);// debuggerBridge

    //QString path = currentWidget->getFilePath();
    // for now, starting with current file, later track    ;  not i only care for line
    // file_manager.current_full_filepath, 0
    //file_manager.executable_file_path = "/home/adam/Desktop/SKK/cmake-build/executable";
    //debuggerBridge->setStartPosition(path.toLatin1().data(), currentWidget->getCursorPosition().y());
    //debuggerView->setExecutable(file_manager.executable_file_path.toStdString());
}

void MainWindow::showHexView() {
    uncheckAllVerticalTabButtons();
    hexViewButton->setChecked(true);

    if (Tabs->count() == 0) {
        // invitation screen is still ON, create some file first
        return;
    }
    HideAllDockWidgets();

    // Tabs->tabToolTip(Tabs->currentIndex());
    QString path = currentWidget->getFilePath();
    if (path != "") {
        hexView->open(path);
        vertical_stack->setCurrentWidget(hexView);
    } else {
        // not saved file, get data and convert right :)
        QString data = currentWidget->toPlainText();
        hexView->setText(data.toUtf8());
        vertical_stack->setCurrentWidget(hexView);
    }
}

void MainWindow::showInvitationScreen() {
    // HideAllDockWidgets();
    vertical_stack->setCurrentWidget(invitation_screen);
}

void MainWindow::uncheckAllVerticalTabButtons() {
    editorViewButton->setChecked(false);
    nodeViewButton->setChecked(false);
    hexViewButton->setChecked(false);
    debuggerViewButton->setChecked(false);
    binaryViewButton->setChecked(false);
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
    // want to show only in debug view manually
    debuggerDock->hide();
    debuggerWatchDock->hide();
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

    invitation_screen = new InvitationScreen(this);
}


void MainWindow::SetupFileExplorer() {
    Explorer = new FileExplorer(this);
    // Explorer->setRootDirectory(QDir::homePath());

    connect(Explorer->FileView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(OpenFile(const QModelIndex &)));
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

    connect(Docker->DockerFileList, SIGNAL(itemClicked(QListWidgetItem * )), this,
            SLOT(UpdateCurrentIndex(QListWidgetItem * )));
    connect(Docker->DockerFileList, SIGNAL(currentRowChanged(int)), Tabs, SLOT(setCurrentIndex(int)));
    addDockWidget(Qt::LeftDockWidgetArea, Docker);
}

void MainWindow::SetupTagsReminder() {
    tagReminder = new CommentTagsReminder(this);
    tagReminder->setSources(file_manager.source_files);
    tagReminder->searchEverywhere();

    connect(tagReminder->view, SIGNAL(itemDoubleClicked(QTreeWidgetItem * , int)), this,
            SLOT(openCommentTag(QTreeWidgetItem * , int)));
}

void MainWindow::openCommentTag(QTreeWidgetItem *item, int column) {
    Q_UNUSED(column);
    OpenFile(item->toolTip(0));

    const int line = item->text(2).toInt();
    currentWidget->setCursorAtLine(line);
}

void MainWindow::SetupCompileDock() {
    console_dock = new ConsoleDock(this);

    connect(console_dock->run, SIGNAL(clicked()), this, SLOT(slotRun()));
    connect(console_dock->rerun, SIGNAL(clicked()), this, SLOT(slotRun()));
    connect(console_dock->stop, SIGNAL(clicked()), this, SLOT(slotStopProcess()));

    // exteranals links
    connect(console_dock->ConsoleOutput, SIGNAL(anchorClicked(const QUrl &)), this, SLOT(slotOpenUrl(const QUrl &)));

    connect(new QShortcut(Qt::CTRL + Qt::Key_F, this), &QShortcut::activated, [=] { slotFind(); });

    addDockWidget(Qt::BottomDockWidgetArea, console_dock);
}

void MainWindow::slotOpenUrl(const QUrl &url) {
    const QString filepath = url.url(QUrl::RemoveScheme);
    const ConsoleDock::Link link = console_dock->findLink(filepath);

    // at the end we return Links{}  -> check it for any element
    if (link.filePath.isEmpty()) {
        return;
    }

    OpenFile(filepath);
    currentWidget->setCursorPosition(link.position.x(), link.position.y());
}

void MainWindow::slotFind() {
    if (!currentWidget) {
        return;
    }
    // top left corner
    // FIXME: when dock is shown on the left side or upside update this rectangle + how to handle moving docks ?
    searchBox->setGeometry(centralWidget()->geometry().x() + currentWidget->lineNumberArea->width(),
                           centralWidget()->geometry().y() + Tabs->tabBar()->height(), 300, 70);
    searchBox->show();
    searchBox->lineEdit->setFocus();
    QString text = currentWidget->textCursor().selectedText();
    // find selected text
    if (!text.isEmpty()) {
        searchBox->lineEdit->setText(text);
        searchBox->slotNext();
    }
        // find word under cursor, if no text is selected
    else {
        text = currentWidget->getWordUnderCursor();
        searchBox->lineEdit->setText(text);
        searchBox->slotNext();
    }
    /*
    searchBox->move(vertical_bar->width(), menuBar->height() + topToolBar->height() + Tabs->height());
    searchBox->setStyleSheet("QWidget{border: 2px solid; background-color: grey;}");
    searchBox->setVisible(true);
    */
}

void MainWindow::slotOpenHoverInfoUrl(const QUrl &url) {
    const QString text = url.url(QUrl::None);
    // filepath:posx:posy
    const int posStart = text.indexOf(":");
    const QString filepath = text.mid(0, posStart);

    const int x = text.mid(0, text.indexOf(":", posStart)).toInt();
    const int y = text.mid(0, text.indexOf(posStart)).toInt();

    OpenFile(filepath);
    currentWidget->setCursorPosition(x, y);
}

void MainWindow::SetupCodeInfoDock() {
    clangBridge = new ClangBridge();
    find_replace = new FindReplaceWidget(Tabs, this);
    Linter = new LinterDock(this);
    Refactor = new RefactoringDock(this);
    codeInfoDock = new CodeInfoDock(this);

    find_replace->setFiles(file_manager.source_files);
    addDockWidget(Qt::BottomDockWidgetArea, find_replace);
    tabifyDockWidget(console_dock, find_replace);
    tabifyDockWidget(codeInfoDock, find_replace);

    addDockWidget(Qt::BottomDockWidgetArea, Linter);
    tabifyDockWidget(Linter, codeInfoDock);
    tabifyDockWidget(Linter, find_replace);

    addDockWidget(Qt::BottomDockWidgetArea, Refactor);
    tabifyDockWidget(Refactor, codeInfoDock);
    tabifyDockWidget(Refactor, find_replace);
    tabifyDockWidget(Linter, Refactor);

    addDockWidget(Qt::BottomDockWidgetArea, codeInfoDock);
    tabifyDockWidget(console_dock, codeInfoDock);
    codeInfoDock->setEditor(currentWidget);
    codeInfoDock->setClang(clangBridge);
    codeInfoDock->setSearch(find_replace);
    codeInfoDock->setLinter(Linter);
    codeInfoDock->setRefactor(Refactor);

    // connect externals windows requests
    connect(codeInfoDock->Linter->Items, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotOpenLinterFile(QListWidgetItem *)));
    // file opening
    connect(find_replace->results, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(slotOpenReferenceFile(QTreeWidgetItem *, int)));
}

void MainWindow::slotOpenLinterFile(QListWidgetItem *item) {
    const QString filepath = item->toolTip();// this should be file path, but we also want position
    // ->text holds position, linter text, etc.
    // match position, by file
    const int row = item->listWidget()->currentIndex().row();
    // const QString filepath = QString::fromStdString(clangBridge->PData->Diags[0].AbsFile.getValue());
    OpenFile(filepath);
    currentWidget->setCursorPosition(codeInfoDock->Handler->linterLocations[0].row,
                                     codeInfoDock->Handler->linterLocations[0].col);
}

void MainWindow::slotOpenReferenceFile(QTreeWidgetItem *item, int column) {
    if (!item->parent()) {
        return;
    }
    // we were searching only in current file
    if (find_replace->inCurrentFile->isChecked()) {
        find_replace->forwardToResult(item, column);
        return;
    }
    // we have to figure out in what file result is, if there are more of them
    // doubleClick, but click were before, so we have temporary variables already set
    if (find_replace->temp_search_result_path.isEmpty()) {
        qDebug() << "find_replace->temp_search_result_path is empty !!! line 584 MainWindow";
    }

    // if we do not have (nullptr), we cant do this, continue and add one, old way
    if (currentWidget) {
        // const QString filepath = item->parent()->toolTip(0); // TOOLTIP is filepath, not text
        if (find_replace->temp_search_result_path == currentWidget->getFilePath()) {
            currentWidget->setCursorPosition(find_replace->temp_pos.x(), find_replace->temp_pos.y());
        }
    }

    // user changed file
    find_replace->savePreview();

    OpenFile(find_replace->temp_search_result_path, true);// we will set document from preview to share memory
    find_replace->preview->setDocument(currentWidget->document());
    // currentWidget->setDocument(find_replace->preview->document());
    currentWidget->extra_selections_search_results = find_replace->selections[find_replace->tempSelectionPos];
    currentWidget->updateExtraSelections();
    find_replace->preview->updateExtraSelections();
    currentWidget->setCursorPosition(find_replace->temp_pos.x(), find_replace->temp_pos.y());

    /*
    // references from code
    if(!find_replace->titleBarWidget()->isEnabled()){
        // row holds file path and column position reference in that file
        const std::string filepath = codeInfoDock->Handler->referencesLocation[row].filepath;
        // match filepath with codeInfoDock->Handler->ReferencesLocation.filepath
        // get index and find ...
        const int posIndex = 0;
        const int row = codeInfoDock->Handler->referencesLocation[posIndex].row;
        const int col = codeInfoDock->Handler->referencesLocation[posIndex].col;

        OpenFile(QString::fromStdString(filepath));

        currentWidget->setCursorPosition(row, col);
    }
    */
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
    connect(education->CppUsersSamples, SIGNAL(itemDoubleClicked(QListWidgetItem * )), this,
            SLOT(slotOpenCppUserSample(QListWidgetItem * )));
    // should there be a disconnection when main window is destructed ?
}

void MainWindow::SetupGitDock() {
    gitDock = new GitDock();
    addDockWidget(Qt::RightDockWidgetArea, gitDock);
}

void MainWindow::SetupNodeView() {
    nodeView = new NodeView(this);
    nodeView->setClangBridge(clangBridge);
    nodeView->setCurrentWidget(currentWidget);
    nodeView->setCodeInfo(codeInfoDock);

    connect(nodeView, SIGNAL(openFile(const QString&, const int&)), this,
            SLOT(slotOpenFileFromNode(const QString&, const int&)));
}

void MainWindow::slotOpenFileFromNode(const QString &filepath, const int &line) {
    if (OpenFile(filepath)) {
        currentWidget->setCursorAtLine(line);
    }
}

void MainWindow::SetupDebuggerView() {
    debuggerDock = new DebuggerDock(this);
    addDockWidget(Qt::BottomDockWidgetArea, debuggerDock);
    tabifyDockWidget(debuggerDock, find_replace);
    tabifyDockWidget(debuggerDock, console_dock);
    tabifyDockWidget(debuggerDock, codeInfoDock);

    debuggerWatchDock = new DebugWatchDock(this);
    addDockWidget(Qt::RightDockWidgetArea, debuggerWatchDock);

    debuggerBridge = new lldbBridge(debuggerDock, debuggerWatchDock, this);
    debuggerBridge->setProjectFilePaths(file_manager.source_files);

    connect(debuggerBridge, SIGNAL(filePathUpdate(const QString &, const int &, const int &)), this,
            SLOT(slotUpdateDebuggerFilePath(const QString &, const int &, const int &)));
}

void MainWindow::slotUpdateDebuggerFilePath(const QString &filepath, const int &row, const int &col) {
    AssemblyLoading = true;
    OpenFile(filepath);
    AssemblyLoading = false;
    // set position
    currentWidget->setCursorPosition(col, row);

    // since we go from debugger, set line selection for our line to BreakPoint type
    currentWidget->setLineSelection(row, PlainTextEdit::BreakPoint);
}

void MainWindow::SetupBinaryView() {
    binaryView = new BinaryView(this);
}
void MainWindow::SetupHexView() {
    hexView = new HexView(this);
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
    currentWidget->setClang(clangBridge);
    currentWidget->setTagReminder(tagReminder);
    currentWidget->setNodeView(nodeView);

    // go to line/column
    connect(currentWidget, SIGNAL(cursorPositionChanged()), this, SLOT(slotCursorPositionChanged()));
    connect(currentWidget, SIGNAL(textChanged()), this, SLOT(UpdateParameter()));
    connect(currentWidget->hoverInfo->MainWidget, SIGNAL(anchorClicked(const QUrl &)), this,
            SLOT(slotOpenHoverInfoUrl(const QUrl &)));
    // breakpoints
    // TODO: if i pass debugger widget by instance pointer, will it be faster ??
    connect(currentWidget->breakPointArea, SIGNAL(breakPointCreated(const int &)), this,
            SLOT(slotCreateBreakPoint(const int &)));
    connect(currentWidget->breakPointArea, SIGNAL(breakPointRemoved(const int &)), this,
            SLOT(slotDeleteBreakPoint(const int &)));

    // file dock
    auto *new_item = new QListWidgetItem;
    new_item->setText(Tabs->tabText(index));
    new_item->setToolTip(Tabs->tabToolTip(index));
    // new_item->setIcon(QIcon(IconFactory::Remove));
    Docker->DockerFileList->addItem(new_item);

    UpdateCurrentIndex(index);

    // we definitelly have a new tab
    showEditorView();// switch from Invitation Screen which is default
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

bool MainWindow::OpenFile(const QString &filepath, const bool &readAndSetDocument) {

    // directory operations
    if (QFileInfo(filepath).isDir()) {
        // set project path into file view
        Explorer->setRootDirectory(filepath);
        // set recursively all files in dir, set root project dir
        file_manager.getFilesRecursively(filepath);
        return false;
    }
    QString content;
    PlainTextEdit *new_text_edit;

    if (!QFileInfo(filepath).exists()) {
        if (SampleLoading) {
            content = education->WorkingContent;
            // all sample are not editable, but enabled, free to copy
            new_text_edit = new PlainTextEdit();
            new_text_edit->setPlainText(content);
            new_text_edit->setReadOnly(true);
            file_manager.current_file_name = filepath;
            file_manager.current_full_filepath = filepath;
            goto end;
        }
        if (AssemblyLoading) {
            content = debuggerBridge->WorkingContent;
            new_text_edit = new PlainTextEdit();
            new_text_edit->setPlainText(content);
            new_text_edit->setReadOnly(true);
            file_manager.current_file_name = filepath;
            file_manager.current_full_filepath = filepath;
            goto end;
        } else {
            // not valid file path, no more exceptions
            return false;
        }
    } else {
        new_text_edit = new PlainTextEdit();
        if (readAndSetDocument) {
            content = file_manager.read(filepath);
            new_text_edit->setPlainText(content);
        } else {
            new_text_edit->setPlainText("");
        }
    }
// handy goto statement to escape nested loop but not return;
end:;

    new_text_edit->setFilePath(filepath);
    new_text_edit->setFileExtension(file_manager.getFileExtension(filepath));

    // check for duplicate file-opening and prevents it by opening identical tab twice
    for (int i = 0; i < Tabs->count(); ++i)
        if (Tabs->tabToolTip(i) == filepath) {
            Tabs->setCurrentIndex(i);
            return false;
        }

    // ????????????????????????????????????????????????????????
    if (new_text_edit->document()->isEmpty() &&
        Tabs->tabToolTip(Tabs->currentIndex()) == "" &&
        Tabs->tabText(Tabs->currentIndex()) == NEW_TAB_NAME) {

        DeleteTabFromList(Tabs->currentIndex());
        delete Tabs->widget(Tabs->currentIndex());
    }


    // tab
    // icons for tab, like in file view
    QFileIconProvider provider;
    int index = Tabs->addTab(new_text_edit, provider.icon(QFileInfo(filepath)), QFileInfo(filepath).fileName());
    Tabs->setCurrentIndex(index);
    Tabs->setTabToolTip(index, filepath);
    Tabs->setTabWhatsThis(index, "No changes");
    connect(new_text_edit, SIGNAL(textChanged()), this, SLOT(UpdateParameter()));

    // file Docker
    auto *new_item = new QListWidgetItem();
    new_item->setText(Tabs->tabText(index));
    new_item->setToolTip(Tabs->tabToolTip(index));
    new_item->setIcon(provider.icon(QFileInfo(filepath)));
    // new_item->setIcon(QIcon(IconFactory::Remove));
    Docker->DockerFileList->addItem(new_item);

    // setting up highlight
    if (highlighter->setExtension(file_manager.getFileExtension(filepath))) {
        highlighter->setDocument(new_text_edit->document());
        highlighter->highlightBlock(new_text_edit->toPlainText());
    }

    Tabs->setTabWhatsThis(index, "No changes");
    UpdateCurrentIndex(index);// setting up selected item in opened_docs_dock

    // we definitely have a new tab
    showEditorView();// switch from Invitation Screen which is default
    return true;
}


void MainWindow::SaveFile() {
    // nothing to save
    if (!Tabs->count()) {
        return;
    }
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
    // nothing to save
    if (!Tabs->count()) {
        return;
    }

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
    disconnect(currentWidget->breakPointArea, SIGNAL(breakPointCreated(const int &)), this,
               SLOT(slotCreateBreakPoint(const int &)));
    disconnect(currentWidget->breakPointArea, SIGNAL(breakPointRemoved(const int &)), this,
               SLOT(slotDeleteBreakPoint(const int &)));
    // go to line/column
    disconnect(currentWidget, SIGNAL(cursorPositionChanged()), this, SLOT(slotCursorPositionChanged()));
    delete Tabs->widget(index_);
    DeleteTabFromList(index_);

    if (!Tabs->count()) {
        // CreateFile();
        showInvitationScreen();
        currentWidget = nullptr;

        // clear status and all other
        current_file_path->clear();
        current_function->clear();
        btn_encoding->setText(QString());
        btn_position->setText(QString());

        return;// tab is not active, (code below)
    }
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

    // while (Tabs->count() > 0)
    // delete Tabs->widget(0);
    Tabs->clear();
    // FIXME: ************************************************************************************************
    // close file one by one bot like this...

    Docker->DockerFileList->clear();

    showInvitationScreen();
    // CreateFile();
    // Tabs->currentWidget()->setFocus();
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
    QStringList tabs_cursor_positions;
    for (int i = 0; i <= Tabs->count(); i++) {       // < or <= ?????????????????
        // default tab has no path, since is not saved
        // Tabs->tabText(i) != "untitled" ||
        if (currentWidget->getFilePath().isEmpty() && currentWidget->toPlainText().isEmpty()) {
            continue;
        }
            // common file
        else {
            opened_tabs.push_back(currentWidget->getFilePath()); // or Tabs->tabToolTip(i)
            tabs_cursor_positions.push_back(QString::number(currentWidget->textCursor().position()));
        }
    }
    QSettings settings("Evolution");
    settings.setValue("Evolution/opened_tabs", opened_tabs);
    settings.setValue("Evolution/opened_tabs_cursor_positions", tabs_cursor_positions);

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


void MainWindow::OpenFile(const QModelIndex &file_index) {
    if (!Explorer->FileModel->isDir(file_index)) {
        OpenFile(Explorer->FileModel->filePath(file_index));

        currentWidget = qobject_cast<PlainTextEdit *>(Tabs->widget(Tabs->currentIndex()));
        currentWidget->setCodeInfo(codeInfoDock);
        currentWidget->setEducation(education);
        currentWidget->setClang(clangBridge);
        currentWidget->setTagReminder(tagReminder);
        currentWidget->setNodeView(nodeView);

        connect(currentWidget, SIGNAL(cursorPositionChanged()), this, SLOT(slotCursorPositionChanged()));
        // breakpoints
        connect(currentWidget->breakPointArea, SIGNAL(breakPointCreated(const int &)), this,
                SLOT(slotCreateBreakPoint(const int &)));
        connect(currentWidget->breakPointArea, SIGNAL(breakPointRemoved(const int &)), this,
                SLOT(slotDeleteBreakPoint(const int &)));
        connect(currentWidget->hoverInfo->MainWidget, SIGNAL(anchorClicked(const QUrl &)), this,
                SLOT(slotOpenHoverInfoUrl(const QUrl &)));
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

// docker
void MainWindow::UpdateCurrentIndex(QListWidgetItem *current_item) {
    int index = current_item->listWidget()->row(current_item);
    currentWidget = qobject_cast<PlainTextEdit *>(Tabs->widget(index));
    Tabs->setCurrentIndex(index);

    current_file_path->setText(currentWidget->getFilePath() + "     ");
}

// tab
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
    if (currentWidget) {
        current_file_path->setText(currentWidget->getFilePath() + "     ");
    }
}


void MainWindow::slotBuild() {

    console_dock->show();
    // clear terminal window
    console_dock->ConsoleOutput->clear();
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
        if (file_manager.executable_file_name.isEmpty()) {
            file_manager.executable_file_name = "a.out";
        } else {
            generator.setProjectName(file_manager.executable_file_name.toStdString());//"executable"
        }
        generator.setCompiler("clang++");
        generator.setCompileFlags("-O0 -g ");
        for (int i = 0; i < file_manager.source_files.size(); i++) {
            generator.addSourceFile((file_manager.source_files[i].toStdString()));
        }
        generator.createCmakeLists(file_manager.Project_Dir.toStdString());
        executor->Build(cmake, file_manager.Project_Dir.toStdString(), console_dock);

        // non static function need also an object to call it

    } else {
        // only 1 file no name
        if (file_manager.source_files.size() == 1 || currentWidget->getFilePath().isEmpty()) {
            // create temp dir
            QDir dir(file_manager.Project_Dir);
            if (!dir.exists()) {
                dir.mkdir("Evolution.temp");
            }

            std::vector<std::string> sources;
            sources.reserve(10);
            for (int i = 0; i < file_manager.source_files.size(); i++) {
                sources.push_back(file_manager.source_files[i].toStdString());
            }
            executor->Build(cmake, file_manager.Project_Dir.toStdString(), console_dock);
        }
    }

    //QSettings settings("Evolution");
    //settings.setValue("Evolution/executable_path/", file_manager.Project_Dir + "/executable");

    console_dock->ConsoleOutput->append("Build done");
    qDebug() << "build done";
}

void MainWindow::slotRun() {

    console_dock->show();
    // clear terminal window
    console_dock->ConsoleOutput->clear();

    bool cmake = true;
    if (CHANGES_IN_PROJECT) {
        slotBuild();
    }

    // TODO: if currentWidget has "" filepath -> dry run, set up temp dir for only 1 file

    if (cmake) {
        // file_manager.executable_file_path;  file_manager.executable_file_name;  (if not defined, first build  --> define a.out)
        // !!!!!!!!!! not reachable condition TODO: return to build stage or just assume it is correct :(
        if (file_manager.executable_file_name.isEmpty()) {
            file_manager.executable_file_name = "a.out";
        }
        if (file_manager.executable_file_path.isEmpty()) {
            file_manager.executable_file_path = file_manager.Project_Dir + "/cmake-build/" + file_manager.executable_file_name;
            executor->Execute(cmake, file_manager.executable_file_path.toStdString(), console_dock);
        }
        //CommandLineExecutor::Execute(cmake, file_manager.Project_Dir.toStdString() + "/cmake-build/" + "executable", console_dock->ConsoleOutput);
        else {
            executor->Execute(cmake, file_manager.executable_file_path.toStdString(), console_dock);
        }
    } else {
        // only 1 file, dir should exist already
        if (file_manager.source_files.size() == 1 || currentWidget->getFilePath().isEmpty()) {
            executor->Execute(cmake, file_manager.Project_Dir.toStdString() + "/Evolution.temp/" + "executable", console_dock);
        }
        //executor.setExecutableName("executable", file_manager.Project_Dir.toStdString());
        //int pid = executor.getPid();
        //QString process = QString("launched ") + QString("executable,  PID:") + QString::number(pid);
        //console_dock->setRawOutput(process);
    }
}

void MainWindow::slotClangFormat() {
    std::vector<std::string> sources;
    console_dock->ConsoleOutput->append(QString::fromStdString(CommandLineExecutor::ClangFormat(sources, file_manager.clang_format_path.toStdString())));
}
void MainWindow::slotClangTidy() {
    std::vector<std::string> sources;
    console_dock->ConsoleOutput->append(QString::fromStdString(CommandLineExecutor::ClangTidy(sources)));
}
void MainWindow::slotClangCheck() {
    std::vector<std::string> sources;
    console_dock->ConsoleOutput->append(QString::fromStdString(CommandLineExecutor::ClangCheck(sources)));
}
void MainWindow::slotValgrind() {
    console_dock->ConsoleOutput->append(QString::fromStdString(CommandLineExecutor::Valgrind(std::string())));
}

void MainWindow::slotPullRequest() {
    gitDock->setVisible(true);
    gitDock->Stack->setCurrentIndex(3);
}

void MainWindow::slotCommitRequest() {
    gitDock->setVisible(true);
    gitDock->Stack->setCurrentIndex(1);
}

void MainWindow::slotPushRequest() {
    gitDock->setVisible(true);
    gitDock->Stack->setCurrentIndex(2);
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
    if (!currentWidget) {
        return;
    }
    currentWidget->copy();
}

void MainWindow::slotCut() {
    if (!currentWidget) {
        return;
    }
    currentWidget->cut();
}

void MainWindow::slotUndo() {
    if (!currentWidget) {
        return;
    }
    currentWidget->undo();
}
void MainWindow::slotRedo() {
    if (!currentWidget) {
        return;
    }
    currentWidget->redo();
}

void MainWindow::slotSelectAll() {
    if (!currentWidget) {
        return;
    }
    currentWidget->selectAll();
}

void MainWindow::slotPaste() {
    if (!currentWidget) {
        return;
    }
    currentWidget->paste();
}

void MainWindow::slotRemoveAll() {
    if (!currentWidget) {
        return;
    }
    currentWidget->clear();
}

void MainWindow::slotExpand() {
    if (!currentWidget) {
        return;
    }
    currentWidget->slotExpand();
}

void MainWindow::slotCollapse() {
    if (!currentWidget) {
        return;
    }
    currentWidget->slotCollapse();
}

void MainWindow::slotToggleComment() {
    if (!currentWidget) {
        return;
    }
    currentWidget->toggleComment();
}

void MainWindow::slotFormat() {
    if (!currentWidget) {
        return;
    }
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
    const QString about = "Evolution IDE is an Integrated Development Environment mainly aimed for C & C++ . <br>"
                          "It is Open source. If you Want to use It, you can read Readme.md file with all features <br>"
                          "<br>"
                          "Newly contain Educational system, which could provide convenient code samples with brief <br>"
                          "comments. There are also provided many stuffs i personally always want to know quicker. <br>"
                          "If you are interested, you can provide your own sample. <br>"
                          "Web:  <br>"
                          "Github: <a style=color:gray; href = https://github.com/adamko222/Evolution-IDE>https://github.com/adamko222/Evolution-IDE </a>";

    QMessageBox *msg = new QMessageBox(this);
    msg->setTextFormat(Qt::RichText);
    msg->setText(about);
    msg->show();
    //QMessageBox::information(this, "About Evolution", about, QMessageBox::Close);
}

void MainWindow::slotStopProcess() {
    // executable is in registers -> no need to set it here, will be called in function
    int pid = CommandLineExecutor::getPid(std::string());
    if (pid != 0) {
        CommandLineExecutor::killProcess(0);
        console_dock->ConsoleOutput->append("Process killed , PID: " + QString::number(pid));
    } else {
        console_dock->ConsoleOutput->append("No Process attached");
    }
}

void MainWindow::slotStartDebug() {
    if (!Tabs->isVisible()) {// debuggerBridge
        showDebuggerView();
        // wait some time
        // unistd.h   -> linux header
        sleep(1);
        debuggerBridge->start();
    } else {
        debuggerBridge->start();
    }
}

void MainWindow::slotStopDebug() {
    debuggerBridge->slotStopDebug();
}

void MainWindow::slotContinue() {
    debuggerBridge->slotStopDebug();
}

void MainWindow::slotRunToCursor() {
    debuggerBridge->slotRunToCursor();
}

void MainWindow::slotStepOver() {
    debuggerBridge->slotStepOver();
}

void MainWindow::slotStepInto() {
    debuggerBridge->slotStepInto();
}

void MainWindow::slotStepInstruction() {
    debuggerBridge->slotStepInstruction();
}

void MainWindow::slotStepOut() {
    debuggerBridge->slotStepOut();
}

// this might go only through containBlock and decide to create or remove
void MainWindow::slotToggleBreakPoint() {
    if (!currentWidget) {
        return;
    }
    const QString filename = currentWidget->getFilePath();
    const int line = currentWidget->getCursorPosition().y();
    // in edit
    bool created = currentWidget->toggleBreakPoint(line);


    // in debugger
    if (!filename.isEmpty()) {
        // created
        if (created) {
            debuggerBridge->createBreakpoint(filename.toLatin1().data(), line);
        }
        // removed
        else {
            debuggerBridge->removeBreakpoint(filename.toLatin1().data(), line);
        }
    }
}

void MainWindow::slotCreateBreakPoint(const int &line) {
    QString filename = currentWidget->getFilePath();
    if (!filename.isEmpty()) {
        debuggerBridge->createBreakpoint(filename.toLatin1().data(), line);
    }
}

void MainWindow::slotDeleteBreakPoint(const int &line) {
    QString filename = currentWidget->getFilePath();
    if (!filename.isEmpty()) {
        debuggerBridge->removeBreakpoint(filename.toLatin1().data(), line);
    }
}

void MainWindow::slotSetBreakpointAtLine() {
    if (!currentWidget) {
        return;
    }
    const QString filename = currentWidget->getFilePath();
    const int line = currentWidget->getCursorPosition().y();

    // in debugger
    if (!filename.isEmpty()) {
        // in edit
        bool created = currentWidget->toggleBreakPoint(line);
        // created
        if (created) {
            debuggerBridge->showSetManualBreakPoint(filename);
        }
        // removed meaningless
    }
}

void MainWindow::slotShowBreakpointsList() {
    debuggerBridge->showBreakPointsList();
}

void MainWindow::slotShowAttachToProcess() {
    debuggerBridge->showTaskManager();
}

void MainWindow::slotRestart() {
    // weird i know :)
    const QString appFilePath = QApplication::applicationFilePath();
    CommandLineExecutor::ExecuteSimpleCommand(appFilePath.toLatin1().data());
    // FIXME: close current app, not working yet...
    CloseWindow();
}

void MainWindow::slotOpenCppSample(QListWidgetItem *item) {
    // represents sample to take
    int index = item->listWidget()->currentRow();
    int num_files = education->cpp_code_samples[index].fileNames.size();

    for (int i = 0; i < num_files; i++) {
        SampleLoading = true;
        education->WorkingContent = education->cpp_code_samples[index].content[i];// index might be out of range if not filled all samples !!!!!
        OpenFile(education->cpp_code_samples[index].fileNames[i]);
        SampleLoading = false;
    }
}

void MainWindow::slotOpenCppUserSample(QListWidgetItem *item) {
    int index = item->listWidget()->currentRow();
    SampleLoading = true;
    education->WorkingContent = education->cpp_user_samples[index];
    OpenFile(item->text());// item hold sample name
    SampleLoading = false;
}
