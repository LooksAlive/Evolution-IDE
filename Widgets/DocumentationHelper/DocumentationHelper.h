#ifndef DOCUMENTATIONHELPER_H
#define DOCUMENTATIONHELPER_H

/*
 * Interactive documentation writer.
 * 2 parts:  (C, C++)
 *  1. in header files
 *  2. in source code
 *
 * Using doxygen, html tags.
 * formation is done easily (look up tags and highlight next word)
*/

#include <QWidget>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QLabel>
#include <QToolBar>
#include <QToolButton>
#include <QComboBox>

#include "EnvironmentSettings.h"


class PlainTextEdit;
//class PlainTextEditExtra;

class DocumentationHelper : public QWidget
{
    Q_OBJECT
public:
    explicit DocumentationHelper(QWidget *parent = nullptr);
    ~DocumentationHelper() = default;

    void setEdit(PlainTextEdit *e) { edit = e; }
    // void setEdit(PlainTextEditExtra *e) { editExtra = e; }

    // position and params to describe them
    // enums are done differently, but also contains main overview comment above; void ... no return
    void setDocData(const QPoint& posToInsert, const QStringList& params, const bool& isVoid = true, const bool& isEnum = false);
    // load and parse already written docs in case of interactive editing
    void loadDocData(const QPoint& posStartDoc, const QPoint& posEndDoc);



private:
    PlainTextEdit *edit = nullptr;
    //PlainTextEditExtra *editExtra = nullptr;

    QVBoxLayout *MLayout;
    QFormLayout *MainLayout;
    // manipulating view, adding, removing them.
    QToolBar *toolBar;
    QComboBox *commentTags;
    QToolButton *addTag;
    QToolButton *removeTag;

    // at the end (bottom right)  ... 0, Apply
    QPushButton *insert;
    QPlainTextEdit *overviewComment;

    // other widgets are dynamically added into list, bc
    // there might be more of them
    // label will be like @param, @return and comment next to it
    QList<QPair<QLabel*, QPlainTextEdit*>> sections;

    void createWindow();

    QPoint positionToInsert;


private slots:
    void addCommentSection();
    void removeCommentSection();
    // inserts doc to givven positoin by setDocData
    void insertDoc();

};

#endif // DOCUMENTATIONHELPER_H
