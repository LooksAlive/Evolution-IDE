#include <QFormLayout>
#include <QGroupBox>
#include <QIcon>
#include <QPushButton>
#include <QVBoxLayout>
#include <QShortcut>

#include "icons/IconFactory.h"
#include "findreplace.h"

FindReplaceWidget::FindReplaceWidget(Tab *tab, QWidget *parent)
    : QDockWidget(parent), m_Tab(tab)
{
    createWindow();
    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(2);

    base = new QWidget(this);
    base->setLayout(MainLayout);
    base->setContentsMargins(5, 5, 5, 5);
    base->setFixedWidth(700);
    //base->setFixedHeight(100);
    setWidget(base);
    this->setVisible(false);

    setWindowTitle("Find & Replace");

    /*
    QToolButton *btn = new QToolButton(this);
    btn->setIcon(QIcon(IconFactory::Remove));
    btn->setFixedWidth(30);
    connect(btn, SIGNAL(clicked()), this, SLOT(close()));
    setTitleBarWidget(btn);
    */
    // escape shortcut  -> will close the window
    connect(new QShortcut(Qt::Key_Escape, this), &QShortcut::activated, [=] {this->setVisible(false);});
}

void FindReplaceWidget::createWindow() {
    MainLayout = new QHBoxLayout();
    input_layout = new QFormLayout();
    flags_layout = new QVBoxLayout();
    CaseSensitive = new QCheckBox("Case Sensitive", this);
    WholeWords = new QCheckBox("Whole Text", this);
    RegularExpression = new QCheckBox("Use Regexp", this);
    LineEditFind = new QLineEdit(this);
    LineEditReplacement = new QLineEdit(this);
    LabelText = new QLabel("0/0", this); // 0/0 -> x/z results
    LineEditFind->setFocus();
    LineEditFind->setMaximumWidth(400);
    LineEditReplacement->setMaximumWidth(400);
    LabelText->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    LabelText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    next = new QPushButton("Next", this);
    previous = new QPushButton("Previous", this);
    replace = new QPushButton("Replace", this);
    replaceall = new QPushButton("Replace All", this);
    next->setIcon(QIcon(IconFactory::Search));

    // enter shortcut
    connect(LineEditFind, SIGNAL(returnPressed()), this, SLOT(slotNext()));
    connect(next, SIGNAL(clicked()), this, SLOT(slotNext()));
    connect(previous, SIGNAL(clicked()), this, SLOT(slotPrevious()));
    connect(replace, SIGNAL(clicked()), this, SLOT(slotReplace()));
    connect(replaceall, SIGNAL(clicked()), this, SLOT(slotReplaceAll()));

    flags_layout->addWidget(CaseSensitive);
    flags_layout->addWidget(WholeWords);
    flags_layout->addWidget(RegularExpression);

    input_layout->addRow("Find:  ", LineEditFind);
    input_layout->addRow("Replace:  ", LineEditReplacement);
    input_layout->addWidget(LabelText);

    auto *find_layout = new QVBoxLayout();
    auto *replace_layout = new QVBoxLayout();

    find_layout->addWidget(next);
    find_layout->addWidget(previous);
    replace_layout->addWidget(replace);
    replace_layout->addWidget(replaceall);

    MainLayout->addLayout(input_layout);
    /*
    MainLayout->addWidget(next);
    MainLayout->addWidget(previous);
    MainLayout->addWidget(replace);
    MainLayout->addWidget(replaceall);
    */
    MainLayout->addLayout(find_layout);
    MainLayout->addLayout(replace_layout);
    MainLayout->addLayout(flags_layout);
}

/* needs to be called everytime in case of changes */
void FindReplaceWidget::getOptionsAndTexts()
{
    LabelText->setText(QString());
    search_text = LineEditFind->text();
    replace_text = LineEditReplacement->text();
    find_options = NULL;

    if (CaseSensitive->isChecked()) {
        find_options |= QTextDocument::FindCaseSensitively;
    }
    if (WholeWords->isChecked()) {
        find_options |= QTextDocument::FindWholeWords;
    }
    // QTextDocument::FindBackward  ----> previous,  next also do not work(stacked on 1 found)
}

void FindReplaceWidget::slotNext(){
    /* same_file == ""  --> means that file has no filepath yet (blank) */
    if(same_file != m_Tab->tabToolTip(m_Tab->currentIndex()) || same_file == ""){
        m_Edit = qobject_cast<PlainTextEdit*>(m_Tab->currentWidget());
        same_file = m_Tab->tabToolTip(m_Tab->currentIndex());
    }
    getOptionsAndTexts();
    m_Edit->findNext(search_text, find_options);
}
// figure it out later
void FindReplaceWidget::slotPrevious(){
    if(same_file != m_Tab->tabToolTip(m_Tab->currentIndex()) || same_file == ""){
        m_Edit = qobject_cast<PlainTextEdit*>(m_Tab->currentWidget());
        same_file = m_Tab->tabToolTip(m_Tab->currentIndex());
    }
    getOptionsAndTexts();
    find_options |= QTextDocument::FindBackward;   // here flag for previous search
    m_Edit->findNext(search_text, find_options);
}

void FindReplaceWidget::slotReplace(){
    if(same_file != m_Tab->tabToolTip(m_Tab->currentIndex()) || same_file == ""){
        m_Edit = qobject_cast<PlainTextEdit*>(m_Tab->currentWidget());
        same_file = m_Tab->tabToolTip(m_Tab->currentIndex());
    }
    getOptionsAndTexts();
    m_Edit->replace(search_text, replace_text);
}

void FindReplaceWidget::slotReplaceAll()
{
    if(same_file != m_Tab->tabToolTip(m_Tab->currentIndex()) || same_file == ""){
        m_Edit = qobject_cast<PlainTextEdit*>(m_Tab->currentWidget());
        same_file = m_Tab->tabToolTip(m_Tab->currentIndex());
    }
    getOptionsAndTexts();

    int occurrences = m_Edit->replaceAll(search_text, replace_text);
    LabelText->setText(tr("Replace %1 occurrences of the search term.").arg(occurrences));
}

