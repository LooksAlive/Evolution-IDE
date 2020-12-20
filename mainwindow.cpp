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


    CreateFile();
    SetupDockWidgetsLayering();

    setCentralWidget(Tabs); /* ??? replace in future */
    Tabs->currentWidget()->setFocus();
    highlighter = new Highlighter(":/highlights/languages.xml", this);

    QFont font;
    font.setFamily("Console");
    font.setWeight(15);


}

MainWindow::~MainWindow() {
    delete ui;
}

/* drag and drop functions for file into window as it would be openned */
void MainWindow::dragEnterEvent(QDragEnterEvent* drag_event) {
    if (drag_event->mimeData()->hasUrls())
        drag_event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent* drop_event) {
    QList<QUrl> url_list = drop_event->mimeData()->urls();
    foreach (QUrl url, url_list) {
        OpenFile(url.url(QUrl::RemoveScheme));
    }
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
    fileMenu->addAction("Settings", this, SLOT(OpenSettingsWindow()), Qt::CTRL + Qt::Key_P);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit",       this, SLOT(CloseWindow()),   Qt::CTRL + Qt::Key_Q);

    editMenu->addAction("Cut",        this, SLOT(slotCut()),       Qt::CTRL + Qt::Key_X);
    editMenu->addAction("Copy",       this, SLOT(slotCopy()),      Qt::CTRL + Qt::Key_C);
    editMenu->addAction("Paste",      this, SLOT(slotPaste()),     Qt::CTRL + Qt::Key_V);
    editMenu->addAction("Delete",     this, SLOT(slotClear()),     Qt::CTRL + Qt::Key_Backspace);
    editMenu->addAction("Select All", this, SLOT(slotSelectAll()), Qt::CTRL + Qt::Key_A);
    editMenu->addSeparator();
    editMenu->addAction("Change Font", this, SLOT(SetFont()));

    viewMenu->addAction(Explorer->toggleViewAction());
    viewMenu->addAction(Docker->toggleViewAction());
    viewMenu->addAction(OutputWindow->toggleViewAction());

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
}


void MainWindow::SetupDockWidgetsLayering(){
    setDockOptions(QMainWindow::AllowTabbedDocks | QMainWindow::AllowNestedDocks |
                   QMainWindow::GroupedDragging | QMainWindow::AnimatedDocks);
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
}

void MainWindow::SetFont(){
    QFont font;
    font.setFamily("Console");
    font.setWeight(15);

    // create font dialog
    bool ok = true;
    font = QFontDialog::getFont(&ok, font, this); // without defaultfont also works
    if(ok){
        Tabs->setFont(font);
    }

}



void MainWindow::SetupTabWidget() {
    Tabs = new Tab(this);
    connect(Tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(CloseFile(int)));

    connect(Tabs->AddNewTabButton, SIGNAL(clicked()), this, SLOT(CreateFile()));
}


void MainWindow::SetupFileExplorer() {
    Explorer = new FileExplorer(this);

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
    OutputWindow = new ConsoleDock(this);
    OutputWindow->find_replace->setTextEdit((PlainTextEdit*)Tabs->currentWidget());
    addDockWidget(Qt::BottomDockWidgetArea, OutputWindow);
}






/* exteranl windows */

void MainWindow::OpenSettingsWindow(){
    Settings = new SettingsWindow(); // if there will be this --> not working ...
    Settings->show();
}





/* File handling stuffs */

void MainWindow::CreateFile() {
    PlainTextEdit* NewPlainText = new PlainTextEdit;
    int index = Tabs->addTab(NewPlainText, NEW_TAB_NAME);
    Tabs->setCurrentIndex(index);
    Tabs->setTabToolTip(index, "");
    Tabs->setTabWhatsThis(index, "No changes");
    connect(NewPlainText, SIGNAL(textChanged()), this, SLOT(UpdateParameter()));

    QListWidgetItem* new_item = new QListWidgetItem;
    new_item->setText(Tabs->tabText(index));
    new_item->setToolTip(Tabs->tabToolTip(index));
    Docker->DockerFileList->addItem(new_item);

    UpdateCurrentIndex(index);
}

/* Base operations in MenuBar */

void MainWindow::OpenFile() {
    QString filepath = QFileDialog::getOpenFileName(this, "Open file", FILE_DIALOG_PATH);
    if (filepath.isEmpty())
        return;
    OpenFile(filepath);
}

void MainWindow::OpenFile(const QString& filepath) {
    /* checks for duplicate file-openning and prevents it by opening identical tab twice */
    for (int i = 0; i < Tabs->count(); ++i)
        if (Tabs->tabToolTip(i) == filepath) {
            Tabs->setCurrentIndex(i);
            return;
        }

    QString filename = filepath.section("/",-1,-1);
    QFile file(filepath);

    if (file.open(QIODevice::ReadOnly)) {
        PlainTextEdit* temp_text = (PlainTextEdit*)Tabs->currentWidget(); /* #TODO: change this ridiculous check */
        if (temp_text->document()->isEmpty() &&
            Tabs->tabToolTip(Tabs->currentIndex()) == "" &&
            Tabs->tabText(Tabs->currentIndex()) == NEW_TAB_NAME) {
            DeleteTabFromList(Tabs->currentIndex());
            delete Tabs->widget(Tabs->currentIndex());
        }

        PlainTextEdit* new_text_edit = new PlainTextEdit;
        QString content = file.readAll();
        file.close();
        new_text_edit->appendPlainText(content);
        int index = Tabs->addTab(new_text_edit, filename);
        Tabs->setCurrentIndex(index);
        Tabs->setTabToolTip(index, filepath);
        Tabs->setTabWhatsThis(index, "No changes");
        connect(new_text_edit, SIGNAL(textChanged()), this, SLOT(UpdateParameter()));

        QListWidgetItem* new_item = new QListWidgetItem;
        new_item->setText(Tabs->tabText(index));
        new_item->setToolTip(Tabs->tabToolTip(index));
        Docker->DockerFileList->addItem(new_item);

        QString file_extension = QFileInfo(filename).suffix(); // setting up highlight
        if (highlighter->setExtension(file_extension)) {
            highlighter->setDocument(new_text_edit->document());
            highlighter->highlightBlock(new_text_edit->toPlainText());
        }

        Tabs->setTabWhatsThis(index, "No changes");
        UpdateCurrentIndex(index); // setting up selected item in opened_docs_dock
    } else {
        (new QErrorMessage(this))->showMessage("Cannot open file!");
        return;
    }
}

void MainWindow::SaveFile() {
    if (Tabs->tabToolTip(Tabs->currentIndex()) == "") {
        SaveFileAs();
        return;
    }
    QString filepath = Tabs->tabToolTip(Tabs->currentIndex());
    QFile file(filepath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(((PlainTextEdit*)Tabs->currentWidget())->document()->toPlainText().toUtf8()); // unsafe getting!
        file.close();
        Tabs->setTabWhatsThis(Tabs->currentIndex(), "No changes");
    } else {
        (new QErrorMessage(this))->showMessage("Cannot save file!");
        return;
    }
}

void MainWindow::SaveFileAs() {
    QString filename = Tabs->tabText(Tabs->currentIndex());
    QString filepath = QFileDialog::getSaveFileName(this, "Save " + filename, "/home/" + filename);
    if (filepath.isEmpty())
        return;
    if (QFileInfo(filepath).suffix().isEmpty())
        filepath.append(".txt");
    if (QFileInfo(filepath).suffix() == "pdf") {
        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A4);
        printer.setOutputFileName(filepath);
        //QTextDocument.setPageSize(printer.pageRect().size());
        ((PlainTextEdit*)Tabs->currentWidget())->document()->print(&printer);
    } else {
        QFile file(filepath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(((PlainTextEdit*)Tabs->currentWidget())->document()->toPlainText().toUtf8()); // unsafe getting!
            file.close();
        } else {
            (new QErrorMessage(this))->showMessage("Cannot save file!");
            return;
        }
    }
    filename = filepath.section("/",-1,-1);
    Tabs->tabBar()->setTabText(Tabs->currentIndex(), filename);
    Tabs->tabBar()->setTabToolTip(Tabs->currentIndex(), filepath);

    QString file_extension = QFileInfo(filename).suffix(); // setting up highlight
    if (highlighter->setExtension(file_extension)) {
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
    if (Tabs->tabWhatsThis(Tabs->currentIndex()) != "No changes") {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Saving changes", "Save changes before closing?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
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
    bool checker = false;
    for (int i = 0; i < Tabs->count(); ++i) { // QTabWidget guarantees the consistency of indices?
        if (Tabs->tabWhatsThis(i) != "No changes") {
            checker = true;
            break;
        }
    }
    if (checker) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Save all changes", "Save all changes before closing?",
            QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
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




/* tab functions ;   also move into separate file with tabs */

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

void MainWindow::UpdateCurrentIndexOnDelete(int) { // should be better?
    /* (Relies on fact that after deletion current tab is always (count() - 1)th tab) */
    Docker->DockerFileList->setCurrentRow(Docker->DockerFileList->count() - 1);
}

void MainWindow::slotCopy() {
    ((PlainTextEdit*)Tabs->currentWidget())->copy();
}

void MainWindow::slotCut() {
    ((PlainTextEdit*)Tabs->currentWidget())->cut();
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
