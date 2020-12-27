#ifndef DEBUGGERDOCK_H
#define DEBUGGERDOCK_H

#include <QDockWidget>
#include <QWidget>
#include <QPushButton>
#include <QLabel>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>


#include "Widgets/PlainTextEdit/plaintextedit.h"

class DebuggerDock : public QWidget
{
    Q_OBJECT
public:
    DebuggerDock(QWidget *parent = nullptr);
    ~DebuggerDock();


private:

    QWidget *base;
    PlainTextEdit *source_view;
    QPushButton *next;
    QWidget *debugwindow;

    QLayout *buildForm();
    QWidget *buildDebuggerWindow();





};




#endif // DEBUGGERDOCK_H
