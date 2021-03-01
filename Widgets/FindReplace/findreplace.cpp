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
    connect(new QShortcut(Qt::Key_Escape, this), &QShortcut::activated, [=] { setVisible(false); titleBarWidget()->setEnabled(true); });

    /* same_file == ""  --> means that file has no filepath yet (blank) */
    if (same_file != m_Tab->tabToolTip(m_Tab->currentIndex()) || same_file == "") {
        m_Edit = qobject_cast<PlainTextEdit *>(m_Tab->currentWidget());
        same_file = m_Tab->tabToolTip(m_Tab->currentIndex());
    }
    // clear search selection
    connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(slotVisible(bool)));
}

void FindReplaceWidget::createWindow() {
    MainLayout = new QHBoxLayout();
    TitleBar = new QToolBar(this);
    results = new QTreeWidget(this);
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

    labelOccurences->setText("0/0");

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
    results->setFont(font);
    auto *delegate = new Delegate();
    results->setItemDelegate(delegate);
    // go to file in main view
    //connect(results, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotForwardToResult(const QModelIndex &)));
    // go to file in preview
    connect(results, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(slotShowPreviewResult(QTreeWidgetItem *, int)));

    // editing possible
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
    inCurrentFile = new QToolButton(this);
    inCurrentFile->setToolTip("Search Current File");
    everywhere = new QToolButton(this);
    everywhere->setToolTip("Search Everywhere");
    next->setIcon(QIcon(IconFactory::ScrollDown));
    previous->setIcon(QIcon(IconFactory::ScrollUp));
    replace->setIcon(QIcon(IconFactory::Replace));
    replaceall->setIcon(QIcon(IconFactory::Replace));
    inCurrentFile->setIcon(QIcon(IconFactory::CurrentFile));
    everywhere->setIcon(QIcon(IconFactory::AllFiles));

    // enter shortcut
    connect(LineEditFind, SIGNAL(returnPressed()), this, SLOT(slotNext()));
    connect(next, SIGNAL(clicked()), this, SLOT(slotNext()));
    connect(previous, SIGNAL(clicked()), this, SLOT(slotPrevious()));
    connect(replace, SIGNAL(clicked()), this, SLOT(slotReplace()));
    connect(replaceall, SIGNAL(clicked()), this, SLOT(slotReplaceAll()));
    connect(inCurrentFile, &QAbstractButton::clicked, this, [=] { SearchCurrentFile = true; SearchEverywhere = false; inCurrentFile->setEnabled(false); everywhere->setEnabled(true); });
    connect(everywhere, &QAbstractButton::clicked, this, [=] { SearchEverywhere = true; SearchCurrentFile = false; everywhere->setEnabled(false); inCurrentFile->setEnabled(true); });
    everywhere->setEnabled(false);// defines default property with SearchCurrentFile = true
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
    TitleBar->addSeparator();
    TitleBar->addWidget(inCurrentFile);
    TitleBar->addWidget(everywhere);


    auto *spacer = new QWidget(this);// align to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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
    if (same_file != m_Tab->tabToolTip(m_Tab->currentIndex()) || same_file == "") {
        m_Edit = qobject_cast<PlainTextEdit *>(m_Tab->currentWidget());
        same_file = m_Tab->tabToolTip(m_Tab->currentIndex());
    }
}

void FindReplaceWidget::savePreview() {
    FileDirManager file_manager;
    file_manager.write(temp_search_result_path, preview->toPlainText().toLatin1().data());
}

void FindReplaceWidget::searchEverywhere() {
    if (search_text == temp_search_text_everywhere) {
        return;
    }

    getOptionsAndTexts();

    results->clear();
    selections.clear();
    MultifileSearchResults.clear();
    search_text = temp_search_text_everywhere;

    FileDirManager file_manager;
    // search all files, help with PlainTextEdit methods by changing preview
    for (int i = 0; i < AllFiles.length(); i++) {
        const QString content = file_manager.read(AllFiles[i]);
        preview->appendPlainText(content);
        preview->setFilePath(AllFiles[i]);// needed for findStoreAndSelectAll[0] item which is filepath
        preview->findStoreAndSelectAll(search_text, find_options);
        if (preview->search_results.empty()) {
            continue;// continue with another file
        }
        // spaces are crytical here, we do not want to add some " "
        const QString Stag = "<span ";
        const QString Style = "style=color:orange;";
        const QString Mtag = " >";
        const QString Etag = "</span>";

        MultifileSearchResults.push_back(preview->search_results);
        selections.push_back(preview->extra_selections_search_results);
        // fill view
        auto *file = new QTreeWidgetItem();
        file->setText(0, preview->search_results[0].fileName);
        results->addTopLevelItem(file);
        for (const auto &elem : preview->search_results) {
            QString row_col = QString::number(elem.row) + ":" + QString::number(elem.col);
            QString line_content = preview->getLineContent(elem.row);
            // append html (highlight background for searched text)
            const QString start = line_content.mid(0, elem.col);
            const QString end = line_content.mid(start.length(), line_content.length());

            line_content = start + Stag + Style + Mtag + search_text + Etag + end;
            auto *pos = new QTreeWidgetItem();
            pos->setText(0, line_content + "    " + row_col);
            file->addChild(pos);
        }
    }
    results->collapseAll();

    // m_Edit not effective here, it would open many tabs, let this behavior to view (clicks)
    // m_Edit->findNext(search_text, find_options);
}


void FindReplaceWidget::slotNext() {
    // SearchCurrentFile
    getOptionsAndTexts();

    // TODO: consider new thread
    if (SearchEverywhere) {
        searchEverywhere();
        return;
    }

    if (search_text == temp_search_text) {// on the begining "" != "00"
        return;
    }

    selections.clear();
    MultifileSearchResults.clear();
    results->clear();
    temp_search_text = search_text;

    m_Edit->findStoreAndSelectAll(search_text, find_options);
    selections.push_back(m_Edit->extra_selections_search_results);// selections[0]

    if (m_Edit->search_results.empty()) {
        //m_Edit->findNext(search_text, find_options);
        labelOccurences->setText("0/0");
        return;
    }
    // spaces are crytical here, we do not want to add some " "
    const QString Stag = "<span ";
    const QString Style = "style=color:orange;";
    const QString Mtag = " >";
    const QString Etag = "</span>";

    auto *file = new QTreeWidgetItem();
    file->setText(0, m_Edit->search_results[0].fileName);
    results->addTopLevelItem(file);
    for (const auto &elem : m_Edit->search_results) {
        QString row_col = QString::number(elem.row) + ":" + QString::number(elem.col);
        QString line_content = m_Edit->getLineContent(elem.row);
        // append html (highlight background for searched text)
        const QString start = line_content.mid(0, elem.col);
        const QString end = line_content.mid(start.length(), line_content.length());

        line_content = start + Stag + Style + Mtag + search_text + Etag + end;
        qDebug() << line_content;
        auto *pos = new QTreeWidgetItem();
        pos->setText(0, line_content + "    " + row_col);
        file->addChild(pos);
    }
    results->collapseAll();

    m_Edit->findNext(search_text, find_options);
    // increase labelOccurences
    const QString label = labelOccurences->text();
    const int c = label.indexOf("/");
    int num = label.mid(0, c).toInt();
    if (num >= m_Edit->extra_selections_search_results.count()) {
        num = 0;
    }
    labelOccurences->setText(QString::number(num + 1) + "/" + QString::number(m_Edit->extra_selections_search_results.count()));
}

void FindReplaceWidget::slotPrevious() {
    getOptionsAndTexts();
    find_options |= QTextDocument::FindBackward;// flag for previous search
    m_Edit->findNext(search_text, find_options);

    m_Edit->findNext(search_text, find_options);
    // decrease labelOccurences
    const QString label = labelOccurences->text();
    const int c = label.indexOf("/");
    int num = label.mid(0, c).toInt();
    if (num <= 1) {
        num = m_Edit->extra_selections_search_results.count();
    }
    labelOccurences->setText(QString::number(num - 1) + "/" + QString::number(m_Edit->extra_selections_search_results.count()));
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

void FindReplaceWidget::slotVisible(bool visible) {
    if (!visible) {
        titleBarWidget()->setEnabled(true);// references were disabling/replacing this
        m_Edit->extra_selections_search_results.clear();
        m_Edit->updateExtraSelections();
    }
}

void FindReplaceWidget::forwardToResult(QTreeWidgetItem *item, int column) {
    Q_UNUSED(column);
    // we have to figure out in what file result is, if there are more of them
    const int filepath_row = results->indexOfTopLevelItem(item->parent());
    const int row = item->parent()->indexOfChild(item);

    m_Edit->setCursorPosition(m_Edit->search_results[row].row, m_Edit->search_results[row].col);
}

void FindReplaceWidget::slotShowPreviewResult(QTreeWidgetItem *item, int column) {
    Q_UNUSED(column);// basically it is 0 in code, but we have only 1 column soo
    // we have to figure out in what file result is, if there are more of them
    const int filepath_row = results->indexOfTopLevelItem(item->parent());
    const int row = item->parent()->indexOfChild(item);
    const QString filepath = results->topLevelItem(filepath_row)->text(0);

    if (SearchEverywhere) {
        if (filepath == temp_search_result_path) {
            preview->setCursorPosition(temp_pos.y(), temp_pos.x());
            preview->extra_selections_search_results = selections[tempSelectionPos];
        }

        FileDirManager file_manager;
        // tree holds filepath
        const QString content = file_manager.read(filepath);
        preview->appendPlainText(content);
        preview->setFilePath(filepath);
        // iterate filepath, find our resuts for active filepath row
        for (unsigned int i = 0; i < MultifileSearchResults.size(); i++) {
            if (MultifileSearchResults[i][0].fileName == filepath) {
                preview->setCursorPosition(MultifileSearchResults[i][row].row, MultifileSearchResults[i][row].col);
                preview->extra_selections_search_results = selections[i];
                tempSelectionPos = i;
                temp_pos.setX(MultifileSearchResults[i][row].col);
                temp_pos.setY(MultifileSearchResults[i][row].row);
                break;// we have found file and posotions
            }
        }
        temp_search_result_path = results->topLevelItem(filepath_row)->text(0);
        return;
    }

    // current file search
    // document pointer updates both views according to changes in each of them, tab close requests
    // handles saving on their own
    preview->setDocument(m_Edit->document());
    preview->setFilePath(m_Edit->getFilePath());
    preview->setCursorPosition(m_Edit->search_results[row].row, m_Edit->search_results[row].col);
    preview->extra_selections_search_results = selections[0];// SearchCurrentFile
    preview->updateExtraSelections();
}

void FindReplaceWidget::slotShowMenu(const QPoint &pos) {
    menu->exec(mapToGlobal(pos));
}
