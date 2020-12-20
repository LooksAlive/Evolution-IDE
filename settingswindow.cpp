#include "settingswindow.h"



SettingsWindow::SettingsWindow(QWidget *parent) : QWidget(parent)
{
    // features
    SettingsWindow::setWindowTitle("Settings");
    SettingsWindow::setMinimumSize(400, 250);

    OuterLayout = new QVBoxLayout(this);
    OuterLayout->addLayout(buildForm());
    OuterLayout->addWidget(buildButtonBox());
    OuterLayout->setContentsMargins(4, 4, 4, 4);
    OuterLayout->setSpacing(2);
    SettingsWindow::setAttribute(Qt::WA_DeleteOnClose);




}

QWidget *SettingsWindow::buildButtonBox()
{
    m_ButtonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
    /*
    connect(m_ButtonBox, &QDialogButtonBox::accepted, m_AppearanceSettingsWidget, &AppearanceSettingsWidget::save);
    connect(m_ButtonBox, &QDialogButtonBox::accepted, m_BinarySettingsWidget, &BinarySettingsWidget::save);
    connect(m_ButtonBox, &QDialogButtonBox::accepted, m_SigningConfigWidget, &SigningConfigWidget::save);
    connect(m_ButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_ButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    */
    return m_ButtonBox;
}

QLayout *SettingsWindow::buildForm()
{
    InnerLayout = new QHBoxLayout(this);
    OptionsList = new QListWidget(this);
    WidgetStack = new QStackedWidget(this);

    InnerLayout->addWidget(OptionsList, 1);
    OptionsList->addItem(new QListWidgetItem("General"));
    OptionsList->addItem(new QListWidgetItem("Appearance"));
    OptionsList->addItem(new QListWidgetItem("Compiler"));
    OptionsList->addItem(new QListWidgetItem("Cmake"));
    OptionsList->addItem(new QListWidgetItem("Debugger"));
    OptionsList->addItem(new QListWidgetItem("Formater"));
    OptionsList->addItem(new QListWidgetItem("Git"));

    OptionsList->setCurrentRow(0);
    InnerLayout->addWidget(WidgetStack, 3);
    WidgetStack->addWidget(new QPlainTextEdit(this)); // 6 more
    connect(OptionsList, &QListWidget::currentRowChanged, WidgetStack, &QStackedWidget::setCurrentIndex);

    return InnerLayout;
}
