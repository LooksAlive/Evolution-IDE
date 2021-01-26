#ifndef GITWIDGET_H
#define GITWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>

/*
 * in root project dir should be .git file which should be loaded and info gathered
 * read it + add commit and push button, diff -> cmd exe git-diff and output, find online
*/

class GitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GitWidget(QWidget *parent = nullptr);
    ~GitWidget() = default;

    void loadData();
    void saveData();

private:

    QFormLayout *Main_Layout;

    QLineEdit *user;
    QLineEdit *email;
    QLineEdit *link;

    void createWindow();

};

#endif // GITWIDGET_H
