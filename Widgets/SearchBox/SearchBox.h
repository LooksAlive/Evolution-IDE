#ifndef EVOLUTION_IDE_SEARCHBOX_H
#define EVOLUTION_IDE_SEARCHBOX_H

#include <QCheckBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMenu>
#include <QToolButton>

#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "Widgets/Tab/tab.h"


class SearchBox : public QWidget {
    Q_OBJECT
public:
    explicit SearchBox(Tab *tab, QWidget *parent = nullptr);
    ~SearchBox() = default;


    QLineEdit *lineEdit;
    QToolButton *more;

private:
    QHBoxLayout *MainLayout;
    QToolButton *next;
    QToolButton *previous;
    QToolButton *find_options_menu_button;// before line edit
    QMenu *menu;
    QAction *caseSensitive;
    QAction *wholeWords;

    //QLabel *LabelOccurrences;

    Tab *m_Tab = nullptr;
    PlainTextEdit *m_Edit = nullptr;
    QString same_file;

    QTextDocument::FindFlags find_options;
    QString search_text;
    QString temp_search_text = "";// for whole file parsing, to not repeat with that same text

    void createWindow();
    void getOptionsAndTexts();

public slots:
    void slotNext();
    void slotPrevious();

    void slotShowMenu(const QPoint &);
    void slotFindCaseSensitively(bool toggled);
    void slotFindWholeWords(bool toggled);
};


#endif//EVOLUTION_IDE_SEARCHBOX_H
