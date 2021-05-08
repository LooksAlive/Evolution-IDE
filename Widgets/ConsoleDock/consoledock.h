#ifndef COMPILEDOCK_H
#define COMPILEDOCK_H

#include <QDockWidget>
#include <QPlainTextEdit>  // remove
#include <QTabWidget>
#include <QTextBrowser>
#include <QToolBar>
#include <QToolButton>
#include <QWidget>
#include <QListWidget>
#include <QTreeWidget>

#include <QHBoxLayout>


#include <iostream>
#include <vector>

#include "customtabstyle.h"
#include "Widgets/ProcessDataPlot/ProcessDataPlot.h"

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
    ProcessDataPlot *processMemoryPlot;

    QToolButton *run;
    QToolButton *rerun;// stop and run again
    QToolButton *stop;

    void processText(const QString &text);
    // also check files to read;  executable path get from registry
    bool containsTests(const QString &text);
    bool containsBenchmarks(const QString &text);
    bool containsFuzzers(const QString &text);
    // called after execution or when some crash occured
    void processFromFile();

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

    Link findLink(const QString &filepath, const Direction &next = Current) const;

private:
    QTabWidget *MainTab;
    QToolButton *hideWindow;  // corner tab widget

    QWidget *linter;
    QHBoxLayout *linterLayout;
    QListWidget *linterList;

    QWidget *logger;
    QHBoxLayout *loggerLayout;
    QTreeWidget *loggerTree;

    QWidget *tests;
    QHBoxLayout *testsLayout;
    QListWidget *testList;

    QWidget *fuzzers;
    QHBoxLayout *fuzzersLayout;

    QWidget *memoryTracker;
    QHBoxLayout *memoryTrackerLayout;
    QTreeWidget *memoryTree;

    QWidget *console;
    QHBoxLayout *consoleLayout;
    QToolBar *tool_bar;

    void BuildConsole();


public slots:
    void slotScrollUp() const;
    void slotScrollDown() const;
    void clearConsole() const;
};

#endif // COMPILEDOCK_H
