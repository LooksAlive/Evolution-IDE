#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QCheckBox>
#include <QDockWidget>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QPointer>
#include <QPushButton>
#include <QToolButton>
#include <QTreeView>
#include <QWidget>

#include "Delegate.h"
#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "Widgets/Tab/tab.h"

class FindReplaceWidget : public QDockWidget {
    Q_OBJECT
public:
    explicit FindReplaceWidget(Tab *tab, QWidget *parent = nullptr);
    ~FindReplaceWidget() = default;

    QLineEdit *LineEditFind;

private:
    QHBoxLayout *MainLayout;
    QToolBar *TitleBar;

    QToolButton *next;
    QToolButton *previous;
    QToolButton *replace;
    QToolButton *replaceall;

    QToolButton *find_options_menu_button;
    QAction *caseSensitive;
    QAction *wholeWords;

    QLineEdit *LineEditReplacement;
    QMenu *menu;

    QLabel *labelOccurences;

    // holds pointer to edit from result, soo editing is possible ;   on the right
    PlainTextEdit *preview;
    QTreeView *results;

    QWidget *base;

    Tab *m_Tab = nullptr;
    PlainTextEdit *m_Edit = nullptr;

    QString same_file;

    void createWindow();

    void getOptionsAndTexts();

    QTextDocument::FindFlags find_options;
    QString search_text;
    QString temp_search_text = "";   // for whole file parsing, to not repeat with that same text
    QString replace_text;


public slots:
    void slotNext();// mainwindow search

private slots:
    void slotPrevious();
    void slotReplace();
    void slotReplaceAll();

    void slotShowMenu(const QPoint &);

    // clear search selections where not visible
    void slotVisible(bool visible);

    void slotForwardToResult(const QModelIndex &);
    void slotShowPreviewResult(const QModelIndex &);
};

#endif // FINDREPLACEDIALOG_H
