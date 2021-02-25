#include <QFormLayout>
#include <QIcon>
#include <QPushButton>
#include <QVBoxLayout>
#include <QShortcut>
#include <QStandardItem>
#include <QToolBar>

#include "icons/IconFactory.h"
#include "findreplace.h"

FindReplaceWidget::FindReplaceWidget(Tab *tab, QWidget *parent)
    : QDockWidget(parent), m_Tab(tab)
{
    createWindow();

    setVisible(false);

    setFeatures(AllDockWidgetFeatures);
    setWindowTitle("Find & Replace");

    // escape shortcut  -> will close the window
    connect(new QShortcut(Qt::Key_Escape, this), &QShortcut::activated, [=] { this->setVisible(false); });

    /* same_file == ""  --> means that file has no filepath yet (blank) */
    if (same_file != m_Tab->tabToolTip(m_Tab->currentIndex()) || same_file == "") {
        m_Edit = qobject_cast<PlainTextEdit *>(m_Tab->currentWidget());
        same_file = m_Tab->tabToolTip(m_Tab->currentIndex());
    }
    // clear search selection
    connect(this, SIGNAL(visibilityChanged(bool)), m_Edit, SLOT(slotVisible(bool)));
}

void FindReplaceWidget::createWindow() {
    MainLayout = new QHBoxLayout();
    TitleBar = new QToolBar(this);
    results = new QTreeView(this);
    preview = new PlainTextEdit(this);
    LineEditFind = new QLineEdit(this);
    LineEditReplacement = new QLineEdit(this);
    labelOccurences = new QLabel("0/0", this);// 0/0 -> x/z results
    menu = new QMenu(this);
    find_options_menu_button = new QToolButton(this);
    caseSensitive = new QAction(this);
    wholeWords = new QAction(this);

    LineEditFind->setFocus();
    LineEditFind->setMaximumWidth(250);
    LineEditFind->setToolTip("Search with");
    LineEditReplacement->setMaximumWidth(250);
    LineEditReplacement->setToolTip("Replace with");
    labelOccurences->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    //labelOccurences->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    caseSensitive->setText("Case sensitive");
    caseSensitive->setCheckable(true);
    caseSensitive->setChecked(false);
    wholeWords->setText("Whole Words");
    wholeWords->setCheckable(true);
    wholeWords->setChecked(false);
    menu->addAction(caseSensitive);
    menu->addAction(wholeWords);

    find_options_menu_button->setMenu(menu);
    find_options_menu_button->setIcon(QIcon(IconFactory::Plus));

    find_options_menu_button->setContextMenuPolicy(Qt::CustomContextMenu);
    // FIXME: this works only for right click !!! , but at least works :)
    connect(find_options_menu_button, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slotShowMenu(const QPoint &)));

    results->setHeaderHidden(true);
    // not editable, later possible updates and auto refactoring edited item; by item function or like this
    results->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QFont font;
    font.setPixelSize(15);
    //results->setStyleSheet("border: 5px solid;");
    results->setFont(font);
    // go to file in main view
    connect(results, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotForwardToResult(const QModelIndex &)));
    // go to file in preview
    connect(results, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotShowPreviewResult(const QModelIndex &)));

    preview->setReadOnly(false);
    preview->setEnabled(true);

    MainLayout->addWidget(results);
    MainLayout->addWidget(preview);

    next = new QToolButton(this);
    next->setToolTip("Next");
    previous = new QToolButton(this);
    previous->setToolTip("Previous");
    replace = new QToolButton(this);
    replace->setToolTip("Replace");
    replaceall = new QToolButton(this);
    replaceall->setToolTip("Replace All");
    next->setIcon(QIcon(IconFactory::ScrollDown));
    previous->setIcon(QIcon(IconFactory::ScrollUp));
    replace->setIcon(QIcon(IconFactory::Replace));
    replaceall->setIcon(QIcon(IconFactory::Replace));

    // enter shortcut
    connect(LineEditFind, SIGNAL(returnPressed()), this, SLOT(slotNext()));
    connect(next, SIGNAL(clicked()), this, SLOT(slotNext()));
    connect(previous, SIGNAL(clicked()), this, SLOT(slotPrevious()));
    connect(replace, SIGNAL(clicked()), this, SLOT(slotReplace()));
    connect(replaceall, SIGNAL(clicked()), this, SLOT(slotReplaceAll()));

    TitleBar->addWidget(find_options_menu_button);
    TitleBar->addWidget(LineEditFind);
    TitleBar->addSeparator();
    TitleBar->addWidget(LineEditReplacement);
    TitleBar->addSeparator();
    TitleBar->addWidget(labelOccurences);
    TitleBar->addSeparator();
    TitleBar->addWidget(next);
    TitleBar->addWidget(previous);
    TitleBar->addWidget(replace);
    TitleBar->addWidget(replaceall);

    auto *spacer = new QWidget(this);// align to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //t_bar->setStyleSheet("background:red;");
    //spacer->setStyleSheet("background:green;");

    TitleBar->addWidget(spacer);
    TitleBar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));
    TitleBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    TitleBar->setFixedHeight(30);
    setTitleBarWidget(TitleBar);

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(2);

    base = new QWidget(this);
    setWidget(base);
    base->setLayout(MainLayout);
    //MainLayout->addWidget(results);
}

/* needs to be called everytime in case of changes */
void FindReplaceWidget::getOptionsAndTexts()
{
    labelOccurences->setText(QString());
    search_text = LineEditFind->text();
    replace_text = LineEditReplacement->text();
    // QTextDocument::FindBackward  ----> previous btn,  so we need to clear it every time as usual
    find_options = NULL;

    if (caseSensitive->isChecked()) {
        find_options |= QTextDocument::FindCaseSensitively;
    }
    if (wholeWords->isChecked()) {
        find_options |= QTextDocument::FindWholeWords;
    }
    // regex removed ...

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
        for (const auto &elem : m_Edit->search_results) {
            QString row_col = QString::number(elem.row + 1) + ":" + QString::number(elem.col + 1);
            QString line_content = m_Edit->getLineContent(elem.row + 1);
            QString all = line_content + "\t" + row_col;
            // QString line; // later append line content and beginning + row_col follows getLineUnderCursor();
            auto *pos = new QStandardItem(all);
            // pos->setBackground(QColor(Qt::red));
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
    labelOccurences->setText(tr("Replace %1 occurrences of the search term.").arg(occurrences));
}

void FindReplaceWidget::slotForwardToResult(const QModelIndex &index) {
    // do this a lot better :)
    m_Edit->setCursorPosition(m_Edit->search_results[index.row()].row, m_Edit->search_results[index.row()].col);
}

void FindReplaceWidget::slotVisible(bool visible) {
    if(!visible){
        m_Edit->extra_selections_search_results.clear();
        m_Edit->updateExtraSelections();
    }
}

void FindReplaceWidget::slotShowPreviewResult(const QModelIndex &index) {
    // TODO: do i need to copy selections ???, if editors are connected to same document pointer ...  , are they visible ?
    preview->setDocument(m_Edit->document());
    preview->setCursorPosition(m_Edit->search_results[index.row()].row, m_Edit->search_results[index.row()].col);
}

void FindReplaceWidget::slotShowMenu(const QPoint &pos) {
    menu->exec(mapToGlobal(pos));
}