#include <QFormLayout>
#include <QGroupBox>
#include <QIcon>
#include <QPushButton>
#include <QVBoxLayout>

#include "findreplace.h"

FindReplaceWidget::FindReplaceWidget(Tab *tab, QWidget *parent)
    : QDockWidget(parent), m_Tab(tab)
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addLayout(buildForm());
    layout->addWidget(buildButtonBox());
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(2);

    base = new QWidget(this);
    base->setLayout(layout);
    setWidget(base);
    setVisible(false);

    base->setMaximumSize(550, base->maximumHeight()); // needed to not expand more etc.
    //FindReplaceWidget::setMaximumSize(base->maximumWidth(), base->maximumHeight());
    // setWindowIcon(QIcon(replace ? ":/icons/fugue/edit-replace.png" : ":/icons/fugue/binocular.png"));

    setWindowTitle("Find & Replace");
    LineEditFind->setFocus();
    LineEditFind->setMaximumWidth(400);
    LineEditReplacement->setMaximumWidth(400);

    // track changing files here
    m_Edit = qobject_cast<PlainTextEdit*>(m_Tab->currentWidget());
    same_file = m_Tab->tabToolTip(m_Tab->currentIndex());

    connect(next, SIGNAL(clicked()), this, SLOT(slotNext()));
    connect(previous, SIGNAL(clicked()), this, SLOT(slotPrevious()));
    connect(replace, SIGNAL(clicked()), this, SLOT(slotReplace()));
    connect(replaceall, SIGNAL(clicked()), this, SLOT(slotReplaceAll()));
}

/* next, previous, replce, replace all buttons */
QWidget *FindReplaceWidget::buildButtonBox()
{
    QGroupBox *button_box = new QGroupBox();
    QVBoxLayout *layout = new QVBoxLayout();

    next = new QPushButton(this);
    previous = new QPushButton(this);
    replace = new QPushButton(this);
    replaceall = new QPushButton(this);

    next->setText("Next");
    next->setIcon(QIcon("/home/adam/Desktop/sources/Evolution-IDE/icons/search.svg"));
    previous->setText("Previous");
    replace->setText("Replace");
    replaceall->setText("Replace All");

    layout->addWidget(next);
    layout->addWidget(previous);
    layout->addWidget(replace);
    layout->addWidget(replaceall);
    button_box->setLayout(layout);

    return button_box;
}

/* check boxes, line edit, labels */
QLayout *FindReplaceWidget::buildForm()
{
    QVBoxLayout *flags = new QVBoxLayout();
    QFormLayout *form = new QFormLayout();
    CaseSensitive = new QCheckBox("Case Sensitive", this);
    WholeWords = new QCheckBox("Whole Text", this);
    RegularExpression = new QCheckBox("Use Regexp", this);
    LineEditFind = new QLineEdit(this);
    LineEditReplacement = new QLineEdit(this);
    LabelText = new QLabel("0/0", this); // 0/0 -> x/z results


    flags->addWidget(CaseSensitive);
    flags->addWidget(WholeWords);
    flags->addWidget(RegularExpression);
    form->addRow("Find:  ", LineEditFind);
    form->addRow("Replace:  ", LineEditReplacement);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(form);
    layout->addLayout(flags);
    layout->addWidget(LabelText);
    LabelText->setAlignment(Qt::AlignTop | Qt::AlignRight);
    LabelText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    return layout;
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

