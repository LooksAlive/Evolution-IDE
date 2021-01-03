#include "debuggerwidget.h"

DebuggerWidget::DebuggerWidget(QWidget *parent) : QWidget(parent)
{
    MainWindowLayout = new QHBoxLayout();
    SourceConsoleLayout = new QVBoxLayout();
    setWindowTitle("debugger");
    buildConsole();
    buildDebugVariableWindow();
    source_view = new PlainTextEdit(this);

    SourceConsoleLayout->addWidget(source_view);
    SourceConsoleLayout->addWidget(Console);

    /*
    splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(Console);
    splitter->addWidget(source_view);

    SourceConsoleLayout->addWidget(splitter);
    */
    MainWindowLayout->addLayout(SourceConsoleLayout);
    MainWindowLayout->addWidget(debug_variable_window);

    setLayout(MainWindowLayout);

}

DebuggerWidget::~DebuggerWidget(){}


void DebuggerWidget::buildConsole(){

    Console = new QWidget(this);
    MainConsoleLayout = new QHBoxLayout(this);
    debug_output = new PlainTextEdit(this);

    Console->setFixedHeight(230);
    //Console->setStyleSheet("border: 1px solid rgb(0, 255, 0);");

    createControlPanel();

    MainConsoleLayout->addLayout(ControlPanel);
    MainConsoleLayout->addWidget(debug_output);

    Console->setLayout(MainConsoleLayout);
}

void DebuggerWidget::createToolBar()
{
    QString tmpToolTip;

    DebugToolBar=new QToolBar(this);
    DebugToolBar->setContentsMargins(0,0,0,0);

    tblStartDebug=new QToolButton(this);
    tblStartDebug->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/StartDebug.png"));
    tblStartDebug->setFixedSize(26,26);
    tmpToolTip=tr("Start debugging.");
    tblStartDebug->setToolTip(tmpToolTip);
    //connect(tblStartDebug,SIGNAL(clicked()),
    //        this,SLOT(onStartDebugButtonClicked()));
    DebugToolBar->addWidget(tblStartDebug);

    tblStopDebug=new QToolButton(this);
    tblStopDebug->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/StopDebug.png"));
    tblStopDebug->setFixedSize(26,26);
    DebugToolBar->addWidget(tblStopDebug);
    DebugToolBar->addSeparator();

    tblRunToCursor=new QToolButton(this);
    tblRunToCursor->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/RunToCursor.png"));
    tblRunToCursor->setFixedSize(26,26);
    DebugToolBar->addWidget(tblRunToCursor);

    ControlPanel->addWidget(DebugToolBar);
}

void DebuggerWidget::createControlPanel()
{

    ControlPanel=new QVBoxLayout;
    ControlPanel->setContentsMargins(0,0,0,0);
    ControlPanel->setSpacing(0);

    createToolBar();

    tblNextLine=new QToolButton(this);
    tblNextLine->setText(QString(" " + tr("Next Step")));
    tblNextLine->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/NextStep.png"));
    tblNextLine->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tblNextLine->setFixedSize(160,30);
    ControlPanel->addWidget(tblNextLine);
    tblIntoLine=new QToolButton(this);
    tblIntoLine->setText(QString(" " + tr("Into Line")));
    tblIntoLine->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/NextLine.png"));
    tblIntoLine->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tblIntoLine->setFixedSize(160,30);
    ControlPanel->addWidget(tblIntoLine);
    tblContinue=new QToolButton(this);
    tblContinue->setText(QString(" " + tr("Continue")));
    tblContinue->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/Continue.png"));
    tblContinue->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tblContinue->setFixedSize(160,30);
    ControlPanel->addWidget(tblContinue);
    tblNextInstruction=new QToolButton(this);
    tblNextInstruction->setText(QString(" " + tr("Next Instruction")));
    tblNextInstruction->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/JumpFunction.png"));
    tblNextInstruction->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tblNextInstruction->setFixedSize(160,30);
    ControlPanel->addWidget(tblNextInstruction);
    tblIntoInstruction=new QToolButton(this);
    tblIntoInstruction->setText(QString(" " + tr("Into Instruction")));
    tblIntoInstruction->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/GetIntoFunction.png"));
    tblIntoInstruction->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tblIntoInstruction->setFixedSize(160,30);
    ControlPanel->addWidget(tblIntoInstruction);
    tblSkipFunction=new QToolButton(this);
    tblSkipFunction->setText(QString(" " + tr("Skip Function")));
    tblSkipFunction->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/Debugger/GetOutOfFunction.png"));
    tblSkipFunction->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tblSkipFunction->setFixedSize(160,30);
    ControlPanel->addWidget(tblSkipFunction);
    ControlPanel->addStretch();
}


void DebuggerWidget::buildDebugVariableWindow() {

    debug_variable_window = new QWidget(this);
    var_layout = new QVBoxLayout(this);
    debug_variable_window->setMaximumWidth(250); // for now ... later change responsibly

    var_layout->addWidget(new PlainTextEdit);
    debug_variable_window->setLayout(var_layout);

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

}
