#include "DebuggerDock.h"
#include <icons/IconFactory.h>

DebuggerDock::DebuggerDock(QWidget *parent) : QDockWidget(parent) {
}
void DebuggerDock::createDock() {
}

void DebuggerDock::createConsole() {

    ConsoleTab = new QTabWidget(this);
    Console = new QWidget(this);
    MainConsoleLayout = new QHBoxLayout(this);
    console_out_in = new QVBoxLayout();
    debug_output = new QPlainTextEdit(this);
    DebuggerPrompt = new QLineEdit(this);
    completer = new QCompleter(this);

    DebuggerOutput = new QWidget(this);
    //auto *variables = new QWidget(this);
    VariablesView = new QTreeView(this);
    VariablesView->setHeaderHidden(true);
    // not editable yet, later possible to change values
    VariablesView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    debug_output->setReadOnly(true);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    // TODO: set data to completer somehow
    DebuggerPrompt->setCompleter(completer);
    // enter
    connect(DebuggerPrompt, SIGNAL(returnPressed()), this, SLOT(slotCmdlineExecute()));

    console_out_in->addWidget(debug_output);
    console_out_in->addWidget(DebuggerPrompt);
    console_out_in->setContentsMargins(0, 0, 0, 0);
    Console->setFixedHeight(200);

    createControlPanel();
    createCallStackWindow();
    createBreakPointList();

    DebuggerOutput->setLayout(console_out_in);
    DebuggerOutput->setContentsMargins(0, 0, 0, 0);


    ConsoleTab->addTab(DebuggerOutput, "Debugger Output");
    ConsoleTab->addTab(VariablesView, "Variables");

    MainConsoleLayout->addLayout(CallStackLayout);
    MainConsoleLayout->addLayout(ControlPanel);
    MainConsoleLayout->addWidget(ConsoleTab);
    MainConsoleLayout->addWidget(BreakPoint_List);

    Console->setLayout(MainConsoleLayout);
}

void DebuggerDock::createToolBar() {
    DebugToolBar = new QToolBar(this);
    //DebugToolBar->setContentsMargins(0,0,0,0);

    btn_StartDebug = new QToolButton(this);
    btn_StartDebug->setIcon(QIcon(IconFactory::StartDebug));
    btn_StartDebug->setFixedSize(26, 26);
    connect(btn_StartDebug, SIGNAL(clicked()), this, SLOT(slotStartDebug()));
    DebugToolBar->addWidget(btn_StartDebug);

    btn_StopDebug = new QToolButton(this);
    btn_StopDebug->setIcon(QIcon(IconFactory::StopDebug));
    btn_StopDebug->setFixedSize(26, 26);
    connect(btn_StopDebug, SIGNAL(clicked()), this, SLOT(slotStopDebug()));
    DebugToolBar->addWidget(btn_StopDebug);
    //DebugToolBar->addSeparator(); // cause of error

    btn_RunToCursor = new QToolButton(this);
    btn_RunToCursor->setIcon(QIcon(IconFactory::RunToCursor));
    btn_RunToCursor->setFixedSize(26, 26);
    connect(btn_RunToCursor, SIGNAL(clicked()), this, SLOT(slotRunToCursor()));
    DebugToolBar->addWidget(btn_RunToCursor);

    btn_StartDebug->setEnabled(true);// at start i can run a process, but not stop it
    btn_StopDebug->setEnabled(false);
    btn_RunToCursor->setEnabled(false);

    ControlPanel->addWidget(DebugToolBar);
}

void DebuggerDock::createControlPanel() {
    ControlPanel = new QVBoxLayout();
    ControlPanel->setContentsMargins(0, 0, 0, 0);

    createToolBar();

    btn_StepOver = new QToolButton(this);
    btn_StepOver->setText(" Step Over");
    btn_StepOver->setIcon(QIcon(IconFactory::NextLine));
    btn_StepOver->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn_StepOver->setFixedSize(160, 30);
    connect(btn_StepOver, SIGNAL(clicked()), this, SLOT(slotStepOver()));
    ControlPanel->addWidget(btn_StepOver);

    btn_StepInto = new QToolButton(this);
    btn_StepInto->setText(" Step Into");
    btn_StepInto->setIcon(QIcon(IconFactory::StepInto));
    btn_StepInto->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn_StepInto->setFixedSize(160, 30);
    connect(btn_StepInto, SIGNAL(clicked()), this, SLOT(slotStepInto()));
    ControlPanel->addWidget(btn_StepInto);

    btn_StepOut = new QToolButton(this);
    btn_StepOut->setText(" Step Out");
    btn_StepOut->setIcon(QIcon(IconFactory::GetOutOfFunction));
    btn_StepOut->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn_StepOut->setFixedSize(160, 30);
    connect(btn_StepOut, SIGNAL(clicked()), this, SLOT(slotStepOut()));
    ControlPanel->addWidget(btn_StepOut);

    btn_StepInstruction = new QToolButton(this);
    btn_StepInstruction->setText(" Step Instruction");
    btn_StepInstruction->setIcon(QIcon(IconFactory::StepInstruction));
    btn_StepInstruction->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn_StepInstruction->setFixedSize(160, 30);
    connect(btn_StepInstruction, SIGNAL(clicked()), this, SLOT(slotStepInstruction()));
    ControlPanel->addWidget(btn_StepInstruction);

    btn_Continue = new QToolButton(this);
    btn_Continue->setText(" Continue");
    btn_Continue->setIcon(QIcon(IconFactory::Resume));
    btn_Continue->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn_Continue->setFixedSize(160, 30);
    connect(btn_Continue, SIGNAL(clicked()), this, SLOT(slotContinue()));
    ControlPanel->addWidget(btn_Continue);

    // at start i can not do anything, only when the process is in progress and breakpoint is hit or else
    btn_StepOver->setEnabled(false);
    btn_StepInto->setEnabled(false);
    btn_StepInstruction->setEnabled(false);
    btn_Continue->setEnabled(false);
    btn_StepOut->setEnabled(false);

    ControlPanel->addStretch();
}

void DebuggerDock::createDebugWatchWindow() {

    WatchWindow = new QWidget(this);
    WatchLayout = new QVBoxLayout(this);
    WatchListView = new QListWidget(this);
    WatchWindow->setMaximumWidth(250);// for now ... later change responsibly

    WatchLayout->addWidget(WatchListView);
    WatchLayout->setContentsMargins(0, 0, 0, 0);
    //var_layout->addWidget(new PlainTextEdit);
    WatchWindow->setLayout(WatchLayout);

    /*
    QSplitter *watchDockContainer=new QSplitter(this);
    watchDockContainer->setOrientation(Qt::Vertical);
    QToolBar *customWatchControl=new QToolBar(watchDockContainer);
    QToolButton *addwatch =new QToolButton(watchDockContainer);
    addwatch->setIcon(QPixmap(":/DebugToolBar/image/Debug Docks/AddWatch.png"));
    customWatchControl->addWidget(addwatch);
    QToolButton *modifywatch=new QToolButton(watchDockContainer);
    modifywatch->setIcon(QPixmap(":/DebugToolBar/image/Debug Docks/ModifyWatch.png"));
    customWatchControl->addWidget(modifywatch);
    customWatchControl->addSeparator();
    QToolButton *removewatch=new QToolButton(watchDockContainer);
    removewatch->setIcon(QPixmap(":/DebugToolBar/image/Debug Docks/RemoveWatch.png"));

    customWatchControl->addWidget(removewatch);
    */
}

void DebuggerDock::createCallStackWindow() {
    CallStackLayout = new QVBoxLayout();
    ThreadBox = new QComboBox(this);
    CallStack = new QListWidget(this);

    CallStackLayout->setContentsMargins(0, 0, 5, 0);
    CallStackLayout->setSpacing(0);
    ThreadBox->setFixedWidth(200);
    CallStack->setFixedWidth(200);
    connect(CallStack, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotOpenCallStackFile(QListWidgetItem *)));

    CallStackLayout->addWidget(ThreadBox);
    CallStackLayout->addWidget(CallStack);
}

void DebuggerDock::fillCallStack() {
    for (int i = 0; i <= 5; i++) {
        CallStack->insertItem(i, "some call");
    }
}

void DebuggerDock::slotOpenCallStackFile(QListWidgetItem *item) {
    //item->text();
}


void DebuggerDock::createBreakPointList() {
    //BreakPoint_List = new BreakPointListWindow(this);
    // connect tool buttons + some new slots
    connect(BreakPoint_List->BpList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotGoToBreakPointFile(QListWidgetItem *)));
    connect(BreakPoint_List->remove, SIGNAL(clicked()), this, SLOT(slotRemoveBreakPoint()));
    connect(BreakPoint_List->removeAll, SIGNAL(clicked()), this, SLOT(slotRemoveAllBreakPoint()));
    //connect(BreakPoint_List->mute, SIGNAL(clicked()), this, SLOT(slotMuteBreakPoint()));
    //connect(BreakPoint_List->muteAll, SIGNAL(clicked()), this, SLOT(slotMuteAllBreakPoint()));
}

void DebuggerDock::slotGoToBreakPointFile(QListWidgetItem *item) {
    const char *filepath = BreakPoint_List->BpList->currentItem()->text(1).toLatin1().data();
    const int line = BreakPoint_List->BpList->currentItem()->text(2).toInt();
    //setStartPosition(filepath, line);
}

void DebuggerDock::slotRemoveBreakPoint() {
    if (!BreakPoint_List->BpList->selectedItems().isEmpty()) {
        const int ID = BreakPoint_List->BpList->currentItem()->text(0).toInt();// ID
        //removeBreakpoint(ID);
    }
    // otherwise raises error, of course no selected item cannot be removed or ... selected at 0
    else {
        // no idea this will works :)
        BreakPoint_List->BpList->setCurrentIndex(BreakPoint_List->BpList->rootIndex());
    }
}

void DebuggerDock::slotRemoveAllBreakPoint() {
    for (int i = 0; i < /*BreakPointList.size()*/ 5; i++) {
        //const int ID = BreakPoint_List->BpList->currentItem()->text(0).toInt(); // ID
        // assume that break IDs are increasing constantly
        if (/*!Target.BreakpointDelete(i)*/ true) {
            debug_output->appendPlainText("Breakpoint was not removed !");
        }
        //BreakPointList.erase(BreakPointList.begin() + i);
    }
    // TODO: figure out how to get specific item in list and remove them 1 by 1
    // clear whole list and insert remaining break points
    BreakPoint_List->BpList->clear();
    //BreakPointList.clear();
}
