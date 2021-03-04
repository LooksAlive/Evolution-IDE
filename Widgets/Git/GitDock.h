#ifndef EVOLUTION_IDE_GITDOCK_H
#define EVOLUTION_IDE_GITDOCK_H

#include <QDockWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "gitbridge.h"


class GitDock : public QDockWidget {
    Q_OBJECT
public:
    explicit GitDock(QWidget *parent = nullptr);
    ~GitDock() = default;

private:
    void createWindow();

    QStackedWidget *Stack;
    QWidget *MainWidget;
    QFormLayout *Main_Layout;
    QPushButton *commitTo;
    QPushButton *pushTo;
    QPushButton *cloneTo;
    QPushButton *createRepositoryTo;

    QLabel *Branch;
    QLabel *Url;
    QLabel *Repository;

    void createCommitWindow();
    QVBoxLayout *CommitLayout;
    QWidget *CommitWidget;
    QPushButton *commit;
    // list of not commited, not contained(add) files, directories
    QTreeWidget *CommitList;


    void createPushWindow();
    QVBoxLayout *PushLayout;
    QWidget *PushWidget;
    QPushButton *push;
    QListWidget *PushList;

    void createCloneWindow();
    QFormLayout *CloneLayout;
    QWidget *CloneWidget;
    QPushButton *clone;
    QLineEdit *cloneUrl;
    QLineEdit *cloneDestination;

    void createNewRepository();
    QFormLayout *NewRepoLayout;
    QWidget *NewRepoWidget;
    QLineEdit *newRepository;
    QPushButton *createRepository;
    QLineEdit *newRepoDestination;

    void setConnections();

    GitBridge gitBridge;

private slots:
    void slotShowCommitWindow();
    void slotShowPushWindow();
    void slotShowCloneWindow();
    void slotShowNewRepoWindow();
};


#endif//EVOLUTION_IDE_GITDOCK_H
