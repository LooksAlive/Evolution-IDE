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
    explicit ConsoleDock(QWidget *parent = nullptr);
    ~ConsoleDock();

    void setRawOutput(const QString &raw);
    void clear();

private:
    QPlainTextEdit *ConsoleOutput;

    void BuildConsole();

};

#endif // COMPILEDOCK_H
