#ifndef EVOLUTION_IDE_COMMENTTAGSREMINDER_H
#define EVOLUTION_IDE_COMMENTTAGSREMINDER_H

/*
 * Finds all tags in project, create list..., user can also toggle them automatically.
 * It should be suggested by interface
 * TODO, FIXME, NOTE
 * added by Evolution-IDE :
 * LONG, USELESS, SPEEDUP, EXPAND, SHRINK, DEPRECATED
*/

#include <QWidget>
#include <QTreeWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QToolBar>

#include <QTextDocument>
#include <QTextCursor>

#include <iostream>
#include <vector>

#include "EnvironmentSettings.h"    // QStringList data to search
#include "filemanager.h"

class CommentTagsReminder : public QWidget {
Q_OBJECT
public:
    explicit CommentTagsReminder(QWidget *parent = nullptr);
    ~CommentTagsReminder() = default;

    void setSources(const QStringList& s) { sources = s; }

    QTreeWidget *view;

    void searchEverywhere();

    // fill according to filter;  or specific to filepath
    // NOTE! there are only sources that actually contain any comment tags
    void fillView(const QString& filepath = QString());

    QStringList sources;

    struct data{
        int line;
        QString LineContent;
        QString tag;
    };

    std::vector<std::vector<data>> tagData;

private:
    // TODO: more tags can be mark within one statement
    QVBoxLayout *MainLayout;
    QToolBar *toolBar;
    QComboBox *tagFilter;
    QComboBox *fileNameFilter;

    void createWindow();
};


#endif //EVOLUTION_IDE_COMMENTTAGSREMINDER_H
