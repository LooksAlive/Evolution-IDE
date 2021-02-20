#ifndef EVOLUTION_IDE_DEBUGWATCHDOCK_H
#define EVOLUTION_IDE_DEBUGWATCHDOCK_H

#include <QDockWidget>
#include <QListWidget>
#include <QToolBar>
#include <QToolButton>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>

class DebugWatchDock : public QDockWidget {
    Q_OBJECT
public:
    explicit DebugWatchDock(QWidget *parent = nullptr);
    ~DebugWatchDock() = default;


public:
    void createDock();

    QToolButton *addWatch, *removeWatch, *modifyWatch;
    QListWidget *WatchedVariables;
    // tree, top item name is variable and its tree all its values
    QTreeView *VariableTreeValues;

private:
    QVBoxLayout *MainLayout;
    QWidget *WatchWidget;
    QToolBar *WatchToolBar;
    QToolBar *TitleBar;
};


#endif//EVOLUTION_IDE_DEBUGWATCHDOCK_H
