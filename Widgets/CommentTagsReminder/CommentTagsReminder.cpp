#include <QFileInfo>

#include "icons/IconFactory.h"
#include "Delegate.h"
#include "CommentTagsReminder.h"

CommentTagsReminder::CommentTagsReminder(QWidget *parent) : QWidget(parent) {
    createWindow();
    // TODO: for all such widgets, figure out how to center to middle of the app, instead of this dialog
    // which adds panel etc.
    // TODO: ESC shortcut to exit, since we are not going to have any panel buttons.
    // kinda frameless window
    setWindowFlags(Qt::Dialog);
    setWindowTitle("Comment tags");
    setVisible(false);

    setMinimumSize(300, 200);
    tagData.reserve(5);
}

void CommentTagsReminder::createWindow() {
    MainLayout = new QVBoxLayout();
    view = new QTreeWidget(this);
    tagFilter = new QComboBox(this);
    fileNameFilter = new QComboBox(this);
    toolBar = new QToolBar(this);

    // html painting
    auto *delegate = new Delegate();
    view->setItemDelegate(delegate);
    view->setHeaderHidden(true);

    tagFilter->addItems(CommentTags + QStringList() << "All tags");
    tagFilter->setCurrentText("All tags");
    // filter->lineEdit()->setAlignment(Qt::AlignCenter);
    tagFilter->setMaxVisibleItems(5);
    tagFilter->setMaximumWidth(150);
    connect(tagFilter, &QComboBox::currentTextChanged, this, [=](){ fillView(); });

    fileNameFilter->addItems(sources + QStringList() << "All files");
    fileNameFilter->setCurrentText("All files");
    // filter->lineEdit()->setAlignment(Qt::AlignCenter);
    fileNameFilter->setMaxVisibleItems(5);
    fileNameFilter->setMaximumWidth(300);
    connect(fileNameFilter, &QComboBox::currentTextChanged, this, [=](){ fillView(fileNameFilter->currentText()); });

    toolBar->setFixedHeight(30);
    toolBar->addWidget(tagFilter);
    toolBar->addWidget(fileNameFilter);
    toolBar->addAction(QIcon(IconFactory::Expand), "Expand All", [=](){ view->expandAll(); });
    toolBar->addAction(QIcon(IconFactory::Collapse), "Collapse All", [=](){ view->collapseAll(); });
    toolBar->addAction(QIcon(IconFactory::Search), "Search again", [=](){ searchEverywhere(); });

    MainLayout->addWidget(toolBar);
    MainLayout->addWidget(view);

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);
    setLayout(MainLayout);
}

void CommentTagsReminder::fillView(const QString& filepath) {
    view->clear();

    if(!filepath.isEmpty()) {
        for(unsigned int i = 0; i < tagData.size(); i++) {
            if(sources[i] == filepath) {
                fileNameFilter->setCurrentText(filepath);
                auto *topItem = new QTreeWidgetItem();
                topItem->setToolTip(0, filepath);
                topItem->setText(0, QFileInfo(filepath).fileName());
                view->addTopLevelItem(topItem);

                for(const auto& elem : tagData[i]) {
                    qDebug() << elem.LineContent + "---------------------";
                    auto *item = new QTreeWidgetItem();
                    item->setToolTip(0, filepath);
                    item->setText(0, elem.LineContent);
                    item->setText(1, elem.tag);
                    item->setText(2, QString::number(elem.line + 1));
                    topItem->addChild(item);
                }
            }
        }
        return;
    }

    if(tagFilter->currentText() == "All tags") {
        for(unsigned int i = 0; i < tagData.size(); i++) {
            auto *topItem = new QTreeWidgetItem();
            topItem->setToolTip(0, sources[i]);
            topItem->setText(0, QFileInfo(sources[i]).fileName());
            view->addTopLevelItem(topItem);

            for(const auto& elem : tagData[i]) {
                qDebug() << elem.LineContent + "---------------------";
                auto *item = new QTreeWidgetItem();
                item->setToolTip(0, sources[i]);
                item->setText(0, elem.LineContent);
                item->setText(1, elem.tag);
                item->setText(2, QString::number(elem.line + 1));
                topItem->addChild(item);
            }
        }
        return;
    }
    else {
        // filtering one , TODO: hybrids allowed
        for(unsigned int i = 0; i < tagData.size(); i++) {
            for(const auto& elem : tagData[i]) {
                if(elem.tag == tagFilter->currentText()) {
                    // FIXME: this will be only list
                    auto *topItem = new QTreeWidgetItem();
                    topItem->setToolTip(0, sources[i]);
                    topItem->setText(0, QFileInfo(sources[i]).fileName());
                    view->addTopLevelItem(topItem);

                    auto *item = new QTreeWidgetItem();
                    item->setToolTip(0, sources[i]);
                    item->setText(0, elem.LineContent);
                    item->setText(1, elem.tag);
                    item->setText(2, QString::number(elem.line + 1));
                    topItem->addChild(item);
                }
            }
        }
        return;
    }
}

void CommentTagsReminder::searchEverywhere() {
    fileNameFilter->clear();
    fileNameFilter->addItem("All files");
    FileDirManager file_manager;
    QTextDocument document;
    for(int i = 0; i < sources.length(); i++) {
        qDebug() << sources[i] + "************************";
        const QString content = file_manager.read(sources[i]);
        document.setPlainText(content);
        // represents one file, filepath is in sources[i]
        std::vector<data> fileTagData;
        for(const QString& tag : CommentTags) {
            bool done = false;
            QTextCursor cursor;
            while(!done) {
                cursor = document.find(tag, cursor, QTextDocument::FindWholeWords);
                if(cursor.isNull()) {
                    done = true;
                    break;  // escape while loop
                }
                const int line = cursor.blockNumber();
                qDebug() << "found at line: " + QString::number(line);
                // format
                // spaces are critical here, we do not want to add some " "
                const QString Stag = "<span ";
                const QString Style = "style=color:orange;";
                const QString Mtag = " >";
                const QString Etag = "</span>";

                const int x = cursor.columnNumber();
                cursor.select(QTextCursor::LineUnderCursor);
                QString line_content = cursor.selectedText();
                // append html (highlight background for searched text)
                // Playing with positions :)
                const QString start = line_content.mid(0, x - tag.length());
                const QString end = line_content.mid(x + 1); // till end

                line_content = start + Stag + Style + Mtag + tag + Etag + end;

                fileTagData.push_back({ data { line, line_content, tag } });
            }
        }
        if(!fileTagData.empty()) {
            tagData.push_back({ fileTagData });
            fileNameFilter->addItem(QFileInfo(sources[i]).fileName());
        }
    }
    fileNameFilter->setCurrentText("All files");
    fillView();
}
