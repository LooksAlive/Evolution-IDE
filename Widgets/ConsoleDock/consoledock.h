#ifndef COMPILEDOCK_H
#define COMPILEDOCK_H

#include <QDockWidget>

#include <QTabWidget>
#include <QPlainTextEdit>

#include <QWidget>

#include "customtabstyle.h"

/*

  dock->OuterTab[0]->InnerTab[0]-> Logs; InnerTab[1]-> Compiler Output; InnerTab[2]-> Raw Output
        OuterTab[1]-> find, search window

*/

class  ConsoleDock : public QDockWidget
{
    Q_OBJECT
public:
    ConsoleDock(QWidget *parent = nullptr);

    void setRawOutput(const QString &raw);
    void addLogMessage(const QString &log_msg);

private:
    QTabWidget *OuterTab;
    CustomTabStyle *TabStyle; // innertab style -> horizontal and right(west) aligned

    void BuildOutput();
    QTabWidget *InnerTab;
    QPlainTextEdit *Logs;
    QPlainTextEdit *CompileOutput;
    QPlainTextEdit *RawOutput;

    // process raw output, set as compile
    void FormatRawOutput(const QString &raw);

};

#endif // COMPILEDOCK_H
