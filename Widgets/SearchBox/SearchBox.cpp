#include "SearchBox.h"
#include "icons/IconFactory.h"


SearchBox::SearchBox(Tab *tab, QWidget *parent) : QWidget(parent), m_Tab(tab) {
    createWindow();
    // 120 for buttons, every button has 30px width
    setFixedWidth(270);
}
void SearchBox::createWindow() {
    MainLayout = new QHBoxLayout();
    find_options_menu_button = new QToolButton(this);
    lineEdit = new QLineEdit(this);
    next = new QToolButton(this);
    previous = new QToolButton(this);
    labelOccurences = new QLabel(this);
    more = new QToolButton(this);

    labelOccurences->setText("0/0");

    menu = new QMenu(this);
    caseSensitive = new QAction(this);
    wholeWords = new QAction(this);

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
    lineEdit->setFixedWidth(150);
    next->setIcon(QIcon(IconFactory::ScrollDown));
    next->setToolTip("Next");
    previous->setIcon(QIcon(IconFactory::ScrollUp));
    previous->setToolTip("Previous");
    more->setIcon(QIcon(IconFactory::Expand));// XDDD
    more->setToolTip("More");

    find_options_menu_button->setContextMenuPolicy(Qt::CustomContextMenu);
    // FIXME: this works only for right click !!! , but at least works :)
    connect(find_options_menu_button, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slotShowMenu(const QPoint &)));

    // enter, return
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(slotNext()));
    connect(next, SIGNAL(clicked()), this, SLOT(slotNext()));
    connect(previous, SIGNAL(clicked()), this, SLOT(slotPrevious()));
    // more is from outside, bc. it has to connect with find_replace dock

    MainLayout->addWidget(find_options_menu_button);
    MainLayout->addWidget(lineEdit);
    MainLayout->addStretch(5);
    MainLayout->addWidget(labelOccurences);
    MainLayout->addWidget(next);
    MainLayout->addWidget(previous);
    MainLayout->addWidget(more);

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);

    setLayout(MainLayout);
}

/* needs to be called everytime in case of changes */
void SearchBox::getOptionsAndTexts() {
    //LabelOccurrences->setText(QString());
    search_text = lineEdit->text();
    find_options = NULL;

    if (caseSensitive->isChecked()) {
        find_options |= QTextDocument::FindCaseSensitively;
    }
    if (wholeWords->isChecked()) {
        find_options |= QTextDocument::FindWholeWords;
    }

    /* same_file == ""  --> means that file has no filepath yet (blank) */
    if (same_file != m_Tab->tabToolTip(m_Tab->currentIndex()) || same_file == "") {
        m_Edit = qobject_cast<PlainTextEdit *>(m_Tab->currentWidget());
        same_file = m_Tab->tabToolTip(m_Tab->currentIndex());
    }
}

void SearchBox::slotPrevious() {
    getOptionsAndTexts();
    find_options |= QTextDocument::FindBackward;// flag for previous search
    m_Edit->findNext(search_text, find_options);
}

void SearchBox::slotNext() {
    getOptionsAndTexts();
    if (search_text != temp_search_text) {
        m_Edit->findStoreAndSelectAll(search_text, find_options);
        temp_search_text = search_text;
    }
    m_Edit->findNext(search_text, find_options);

    // increase labelOccurences
    const QString label = labelOccurences->text();
    const int c = label.indexOf("/");
    int num = label.mid(0, c).toInt();
    if (num > m_Edit->extra_selections_search_results.count()) {
        num = 0;
    }
    labelOccurences->setText(QString::number(num + 1) + "/" + QString::number(m_Edit->extra_selections_search_results.count()));
}

void SearchBox::slotShowMenu(const QPoint &pos) {
    menu->exec(mapToGlobal(pos));
}
