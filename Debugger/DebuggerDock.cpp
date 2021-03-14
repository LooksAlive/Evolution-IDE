#include <QFileInfo>
#include "icons/IconFactory.h"
#include "DebuggerDock.h"

BreakPointListWindow::BreakPointListWindow(QWidget *parent) : QWidget(parent) {
    createWindow();
}

void BreakPointListWindow::createWindow() {
    MainLayout = new QHBoxLayout();
    BpList = new QTreeWidget(this);
    BpBar = new QToolBar(this);

    setMaximumWidth(300);
    BpList->setMinimumWidth(220);
    BpList->setColumnCount(3);
    BpList->setHeaderLabels(QStringList() << "ID"
                                          << "File"
                                          << "Line");
    BpList->setColumnWidth(0, 40); // ID
    BpList->setColumnWidth(1, 150);// filename
    BpList->setColumnWidth(2, 30); // line

    BpBar->setOrientation(Qt::Vertical);
    BpBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    BpBar->setFixedWidth(30);
    // connect outside, we have to join editor

    remove = new QToolButton(this);
    remove->setToolTip("Remove");
    remove->setIcon(QIcon(IconFactory::Remove));
    removeAll = new QToolButton(this);
    removeAll->setToolTip("Remove All");
    removeAll->setIcon(QIcon(IconFactory::RemoveAll));
    mute = new QToolButton(this);
    mute->setToolTip("Mute");
    mute->setIcon(QIcon(IconFactory::Mute));
    muteAll = new QToolButton(this);
    muteAll->setToolTip("Mute All");
    muteAll->setIcon(QIcon(IconFactory::Mute));

    BpBar->addWidget(remove);
    BpBar->addWidget(removeAll);
    BpBar->addWidget(mute);
    BpBar->addWidget(muteAll);

    MainLayout->addWidget(BpList);
    MainLayout->addWidget(BpBar);// right to list

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);

    setLayout(MainLayout);
}

void BreakPointListWindow::insertBreakPoint(const uint32_t &ID, const char *filepath, const int &line) const {
    auto *item = new QTreeWidgetItem(ID);            // any type
    item->setIcon(0, QIcon(IconFactory::BreakPoint));// BP. icon
    item->setText(0, QString::number(ID));
    item->setText(1, QFileInfo(filepath).fileName());
    item->setToolTip(1, filepath);
    item->setText(2, QString::number(line));
    BpList->addTopLevelItem(item);
}


DebuggerDock::DebuggerDock(QWidget *parent) : QDockWidget(parent) {
    createToolBar();
    createCallStackWindow();
    createBreakPointList();

    createConsole();

    setWindowTitle("Debugger Dock");
    setFeatures(AllDockWidgetFeatures);
    setVisible(false);

    setWidget(Console);

    createControlTitleBar();
    setTitleBarWidget(TitleControlBar);
}

void DebuggerDock::createConsole() {

    ConsoleTab = new QTabWidget(this);
    Console = new QWidget(this);
    MainConsoleLayout = new QHBoxLayout();
    console_out_in = new QVBoxLayout();
    debug_output = new QPlainTextEdit(this);
    DebuggerPrompt = new QLineEdit(this);
    completer = new QCompleter(this);
    DebuggerOutput = new QWidget(this);

    VariablesView = new QTreeWidget(this);
    VariablesView->setHeaderHidden(true);
    // not editable yet, later possible to change values
    VariablesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QFont font;
    font.setPixelSize(17);
    VariablesView->setFont(font);
    auto *delegate = new Delegate();
    VariablesView->setItemDelegate(delegate);

    debug_output->setReadOnly(true);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    // TODO: set data to completer somehow
    DebuggerPrompt->setCompleter(completer);

    console_out_in->addWidget(debug_output);
    console_out_in->addWidget(DebuggerPrompt);
    console_out_in->setContentsMargins(0, 0, 0, 0);
    //Console->setFixedHeight(200); // !!!!! splitter will not show

    DebuggerOutput->setLayout(console_out_in);
    DebuggerOutput->setContentsMargins(0, 0, 0, 0);

    ConsoleTab->addTab(DebuggerOutput, "Debugger Output");
    ConsoleTab->addTab(VariablesView, "Variables");


    MainConsoleLayout->addWidget(DebugToolBar);
    MainConsoleLayout->addLayout(CallStackLayout);
    MainConsoleLayout->addWidget(ConsoleTab);
    MainConsoleLayout->addWidget(BreakPoint_List);

    MainConsoleLayout->setContentsMargins(0, 0, 0, 0);
    MainConsoleLayout->setSpacing(1);

    Console->setLayout(MainConsoleLayout);
}

void DebuggerDock::createToolBar() {
    DebugToolBar = new QToolBar(this);
    DebugToolBar->setOrientation(Qt::Vertical);
    DebugToolBar->setFixedWidth(35);

    btn_StartDebug = new QToolButton(this);
    btn_StartDebug->setIcon(QIcon(IconFactory::StartDebug));
    btn_StartDebug->setToolTip("Start");
    DebugToolBar->addWidget(btn_StartDebug);

    btn_StopDebug = new QToolButton(this);
    btn_StopDebug->setIcon(QIcon(IconFactory::StopDebug));
    btn_StopDebug->setToolTip("Stop");
    DebugToolBar->addWidget(btn_StopDebug);
    //DebugToolBar->addSeparator(); // cause of error

    btn_RunToCursor = new QToolButton(this);
    btn_RunToCursor->setIcon(QIcon(IconFactory::RunToCursor));
    btn_RunToCursor->setToolTip("Cursor for position");
    DebugToolBar->addWidget(btn_RunToCursor);

    btn_StartDebug->setEnabled(true);// at start i can run a process, but not stop it
    btn_StopDebug->setEnabled(false);
    btn_RunToCursor->setEnabled(false);

}

void DebuggerDock::createControlTitleBar() {
    TitleControlBar = new QToolBar(this);
    TitleControlBar->setOrientation(Qt::Horizontal);
    TitleControlBar->setFixedHeight(30);

    // call stack width + toolbar width
    auto *spacer = new QWidget(this);
    spacer->setFixedWidth(200 + 30);
    TitleControlBar->addWidget(spacer);

    btn_StepOver = new QToolButton(this);
    btn_StepOver->setToolTip("Step Over");
    btn_StepOver->setIcon(QIcon(IconFactory::NextLine));
    btn_StepOver->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    TitleControlBar->addWidget(btn_StepOver);

    btn_StepInto = new QToolButton(this);
    btn_StepInto->setToolTip("Step Into");
    btn_StepInto->setIcon(QIcon(IconFactory::StepInto));
    btn_StepInto->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    TitleControlBar->addWidget(btn_StepInto);

    btn_StepOut = new QToolButton(this);
    btn_StepOut->setToolTip("Step Out");
    btn_StepOut->setIcon(QIcon(IconFactory::GetOutOfFunction));
    btn_StepOut->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    TitleControlBar->addWidget(btn_StepOut);

    btn_StepInstruction = new QToolButton(this);
    btn_StepInstruction->setToolTip("Step Instruction");
    btn_StepInstruction->setIcon(QIcon(IconFactory::StepInstruction));
    btn_StepInstruction->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    TitleControlBar->addWidget(btn_StepInstruction);

    btn_Continue = new QToolButton(this);
    btn_Continue->setToolTip("Continue");
    btn_Continue->setIcon(QIcon(IconFactory::Resume));
    btn_Continue->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    TitleControlBar->addWidget(btn_Continue);

    TitleControlBar->addSeparator();
    btn_addWatch = new QToolButton(this);
    btn_addWatch->setIcon(QIcon(IconFactory::AddWatch));
    btn_addWatch->setToolTip("Add Watch");
    TitleControlBar->addWidget(btn_addWatch);

    auto *spacer2 = new QWidget(this);// align to right with blank widget
    spacer2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    TitleControlBar->addWidget(spacer2);
    TitleControlBar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));

    // at start i can not do anything, only when the process is in progress and breakpoint is hit or else
    btn_StepOver->setEnabled(false);
    btn_StepInto->setEnabled(false);
    btn_StepInstruction->setEnabled(false);
    btn_Continue->setEnabled(false);
    btn_StepOut->setEnabled(false);
}

void DebuggerDock::createCallStackWindow() {
    CallStackLayout = new QVBoxLayout();
    ThreadBox = new QComboBox(this);
    CallStack = new QListWidget(this);

    CallStackLayout->setContentsMargins(0, 0, 5, 0);
    CallStackLayout->setSpacing(0);
    ThreadBox->setFixedWidth(200);
    ThreadBox->addItem("Thread");// this will be removed when filling
    CallStack->setFixedWidth(200);

    CallStackLayout->addWidget(ThreadBox);
    CallStackLayout->addWidget(CallStack);
}
void DebuggerDock::createBreakPointList() {
    BreakPoint_List = new BreakPointListWindow(this);
}
