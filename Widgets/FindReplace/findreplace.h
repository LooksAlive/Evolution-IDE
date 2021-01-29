#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QCheckBox>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>
#include <QToolButton>
#include <QGroupBox>
#include <QDockWidget>
#include <QFormLayout>


#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "Widgets/Tab/tab.h"

class FindReplaceWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit FindReplaceWidget(Tab *tab, QWidget *parent = nullptr);
    ~FindReplaceWidget() = default;

    QLineEdit *LineEditFind;

private:
    QHBoxLayout *MainLayout;
    QFormLayout *input_layout;
    QVBoxLayout *flags_layout;

    QPushButton *next;
    QPushButton *previous;
    QPushButton *replace;
    QPushButton *replaceall;

    QCheckBox *CaseSensitive;
    QCheckBox *RegularExpression;
    QCheckBox *WholeWords;

    QLineEdit *LineEditReplacement;

    QLabel *LabelText;
    QWidget *base;

    Tab *m_Tab = nullptr;
    PlainTextEdit *m_Edit = nullptr;
    QString same_file;

    void createWindow();

    void getOptionsAndTexts();

    QTextDocument::FindFlags find_options;
    QString search_text;
    QString replace_text;

public slots:
    void slotNext();   // mainwindow search

private slots:
    void slotPrevious();
    void slotReplace();
    void slotReplaceAll();
};

#endif // FINDREPLACEDIALOG_H
