#include "settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) : QDialog(parent) {
    // features
    setWindowTitle("Settings");
    setMinimumSize(500, 400);

    OuterLayout = new QVBoxLayout(this);
    createButtons();
    OuterLayout->addLayout(buildForm());
    OuterLayout->setContentsMargins(0, 0, 0, 0);
    OuterLayout->setSpacing(2);
    setAttribute(Qt::WA_DeleteOnClose);

    slotLoadData();
}

void SettingsWindow::createButtons() {
    buttonsLayout = new QHBoxLayout();
    btn_save = new QPushButton(this);
    btn_close = new QPushButton(this);
    auto *spacer = new QWidget(this);// align to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    btn_save->setText("Save");
    btn_close->setText("Close");
    btn_save->setFixedWidth(60);
    btn_close->setFixedWidth(60);

    connect(btn_save, SIGNAL(clicked()), this, SLOT(slotSaveData()));
    connect(btn_close, SIGNAL(clicked()), this, SLOT(close()));

    buttonsLayout->addWidget(spacer);
    buttonsLayout->addWidget(btn_save);
    buttonsLayout->addWidget(btn_close);
}

QLayout *SettingsWindow::buildForm() {
    InnerLayout = new QHBoxLayout();
    OuterLayout = new QVBoxLayout();
    WidgetStack = new QStackedWidget();
    OptionsList = new QListWidget(this);

    cmake = new CMakeGeneratorWidget(this);
    git = new GitWidget(this);
    appearence = new AppearenceWidget(this);
    debugger = new DebuggerSettings(this);
    clangFormat = new ClangFormat(this);
    installer = new Installer(this);

    InnerLayout->addWidget(OptionsList, 1);
    OptionsList->addItem(new QListWidgetItem("Appearance"));
    OptionsList->addItem(new QListWidgetItem("Cmake"));
    OptionsList->addItem(new QListWidgetItem("Debugger"));
    OptionsList->addItem(new QListWidgetItem("Clang Format"));
    OptionsList->addItem(new QListWidgetItem("Git"));
    OptionsList->addItem(new QListWidgetItem("Installer"));

    OptionsList->setCurrentRow(0);
    OptionsList->setMaximumWidth(125);
    InnerLayout->addWidget(WidgetStack);

    WidgetStack->addWidget(appearence);
    WidgetStack->addWidget(cmake);
    WidgetStack->addWidget(debugger);
    WidgetStack->addWidget(clangFormat);
    WidgetStack->addWidget(git);
    WidgetStack->addWidget(installer);
    // they are in list statically places, so i can switch them simply by index and row
    connect(OptionsList, &QListWidget::currentRowChanged, WidgetStack, &QStackedWidget::setCurrentIndex);

    OuterLayout->addLayout(InnerLayout);
    OuterLayout->addLayout(buttonsLayout);
    return OuterLayout;
}

void SettingsWindow::slotSaveData() {
    cmake->saveData();
    git->saveData();
    appearence->saveData();
    debugger->saveData();
    clangFormat->saveData();
    installer->saveData();

    close();
}

void SettingsWindow::slotLoadData() {
    cmake->loadData();
    git->loadData();
    appearence->loadData();
    debugger->loadData();
    clangFormat->loadData();
    installer->loadData();
}
