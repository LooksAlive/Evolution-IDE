#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
keyword this always represents parent in which class it is declared, also
want to show exact widget into this section(mainwindow - app)
*/

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setAcceptDrops(true);
    setWindowIcon(QIcon(":/icons/icon.png"));
    setWindowTitle("Evolution IDE");

    /* set all necessary widgets, features etc. */
    SetupTabWidget();
    SetupFileExplorer();
    SetupFileDocker();
    SetupCompileDock();
    SetupMenuBar();
    SetupToolBar();

    SetupDebuggerView();
    SetupBinaryView();
    SetupHexView();
    SetupVerticalBar();

    CreateFile();
    SetupDockWidgetsLayering();

    setCentralWidget(vertical_stack);
    Tabs->currentWidget()->setFocus();
    highlighter = new Highlighter(":/highlights/languages.xml", this);

    LoadRegisters();
}

MainWindow::~MainWindow() {
    delete ui;
    delete converter;
}

/* drag and drop functions for file into window as it would be openned */
void MainWindow::dragEnterEvent(QDragEnterEvent* drag_event) {
    if (drag_event->mimeData()->hasUrls())
        drag_event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent* drop_event) {
    QList<QUrl> url_list = drop_event->mimeData()->urls();
    // set dir into file explorer as default for project; handled in drop event
    foreach (QUrl url, url_list) {
        QString path = url.url(QUrl::RemoveScheme);
        OpenFile(path);
    }
}

void MainWindow::LoadRegisters(){
    QRect rect = settings.value("Evolution/MainWindowGeometry").toRect();
    if(!rect.isEmpty()){
        setGeometry(rect);
    }

    /*
    QStringList tabs = settings.value("Evolution/opened_tabs").toStringList();
    if(!tabs.isEmpty()){
        for (int i = 0; i <= tabs.size(); i++) {
            OpenFile(tabs[i]);
        }
    }
    */

    QString Project_Root_Dir = settings.value("Evolution/Project_Root_Dir").toString();
    if(!Project_Root_Dir.isEmpty()){
        Explorer->setRootDirectory(Project_Root_Dir);
    }

    // visible docks:
    if(settings.value("Evolution/ExplorerVisibility").toBool()){
        Explorer->toggleViewAction();
    }
    if(settings.value("Evolution/DockerVisibility").toBool()){;
        Docker->toggleViewAction();
    }
    if(settings.value("Evolution/ConsoleOutputVisibility").toBool()){
        ConsoleOutput->toggleViewAction();
    }

}

void MainWindow::SetupVerticalBar(){
    vertical_bar = new QToolBar(this);
    vertical_bar->setWindowTitle("Windows");
    vertical_bar->setOrientation(Qt::Vertical);
    vertical_bar->setMovable(false);
    //vertical_bar->setSizePolicy(QSizePolicy::Fixed);
    vertical_bar->setFixedWidth(70);
    vertical_bar->setFloatable(false);
    vertical_bar->setAcceptDrops(false);
    vertical_bar->setIconSize(QSize(250, 45));

    //vertical_bar->setContentsMargins(10,10,10,10);
    vertical_bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    vertical_bar->setFocusPolicy(Qt::ClickFocus);


    vertical_stack = new QStackedWidget(this);
    vertical_stack->setParent(this);

    vertical_stack->insertWidget(0, Tabs);
    vertical_stack->insertWidget(1, binaryView);
    vertical_stack->insertWidget(2, debuggerView);
    vertical_stack->insertWidget(3, hexview);

    vertical_stack->setCurrentIndex(0);  // start with editor

    vertical_bar->addAction(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/accessories-text-editor.png"), "Editor",  this, SLOT(showEditorView()));
    vertical_bar->addSeparator();
    vertical_bar->addAction(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/hex.png"), "Hex Editor",  this, SLOT(showHexView()));
    vertical_bar->addSeparator();
    vertical_bar->addAction(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/bug-buddy.svg"), "Debugger",  this, SLOT(showDebuggerView()));
    vertical_bar->addSeparator();
    vertical_bar->addAction(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/binary_view.svg"), "Binary View",  this, SLOT(showBinaryView()));
    vertical_bar->addSeparator();
    // decompiler ... later :)
    //vertical_bar->addAction(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/hex.png"), "Binary View",  this, SLOT(showBinaryView()));
    //vertical_bar->addSeparator();

    addToolBar(Qt::LeftToolBarArea, vertical_bar);
}

void MainWindow::SetupMenuBar() {
    /* menu names  */
    QMenu* fileMenu = new QMenu("File");
    QMenu* editMenu = new QMenu("Edit");
    QMenu* viewMenu = new QMenu("View");

    /* actions under specific menu section, some shortcuts are random :) */
    fileMenu->addAction("New File",   this, SLOT(CreateFile()),    Qt::CTRL + Qt::Key_N);
    fileMenu->addAction("Open File",  this, SLOT(OpenFile()),      Qt::CTRL + Qt::Key_O);
    fileMenu->addSeparator();
    fileMenu->addAction("Save File",  this, SLOT(SaveFile()),      Qt::CTRL + Qt::Key_S);
    fileMenu->addAction("Save As...", this, SLOT(SaveFileAs()),    Qt::SHIFT + Qt::CTRL + Qt::Key_S);
    fileMenu->addAction("Save All",   this, SLOT(SaveAllFiles()));
    fileMenu->addSeparator();
    fileMenu->addAction("Close File", this, SLOT(CloseFile()),     Qt::CTRL + Qt::Key_W);
    fileMenu->addAction("Close All",  this, SLOT(CloseAllFiles()), Qt::SHIFT + Qt::CTRL + Qt::Key_W);
    fileMenu->addSeparator();
    fileMenu->addAction("Settings", this, SLOT(SetupSettingsWindow()), Qt::CTRL + Qt::Key_P);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit",       this, SLOT(CloseWindow()),   Qt::CTRL + Qt::Key_Q);

    editMenu->addAction("Cut",        this, SLOT(slotCut()),       Qt::CTRL + Qt::Key_X);
    editMenu->addAction("Copy",       this, SLOT(slotCopy()),      Qt::CTRL + Qt::Key_C);
    editMenu->addAction("Paste",      this, SLOT(slotPaste()),     Qt::CTRL + Qt::Key_V);
    editMenu->addAction("Delete",     this, SLOT(slotClear()),     Qt::CTRL + Qt::Key_Backspace);
    editMenu->addAction("Select All", this, SLOT(slotSelectAll()), Qt::CTRL + Qt::Key_A);
    editMenu->addAction("toggle comment", this, SLOT(slotToggleComment()), Qt::CTRL + Qt::SHIFT + Qt::Key_C);

    viewMenu->addAction(Explorer->toggleViewAction());
    viewMenu->addAction(Docker->toggleViewAction());
    viewMenu->addAction(ConsoleOutput->toggleViewAction());
    viewMenu->addAction(find_replace->toggleViewAction());
    viewMenu->addAction("Converter", this, SLOT(SetupConverter()));

    /* replace ui; decide if use namespace or just MainWindow->declared int .h as private pointer  */
    ui->menuBar->addMenu(fileMenu);
    ui->menuBar->addMenu(editMenu);
    ui->menuBar->addMenu(viewMenu);
}

void MainWindow::SetupToolBar() {
    ui->mainToolBar->addAction(QIcon(":/icons/new_file.png"),       "Create New File", this, SLOT(CreateFile()));
    ui->mainToolBar->addAction(QIcon(":/icons/open_file.png"),      "Open File",       this, SLOT(OpenFile()));
    ui->mainToolBar->addAction(QIcon(":/icons/save_file.png"),      "Save File",       this, SLOT(SaveFile()));
    ui->mainToolBar->addAction(QIcon(":/icons/save_all_files.png"), "Save All Files",  this, SLOT(SaveAllFiles()));
    ui->mainToolBar->addAction(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/undo.png"), "undo", this, SLOT(slotUndo()));
    ui->mainToolBar->addAction(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/redo.png"), "redo", this, SLOT(slotRedo()));

    ui->mainToolBar->addAction(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/build.png"), "Build", this, SLOT(slotBuild()));
    ui->mainToolBar->addAction(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/run.png"), "Run", this, SLOT(slotRun()));

}


void MainWindow::SetupDockWidgetsLayering(){
    setDockOptions(QMainWindow::AllowTabbedDocks | QMainWindow::AllowNestedDocks |
                   QMainWindow::GroupedDragging | QMainWindow::AnimatedDocks);
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
}

// setToolTip: 1 -> created[visible] ;  2 -> created[invisible] ;   "" -> not created
void MainWindow::showEditorView(){

    vertical_stack->setCurrentWidget(Tabs);
}

void MainWindow::showBinaryView(){

    vertical_stack->setCurrentWidget(binaryView);
}

void MainWindow::showDebuggerView(){

    vertical_stack->setCurrentWidget(debuggerView);

    // for now, starting with current file, later track    ;  not i only care for line
    debuggerView->setDebugPosition(file_manager.current_full_filepath, 5);
    debuggerView->setExecutable(file_manager.executable_file_path.toStdString());
}

void MainWindow::showHexView(){

    hexview->open(file_manager.current_full_filepath);
    vertical_stack->setCurrentWidget(hexview);
}




void MainWindow::SetupTabWidget() {
    Tabs = new Tab(this);
    connect(Tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(CloseFile(int)));

    connect(Tabs->AddNewTabButton, SIGNAL(clicked()), this, SLOT(CreateFile()));
}


void MainWindow::SetupFileExplorer() {
    Explorer = new FileExplorer(this);
    Explorer->setRootDirectory(QDir::homePath());

    connect(Explorer->FileView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(OpenFile(QModelIndex)));/* double click */
    addDockWidget(Qt::LeftDockWidgetArea, Explorer); /* show at left side; function for MainWindow */
}

void MainWindow::SetupFileDocker() {
    Docker = new FileDock(this);

    connect(Tabs->tabBar(),      SIGNAL(tabMoved(int, int)),            this, SLOT(ChangeTabIndexInList(int, int)));
    connect(Docker->DockerFileList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(UpdateCurrentIndex(QListWidgetItem*)));
    connect(Docker->DockerFileList, SIGNAL(currentRowChanged(int)),        Tabs, SLOT(setCurrentIndex(int)));
    connect(Tabs->tabBar(),      SIGNAL(currentChanged(int)),           this, SLOT(UpdateCurrentIndex(int)));
    connect(Tabs->tabBar(),      SIGNAL(tabCloseRequested(int)),        this, SLOT(UpdateCurrentIndexOnDelete(int)));

    addDockWidget(Qt::LeftDockWidgetArea, Docker);
}


void MainWindow::SetupCompileDock(){
    ConsoleOutput = new ConsoleDock(this);
    find_replace = new FindReplaceWidget(Tabs, this);

    addDockWidget(Qt::BottomDockWidgetArea, ConsoleOutput);
    addDockWidget(Qt::BottomDockWidgetArea, find_replace);

    tabifyDockWidget(ConsoleOutput, find_replace);
}


/* exteranl windows */

void MainWindow::SetupSettingsWindow(){
    Settings = new SettingsWindow(this); // if there will be this --> not working ...
    Settings->show();
}

void MainWindow::SetupConverter(){
    converter = new Converter();
    converter->show();
}


void MainWindow::SetupDebuggerView(){
    debuggerView = new DebuggerWidget(this);

}
void MainWindow::SetupBinaryView(){
    binaryView = new BinaryView(this);
}
void MainWindow::SetupHexView(){
    hexview = new HexView(this);
}
void MainWindow::showDecompilerView(){

}




/* File handling stuffs */

void MainWindow::CreateFile() {
    // tab
    PlainTextEdit* NewPlainText = new PlainTextEdit;
    int index = Tabs->addTab(NewPlainText, NEW_TAB_NAME);
    Tabs->setCurrentIndex(index);
    Tabs->setTabToolTip(index, "");
    Tabs->setTabWhatsThis(index, "No changes");
    connect(NewPlainText, SIGNAL(textChanged()), this, SLOT(UpdateParameter()));

    // file dock
    QListWidgetItem* new_item = new QListWidgetItem;
    new_item->setText(Tabs->tabText(index));
    new_item->setToolTip(Tabs->tabToolTip(index));
    Docker->DockerFileList->addItem(new_item);

    UpdateCurrentIndex(index);
}

/* Base operations in MenuBar
--------------------------------------------------------------------------*/


void MainWindow::OpenFile() {
    // #TODO:  do not work yet << setfilemode, to set dir into explorer from dialog
    QFileDialog *dialog = new QFileDialog(this);
    //QString filepath = dialog->getOpenFileName(this, "Open file", QDir::homePath());
    QString filepath = dialog->getExistingDirectory(this, "Open directory", QDir::homePath());
    //dialog->setFileMode(QFileDialog::ExistingFiles);
    //dialog->setFileMode(QFileDialog::Directory);

    if (filepath.isEmpty())
        return;

    OpenFile(filepath);
}

void MainWindow::OpenFile(const QString& filepath) {

    // directory operations
    if (QFileInfo(filepath).isDir()){
        // set project path into file view
        Explorer->setRootDirectory(filepath);
        // set recursively all files in dir, set root project dir
        file_manager.getFilesRecursively(filepath);
        return;
    }

    PlainTextEdit* new_text_edit = new PlainTextEdit();
    new_text_edit->appendPlainText(file_manager.read(filepath));

    /* checks for duplicate file-openning and prevents it by opening identical tab twice */

    for (int i = 0; i < Tabs->count(); ++i)
        if (Tabs->tabToolTip(i) == file_manager.current_full_filepath) {
            Tabs->setCurrentIndex(i);
            return;
        }

    // ????????????????????????????????????????????????????????

    PlainTextEdit *temp_text = (PlainTextEdit*)Tabs->currentWidget();
    if (temp_text->document()->isEmpty() &&
            Tabs->tabToolTip(Tabs->currentIndex()) == "" &&
            Tabs->tabText(Tabs->currentIndex()) == NEW_TAB_NAME) {

        DeleteTabFromList(Tabs->currentIndex());
        delete Tabs->widget(Tabs->currentIndex());
    }



    // tab
    int index = Tabs->addTab(new_text_edit, file_manager.current_file_name);
    Tabs->setCurrentIndex(index);
    Tabs->setTabToolTip(index, file_manager.current_full_filepath);
    Tabs->setTabWhatsThis(index, "No changes");
    connect(new_text_edit, SIGNAL(textChanged()), this, SLOT(UpdateParameter()));

    QListWidgetItem* new_item = new QListWidgetItem();
    new_item->setText(Tabs->tabText(index));
    new_item->setToolTip(Tabs->tabToolTip(index));
    Docker->DockerFileList->addItem(new_item);

    // setting up highlight
    if (highlighter->setExtension(file_manager.getFileExtension(file_manager.current_file_name))) {
        highlighter->setDocument(new_text_edit->document());
        highlighter->highlightBlock(new_text_edit->toPlainText());
    }

    Tabs->setTabWhatsThis(index, "No changes");
    UpdateCurrentIndex(index); // setting up selected item in opened_docs_dock
}


void MainWindow::SaveFile() {
    // new file created, but not saved yet
    if (Tabs->tabToolTip(Tabs->currentIndex()) == "") {
        CHANGES_IN_PROJECT = true;
        SaveFileAs();
        return;
    }
    CHANGES_IN_PROJECT = true;

    file_manager.write(Tabs->tabToolTip(Tabs->currentIndex()), // filepath, buffer
                       ((PlainTextEdit*)Tabs->currentWidget())->toPlainText().toUtf8());

    // ??????????????????????????????????????????
    Tabs->setTabWhatsThis(Tabs->currentIndex(), "No changes");

}

void MainWindow::SaveFileAs() {

    CHANGES_IN_PROJECT = true;

    //QString filename = Tabs->tabText(Tabs->currentIndex());
    QString filepath = QFileDialog::getSaveFileName(this, "Save as");
    if (filepath.isEmpty())
        return;

    file_manager.write(filepath, ((PlainTextEdit*)Tabs->currentWidget())->toPlainText().toUtf8());


    Tabs->tabBar()->setTabText(Tabs->currentIndex(), file_manager.current_file_name);
    Tabs->tabBar()->setTabToolTip(Tabs->currentIndex(), filepath);

    // setting up highlight
    if (highlighter->setExtension(file_manager.getFileExtension(file_manager.current_file_name))) {
        highlighter->setDocument(((PlainTextEdit*)Tabs->currentWidget())->document()); // unsafe getting!
        highlighter->highlightBlock(((PlainTextEdit*)Tabs->currentWidget())->toPlainText()); // unsafe getting!
    }

    Tabs->setTabWhatsThis(Tabs->currentIndex(), "No changes");
}

void MainWindow::SaveAllFiles() {
    int current_index = Tabs->currentIndex();
    for (int i = 0; i < Tabs->count(); ++i) { // QTabWidget guarantees the consistency of indices?
        Tabs->setCurrentIndex(i);
        if (Tabs->tabWhatsThis(Tabs->currentIndex()) != "No changes")
            SaveFile();
    }
    Tabs->setCurrentIndex(current_index);
}

/* help function for tab index close action */
void MainWindow::CloseFile(int index_) {

    // untitled tab, first ask, before opening savedialog in savefile function
    if(Tabs->tabText(index_) == NEW_TAB_NAME && Tabs->tabWhatsThis(Tabs->currentIndex()) != "No changes")
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Saving changes", "Save changes before closing?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            SaveFile();
        }
    }else if (Tabs->tabWhatsThis(Tabs->currentIndex()) != "No changes") {
        if(ALWAYS_SAVE){
            SaveFile();
        }
    }

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
    for (int i = 0; i < Tabs->count(); ++i) { // QTabWidget guarantees the consistency of indices?
        if (Tabs->tabWhatsThis(i) != "No changes") {
            some_changes = true;
            break;
        }
    }
    if (some_changes && !ALWAYS_SAVE) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Save all changes", "Save all changes before closing?",
            QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
            SaveAllFiles();
    }
    else if(ALWAYS_SAVE){
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
    // reopen project not closed tabs later.
    QStringList openned_tabs;
    for (int i = 0; i <= Tabs->count(); i++) {
        openned_tabs.append(Tabs->tabToolTip(i));
    }
    settings.setValue("Evolution/opened_tabs", openned_tabs);
    // save window geometry, load in constructor
    settings.setValue("Evolution/MainWindowGeometry", geometry());

    // docks visible
    if(Explorer->isVisible()){
        settings.setValue("Evolution/ExplorerVisibility", true);
    }
    if(Docker->isVisible()){
        settings.setValue("Evolution/DockerVisibility", true);
    }
    if(ConsoleOutput->isVisible()){
        settings.setValue("Evolution/ConsoleOutputVisibility", true);
    }

    CloseAllFiles();
    QApplication::quit();
}

/* X --> close app - virtual func. */
void MainWindow::closeEvent(QCloseEvent*) {
    CloseWindow();
}

void::MainWindow::UpdateParameter() {
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
    if (!Explorer->FileModel->isDir(file_index))
        OpenFile(Explorer->FileModel->filePath(file_index));
}




/* tab functions ;   also move into separate file with tabs
--------------------------------------------------------------------------*/

void MainWindow::DeleteTabFromList(int index) {
    QListWidgetItem* temp_item = Docker->DockerFileList->takeItem(index);
    delete temp_item;
}

void MainWindow::ChangeTabIndexInList(int old_index, int new_index) {
    QListWidgetItem* first_item  = Docker->DockerFileList->takeItem(old_index);
    Docker->DockerFileList->insertItem(new_index, first_item);
}

void MainWindow::UpdateCurrentIndex(QListWidgetItem* current_item) {
    int index = current_item->listWidget()->row(current_item);
    Tabs->setCurrentIndex(index);
}

void MainWindow::UpdateCurrentIndex(int new_selection_index) {
    Docker->DockerFileList->setCurrentRow(new_selection_index);

    // + highlight update
    QString file = Tabs->tabBar()->tabText(new_selection_index);
    QString file_extension = QFileInfo(file).suffix();
    if (!file_extension.isEmpty()) {
        if (highlighter->setExtension(file_extension)) {
            highlighter->setDocument(((PlainTextEdit*)Tabs->currentWidget())->document()); // unsafe getting!
            highlighter->highlightBlock(((PlainTextEdit*)Tabs->currentWidget())->toPlainText()); // unsafe getting!
        }
    }
}

void MainWindow::slotBuild(){

    bool cmake = true;
    /*
    QString exec = file_manager.Project_Dir +"/" + "executable";
    if(!CHANGES_IN_PROJECT && QDir::exists(exec)){
        return;
    }
    */
    CommandLineExecutor executor;

    if(cmake){ // set option flags to some default
        CmakeGenerator generator;
        generator.setProjectName("executable");
        generator.setCompiler("clang++");
        generator.setCompileFlags("-O0 -g ");
        for (int i = 0; i < file_manager.source_files.size(); i++) {
            generator.addSourceFile((file_manager.source_files[i].toStdString()));
        }
        generator.createCmakeLists(file_manager.Project_Dir.toStdString());
        executor.ProjectRootDir = file_manager.Project_Dir.toStdString();

        ConsoleOutput->setRawOutput(QString::fromStdString(executor.cmake_build));
        QString raw = QString::fromStdString(executor.Build(cmake));
        ConsoleOutput->setRawOutput(raw);
        qDebug() << QString::fromStdString(executor.cmake_build);
        qDebug() << raw;
    }
    else{
        executor.setCompiler("clang++", CommandLineExecutor::Debug, "");
        executor.setExecutableName("executable", file_manager.Project_Dir.toStdString()); // watch for / at the end
        std::vector<std::string> sources;
        for (int i = 0; i < file_manager.source_files.size(); i++) {
            sources.push_back(file_manager.source_files[i].toStdString());
        }
        executor.setSourceFiles(sources);
        QString raw = QString::fromStdString(executor.Build(cmake));
        // works perfectly, check unprintable characters
        /*
        std::string s = "test";
        QString q = QString::fromStdString(s);
        qDebug() << q;
        qDebug() << q.size();
        */
        qDebug() << QString::fromStdString(executor.compile_args);
        ConsoleOutput->setRawOutput(raw);
        qDebug() << raw;
    }
    ConsoleOutput->setRawOutput("done");
    qDebug() << "build done";
}
void MainWindow::slotRun(){

    bool cmake = true;
    if(CHANGES_IN_PROJECT){
        slotBuild();
    }

    CommandLineExecutor executor;

    if(cmake){
        executor.setExecutableName("executable", file_manager.Project_Dir.toStdString());
        ConsoleOutput->setRawOutput(QString::fromStdString(executor.Execute(cmake)));
        //ConsoleOutput->setRawOutput(QString::fromStdString(executor.cmake_exec));
        qDebug() << QString::fromStdString(executor.cmake_exec);
    }
    else{
        ConsoleOutput->setRawOutput(QString::fromStdString(executor.exec_args));
        qDebug() << QString::fromStdString(executor.exec_args);
        executor.setExecutableName("executable", file_manager.Project_Dir.toStdString());
        ConsoleOutput->setRawOutput(QString::fromStdString(executor.Execute(cmake)));
    }
}
void MainWindow::slotClangFormat(){
    CommandLineExecutor executor;
    std::vector<std::string> sources;
    ConsoleOutput->setRawOutput(QString::fromStdString(executor.ClangFormat(sources)));
}
void MainWindow::slotClangTidy(){
    CommandLineExecutor executor;
    std::vector<std::string> sources;
    ConsoleOutput->setRawOutput(QString::fromStdString(executor.ClangTidy(sources)));
}
void MainWindow::slotClangCheck(){
    CommandLineExecutor executor;
    std::vector<std::string> sources;
    ConsoleOutput->setRawOutput(QString::fromStdString(executor.ClangCheck(sources)));
}
void MainWindow::slotValgrind(){
    CommandLineExecutor executor;
    ConsoleOutput->setRawOutput(QString::fromStdString(executor.Valgrind()));
}
void MainWindow::slotGdbGui(){
    CommandLineExecutor executor;
    executor.OpenGdbGui();
}




void MainWindow::UpdateCurrentIndexOnDelete(int) {
    /* (Relies on fact that after deletion current tab is always (count() - 1)th tab) */
    Docker->DockerFileList->setCurrentRow(Docker->DockerFileList->count() - 1);
}

void MainWindow::slotCopy() {
    ((PlainTextEdit*)Tabs->currentWidget())->copy();
}

void MainWindow::slotCut() {
    ((PlainTextEdit*)Tabs->currentWidget())->cut();
}

void MainWindow::slotUndo(){
    ((PlainTextEdit*)Tabs->currentWidget())->undo();
}
void MainWindow::slotRedo(){
    ((PlainTextEdit*)Tabs->currentWidget())->redo();
}

void MainWindow::slotSelectAll() {
    ((PlainTextEdit*)Tabs->currentWidget())->selectAll();
}

void MainWindow::slotPaste() {
    ((PlainTextEdit*)Tabs->currentWidget())->paste();
}

void MainWindow::slotClear() {
    ((PlainTextEdit*)Tabs->currentWidget())->clear();
}



// text operations
void MainWindow::slotToggleComment() {
    ((PlainTextEdit *) Tabs->currentWidget())->toggleComment();
}
