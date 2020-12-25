#include "settingswindow.h"



SettingsWindow::SettingsWindow(QWidget *parent) : QDialog(parent)
{
    // features
    SettingsWindow::setWindowTitle("Settings");
    SettingsWindow::setMinimumSize(500, 250);

    OuterLayout = new QVBoxLayout(this);
    OuterLayout->addLayout(buildForm());
    OuterLayout->addWidget(buildButtonBox());
    OuterLayout->setContentsMargins(4, 4, 4, 4);
    OuterLayout->setSpacing(2);
    SettingsWindow::setAttribute(Qt::WA_DeleteOnClose);




}

QWidget *SettingsWindow::buildButtonBox()
{
    ButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this); // Save

    /*
    connect(m_ButtonBox, &QDialogButtonBox::accepted, m_AppearanceSettingsWidget, &AppearanceSettingsWidget::save);
    connect(m_ButtonBox, &QDialogButtonBox::accepted, m_BinarySettingsWidget, &BinarySettingsWidget::save);
    connect(m_ButtonBox, &QDialogButtonBox::accepted, m_SigningConfigWidget, &SigningConfigWidget::save);
    */
    connect(ButtonBox, &QDialogButtonBox::accepted, this, &SettingsWindow::accept);
    connect(ButtonBox, &QDialogButtonBox::rejected, this, &SettingsWindow::reject);

    return ButtonBox;
}

QLayout *SettingsWindow::buildForm()
{
    InnerLayout = new QHBoxLayout();
    OptionsList = new QListWidget();
    WidgetStack = new QStackedWidget();

    cmake = new CmageGeneratorWidget();
    git = new GitWidget();

    InnerLayout->addWidget(OptionsList, 1);
    OptionsList->addItem(new QListWidgetItem("General"));
    OptionsList->addItem(new QListWidgetItem("Appearance"));
    OptionsList->addItem(new QListWidgetItem("Compiler"));
    OptionsList->addItem(new QListWidgetItem("Cmake"));
    OptionsList->addItem(new QListWidgetItem("Debugger"));
    OptionsList->addItem(new QListWidgetItem("Formater"));
    OptionsList->addItem(new QListWidgetItem("Git"));

    OptionsList->setCurrentRow(0);
    OptionsList->setMaximumWidth(125);
    InnerLayout->addWidget(WidgetStack);
    WidgetStack->addWidget(new QPlainTextEdit(this));
    WidgetStack->addWidget(new QPlainTextEdit(this));
    WidgetStack->addWidget(new QPlainTextEdit(this));
    WidgetStack->addWidget(cmake);
    WidgetStack->addWidget(new QPlainTextEdit(this));
    WidgetStack->addWidget(new QPlainTextEdit(this));
    WidgetStack->addWidget(git);
    connect(OptionsList, &QListWidget::currentRowChanged, WidgetStack, &QStackedWidget::setCurrentIndex);

    return InnerLayout;
}
