#ifndef COMPILEDOCK_H
#define COMPILEDOCK_H

#include <QDockWidget>

#include <QTabWidget>
#include <QPlainTextEdit>

#include <QWidget>

#include "customtabstyle.h"
#include "findreplace.h"

/*

  dock->OuterTab[0]->InnerTab[0]-> Logs; InnerTab[1]-> Compiler Output; InnerTab[2]-> Raw Output
        OuterTab[1]-> find, search window

*/

class ConsoleDock : public QDockWidget
{
    Q_OBJECT
public:
    ConsoleDock(QWidget *parent = nullptr);
    FindReplaceWidget *find_replace;

private:
    QTabWidget *OuterTab;
    CustomTabStyle *TabStyle; // innertab style -> horizontal and right(west) aligned

    QWidget *BuildOutput();
    QTabWidget *InnerTab;
    QPlainTextEdit *Logs;
    QPlainTextEdit *CompileOutput;
    QPlainTextEdit *RawOutput;

    QWidget *BuildSearchResults();

};

#endif // COMPILEDOCK_H
