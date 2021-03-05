#ifndef EVOLUTION_IDE_GITDOCK_H
#define EVOLUTION_IDE_GITDOCK_H

#include <QDockWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
//#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

#include <QStatusBar>
#include <QFileIconProvider>

#include "gitbridge.h"


class GitDock : public QDockWidget {
    Q_OBJECT
public:
    explicit GitDock(QWidget *parent = nullptr);
    ~GitDock() = default;

    void setStatusBar(QStatusBar *status_bar) { statusBar = status_bar; }

    QStackedWidget *Stack;

private:
    QStatusBar *statusBar;


    void createWindow();

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
    QTreeWidget *PushList;

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

    // TODO: PullWidget  --- only updates current directory from github

    void setConnections();

    // not added files in project, checked will be added and commited on request
    // there might be more repos in ours...
    void fillCommitWithFiles(const QStringList &repositoriesPaths, const QList<QStringList>& filePaths);

    // commited files, which user want to push
    // there might be more repos in ours...
    void fillPushWithFiles(const QStringList &repositoriesPaths, const QList<QStringList>& filePaths);

    GitBridge gitBridge;

private slots:
    void slotShowCommitWindow();
    void slotShowPushWindow();
    void slotShowCloneWindow();
    void slotShowNewRepoWindow();
};


#endif//EVOLUTION_IDE_GITDOCK_H
