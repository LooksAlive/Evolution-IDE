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
#include <QStandardItem>
#include <QStandardItemModel>
#include <QToolButton>
#include <QTreeWidget>
#include <QWidget>

#include "Delegate.h"
#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "Widgets/Tab/tab.h"
#include "filemanager.h"
#include "highlighter.h"

#include <QDebug>

class FindReplaceWidget : public QDockWidget {
    Q_OBJECT
public:
    explicit FindReplaceWidget(Tab *tab, QWidget *parent = nullptr);
    ~FindReplaceWidget() = default;

    // all files we will searching
    void setFiles(const QStringList &files) { AllFiles = files; }

    QLineEdit *LineEditFind;

    // editing references from outside also
    // holds pointer to edit from result, soo editing is possible ;   on the right
    PlainTextEdit *preview;
    QTreeWidget *results;

    bool SearchEverywhere = false;
    // indicates if we are still on the same file, to bypass new file reading, when it is already set
    QString temp_search_result_path;
    QPoint temp_pos;
    // which selection we should set from data storage
    int tempSelectionPos;
    // selections storage (for references its needed to preform manual search)
    std::vector<QList<QTextEdit::ExtraSelection>> selections;
    // kinda tricky PlainTextEdit::search_results cannot be a type
    std::vector<std::vector<QPoint>> MultifileSearchResults;
    std::vector<QString> SearchFilesPaths;

    // references are set in data storage and as well in tree, but opening certain file by doubleClick
    // must be done from outside (tab, edit, file dock, ...)
    // this function navigate main edit with SearchCurrentFile mode
    void forwardToResult(QTreeWidgetItem *, int);

    // when searching multiple files and user make change in preview, lets save this new content
    // called whenever preview file changes to another
    void savePreview() const;

    QToolButton *inCurrentFile;
    QToolButton *everywhere;

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

    QWidget *base;

    Tab *m_Tab = nullptr;
    PlainTextEdit *m_Edit = nullptr;

    QString same_file;

    void createWindow();

    void getOptionsAndTexts();

    QTextDocument::FindFlags find_options;
    QString search_text;
    QString temp_search_text = "00";           // for whole file parsing, to not repeat with that same text, assure we
    QString temp_search_text_everywhere = "00";// have not  match at start
    QString replace_text;

    QStringList AllFiles;

    Highlighter *previewHighlighter;

    void searchEverywhere();

public slots:
    void slotNext();// MainWindow search


    // preview content is set by manual read
    void slotShowPreviewResult(QTreeWidgetItem *, int);

private slots:
    void slotPrevious();
    void slotReplace();
    void slotReplaceAll();

    void slotShowMenu(const QPoint &);

    void inCurrentFileToggled(bool);
    void everywhereToggled(bool);

    // clear search selections where not visible
    void slotVisible(bool visible);
};

#endif // FINDREPLACEDIALOG_H
