#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "icons/IconFactory.h"
#include "DocumentationHelper.h"

DocumentationHelper::DocumentationHelper(QWidget *parent) : QWidget(parent) {
    createWindow();
    setWindowFlags(Qt::Dialog);

    setMinimumSize(200, 300);
    setVisible(false);
}

void DocumentationHelper::createWindow() {
    MLayout = new QVBoxLayout(this);
    MainLayout = new QGridLayout();
    toolBar = new QToolBar(this);
    commentTags = new QComboBox(this);
    addTag = new QToolButton(this);
    removeTag = new QToolButton(this);
    insert = new QPushButton(this);
    overviewComment = new QPlainTextEdit(this);

    toolBar->setFixedHeight(25);
    commentTags->addItems(DocumentationTags);
    commentTags->setCurrentText("@param");
    addTag->setToolTip("Add Tag");
    addTag->setIcon(QIcon(IconFactory::Plus));
    removeTag->setToolTip("Remove Tag");
    removeTag->setIcon(QIcon(IconFactory::Minus));
    insert->setText("Insert");
    overviewComment->setPlaceholderText("Base Comment");

    connect(addTag, SIGNAL(clicked()), this, SLOT(addCommentSection()));
    connect(addTag, SIGNAL(clicked()), this, SLOT(removeCommentSection()));
    connect(insert, SIGNAL(clicked()), this, SLOT(insertDoc()));

    toolBar->layout()->setContentsMargins(0, 0, 0, 0);
    toolBar->layout()->setSpacing(0);
    toolBar->addWidget(commentTags);
    toolBar->addWidget(addTag);
    toolBar->addWidget(removeTag);

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(1);

    MLayout->setContentsMargins(0, 0, 0, 0);
    MLayout->setSpacing(0);

    MLayout->addWidget(toolBar);
    MLayout->addWidget(overviewComment);
    MLayout->addLayout(MainLayout);
    MLayout->addWidget(insert, 2, Qt::AlignRight);
}


void DocumentationHelper::setDocData(const QPoint &posToInsert, const QStringList &params, const bool& isVoid, const bool& isEnum) {
    positionToInsert = posToInsert;

    if(isEnum) {
        // TODO: solve this one
        MainLayout->addWidget(insert);
        return;
    }

    for (int i = 0; i < params.size(); i++) {
        auto *label = new QLabel(this);
        label->setTextFormat(Qt::RichText);
        label->setText("@param <br> <b>" + params[i] + "</b>");
        auto *comment = new QPlainTextEdit(this);
        sections.append(QPair(label, comment));
        MainLayout->addWidget(label, MainLayout->rowCount(), 0);
        MainLayout->addWidget(comment, MainLayout->rowCount(), 1);
    }

    if(!isVoid) {
        // @return
        auto *label = new QLabel(this);
        label->setText("@return");
        auto *comment = new QPlainTextEdit(this);
        sections.append(QPair(label, comment));
        MainLayout->addWidget(label, MainLayout->rowCount(), 0);
        MainLayout->addWidget(comment, MainLayout->rowCount(), 1);
    }
}

void DocumentationHelper::loadDocData(const QPoint &posStartDoc, const QPoint &posEndDoc) {

}

void DocumentationHelper::insertDoc() {
    QString content;
    content.append(overviewComment->toPlainText());
    for (int i = 0; i <= sections.size(); i++) {
        content.append("\n");
        content.append(sections[i].first->text());  // label
        content.append(sections[i].second->toPlainText());  // comment
    }

    QTextCursor cursor = edit->textCursor();
    edit->setCursorPosition(cursor, positionToInsert.y(), positionToInsert.x());
    cursor.insertText(content);
    edit->setTextCursor(cursor);
}

void DocumentationHelper::addCommentSection() {
    const QString tag = commentTags->currentText();

    auto *label = new QLabel(this);
    label->setText(tag);
    auto *comment = new QPlainTextEdit(this);
    sections.append(QPair(label, comment));
    MainLayout->addWidget(label, MainLayout->rowCount(), 0);
    MainLayout->addWidget(comment, MainLayout->rowCount(), 1);

    // FIXME: insert button goes at the end
}

void DocumentationHelper::removeCommentSection() {
    MainLayout->removeWidget(overviewComment);
}
