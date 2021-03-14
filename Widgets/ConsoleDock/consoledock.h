#ifndef COMPILEDOCK_H
#define COMPILEDOCK_H

#include <QDockWidget>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <QTextBrowser>
#include <QToolBar>
#include <QToolButton>
#include <QWidget>

#include <QHBoxLayout>


#include <iostream>
#include <vector>

#include "customtabstyle.h"

/*

  dock->OuterTab[0]->InnerTab[0]-> Logs; InnerTab[1]-> Compiler Output; InnerTab[2]-> Raw Output
        OuterTab[1]-> find, search window

*/

class ConsoleDock : public QDockWidget {
    Q_OBJECT
public:
    explicit ConsoleDock(QWidget *parent = nullptr);

    ~ConsoleDock() = default;

    QTextBrowser *ConsoleOutput;

    QToolButton *run;
    QToolButton *rerun;// stop and run again
    QToolButton *stop;

    void processText(const QString &text);

    // the only way to track links (num) is to store them

    struct Link {
        QString filePath;
        int lineNumber;   // line in browser,   ? is column num also needed ?
        QPoint position;
    };

    std::vector<Link> Links;

    enum Direction {
        Current = 0,
        Next,
        Previous
    };

    Link findLink(const QString filepath, const Direction &next = Current);

private:
    QHBoxLayout *MainLayout;
    QToolBar *title_bar;
    QToolBar *tool_bar;

    void BuildConsole();


public slots:
    void slotScrollUp() const;
    void slotScrollDown() const;
    void clearConsole() const;
};

#endif // COMPILEDOCK_H
