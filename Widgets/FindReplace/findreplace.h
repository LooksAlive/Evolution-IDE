#ifndef FINDREPLACEDIALOG_H
#define FINDREPLACEDIALOG_H

#include <QCheckBox>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>
#include <QGroupBox>
#include <QDockWidget>


#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "Widgets/Tab/tab.h"

class FindReplaceWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit FindReplaceWidget(Tab *tab, QWidget *parent = nullptr);
    ~FindReplaceWidget() = default;

private:
    QGroupBox *button_box;
    QPushButton *next;
    QPushButton *previous;
    QPushButton *replace;
    QPushButton *replaceall;

    QCheckBox *CaseSensitive;
    QCheckBox *RegularExpression;
    QCheckBox *WholeWords;

    QLineEdit *LineEditFind;
    QLineEdit *LineEditReplacement;

    QLabel *LabelText;
    QWidget *base;

    Tab *m_Tab = nullptr;
    PlainTextEdit *m_Edit = nullptr;
    QString same_file;

    QWidget *buildButtonBox();
    QLayout *buildForm();
    void getOptionsAndTexts();

    QTextDocument::FindFlags find_options;
    QString search_text;
    QString replace_text;

private slots:
    void slotNext();
    void slotPrevious();
    void slotReplace();
    void slotReplaceAll();
};

#endif // FINDREPLACEDIALOG_H
