#include <QFormLayout>
#include <QIcon>
#include <QPushButton>
#include <QVBoxLayout>
#include <QShortcut>
#include <QStandardItem>

#include "icons/IconFactory.h"
#include "findreplace.h"

FindReplaceWidget::FindReplaceWidget(Tab *tab, QWidget *parent)
    : QDockWidget(parent), m_Tab(tab)
{
    createWindow();

    MainLayout->setContentsMargins(0, 5, 0, 0);
    MainLayout->setSpacing(2);

    base = new QWidget(this);
    base->setLayout(MainLayout);
    base->setContentsMargins(5, 5, 5, 5);
    base->setFixedWidth(700);
    //base->setFixedHeight(100);
    setWidget(base);
    this->setVisible(false);

    setWindowTitle("Find & Replace");


    auto *btn = new QToolButton(this);
    btn->setIcon(QIcon(IconFactory::Remove));
    btn->setFixedWidth(30);
    btn->setWindowFlags(Qt::FramelessWindowHint);
    //btn->setSizePolicy(QWidget::Expanding);
    connect(btn, SIGNAL(clicked()), this, SLOT(close()));
    setTitleBarWidget(btn);

    // escape shortcut  -> will close the window
    connect(new QShortcut(Qt::Key_Escape, this), &QShortcut::activated, [=] {this->setVisible(false);});
    
    /* same_file == ""  --> means that file has no filepath yet (blank) */
    if(same_file != m_Tab->tabToolTip(m_Tab->currentIndex()) || same_file == ""){
        m_Edit = qobject_cast<PlainTextEdit*>(m_Tab->currentWidget());
        same_file = m_Tab->tabToolTip(m_Tab->currentIndex());
    }
    // clear search selection
    connect(this, SIGNAL(visibilityChanged(bool)), m_Edit, SLOT(slotVisible(bool)));
}

void FindReplaceWidget::createWindow() {
    MainLayout = new QVBoxLayout();
    UpLayout = new QHBoxLayout();
    input_layout = new QFormLayout();
    flags_layout = new QVBoxLayout();
    results = new QTreeView(this);
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

    results->setHeaderHidden(true);
    // not editable, later possible updates and auto refactoring edited item; by item function or like this
    results->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(results, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotForwardToResult(const QModelIndex &)));

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

    UpLayout->addLayout(input_layout);
    /*
    MainLayout->addWidget(next);
    MainLayout->addWidget(previous);
    MainLayout->addWidget(replace);
    MainLayout->addWidget(replaceall);
    */
    UpLayout->addLayout(find_layout);
    UpLayout->addLayout(replace_layout);
    UpLayout->addLayout(flags_layout);

    UpLayout->setContentsMargins(0, 0, 0, 0);
    UpLayout->setSpacing(2);

    MainLayout->addLayout(UpLayout);
    MainLayout->addWidget(results);
}

/* needs to be called everytime in case of changes */
void FindReplaceWidget::getOptionsAndTexts()
{
    LabelText->setText(QString());
    search_text = LineEditFind->text();
    replace_text = LineEditReplacement->text();
    // QTextDocument::FindBackward  ----> previous btn,  so we need to clear it every time as usual
    find_options = NULL;

    if (CaseSensitive->isChecked()) {
        find_options |= QTextDocument::FindCaseSensitively;
    }
    if (WholeWords->isChecked()) {
        find_options |= QTextDocument::FindWholeWords;
    }

    /* same_file == ""  --> means that file has no filepath yet (blank) */
    if(same_file != m_Tab->tabToolTip(m_Tab->currentIndex()) || same_file == ""){
        m_Edit = qobject_cast<PlainTextEdit*>(m_Tab->currentWidget());
        same_file = m_Tab->tabToolTip(m_Tab->currentIndex());
    }
}

void FindReplaceWidget::slotNext(){
    getOptionsAndTexts();

    if(search_text != temp_search_text){
        m_Edit->findStoreAndSelectAll(search_text, find_options);
        temp_search_text = search_text;

        // result tree:
        // remove all elements and insert new ones
        results->reset();
        if(m_Edit->search_results.empty()){
            m_Edit->findNext(search_text, find_options);
            return;
        }
        auto *model = new QStandardItemModel(this);
        QStandardItem *rootNode = model->invisibleRootItem();
        // for now only 1 file, later on figure out how to manage more files
        // maybe consider (no idea) ...
        auto *file = new QStandardItem(m_Edit->search_results[0].fileName);
        rootNode->appendRow(file);
        for (auto & elem : m_Edit->search_results) {
            QString row_col = QString::number(elem.row) + ":" + QString::number(elem.col);
            QString line_content = m_Edit->getLineContent(elem.row);
            QString all = line_content + "   " + row_col;
            // QString line; // later append line content and beginning + row_col follows getLineUnderCursor();
            auto *pos = new QStandardItem(all);
            file->appendRow(pos);
        }

        results->setModel(model);
        results->collapseAll();
    }
    m_Edit->findNext(search_text, find_options);
}

void FindReplaceWidget::slotPrevious(){
    getOptionsAndTexts();
    find_options |= QTextDocument::FindBackward;   // flag for previous search
    m_Edit->findNext(search_text, find_options);
}

void FindReplaceWidget::slotReplace(){
    getOptionsAndTexts();
    m_Edit->replace(search_text, replace_text, find_options);
}

void FindReplaceWidget::slotReplaceAll()
{
    getOptionsAndTexts();
    int occurrences = m_Edit->replaceAll(search_text, replace_text, find_options);
    LabelText->setText(tr("Replace %1 occurrences of the search term.").arg(occurrences));
}

void FindReplaceWidget::slotForwardToResult(const QModelIndex &index) {
    // do this a lot better :)
    m_Edit->setCursorPosition(m_Edit->search_results[index.row()].row, m_Edit->search_results[index.row()].col);
}

void FindReplaceWidget::slotVisible(bool visible) {
    if(!visible){
        m_Edit->clearSelectionsBySearch = visible;
        m_Edit->updateExtraSelections();
    }
}

