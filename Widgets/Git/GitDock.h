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
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QToolBar>
#include <QPlainTextEdit>
#include <QCheckBox>

#include <QStatusBar>
#include <QFileIconProvider>

#include "icons/IconFactory.h"
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
    QToolBar *TitleBar;

    void createWindow();

    QWidget *MainWidget;
    QFormLayout *Main_Layout;
    QPushButton *commitTo;
    QPushButton *pushTo;
    QPushButton *cloneTo;
    QPushButton *createRepositoryTo;
    QPushButton *lookUpRepository;

    QLabel *Branch;
    QLabel *Url;
    QLabel *Repository;

    void createCommitWindow();
    QVBoxLayout *CommitLayout;
    QWidget *CommitWidget;
    QPushButton *commit;
    // list of not commited, not contained(add) files, directories
    QTreeWidget *CommitList;
    QPlainTextEdit *Comment;


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
    QLineEdit *newRepoDestination;
    QCheckBox *InitialCommit;
    QPushButton *createRepository;

    void createLookUpWindow();

    QVBoxLayout *LookUpLayout;
    QWidget *LookUpWidget;
    QPushButton *lookUp;
    QLineEdit *lookUpName;
    QListWidget *LookUpList;

    // TODO: PullWidget  --- only updates current directory from github

    // make all connections in dock
    void setConnections();

    // not added files in project, checked will be added and commited on request
    // there might be more repos in ours...
    // TODO: disable removed file paths elements, highlight not added
    void fillCommitWithFiles(const QStringList &repositoriesPaths, const QList<QStringList> &filePaths);

    // commited files, which user want to push
    // there might be more repos in ours...
    // TODO: disable removed file paths elements, highlight not added
    void fillPushWithFiles(const QStringList &repositoriesPaths, const QList<QStringList> &filePaths);

    // returns list of all filepaths user chosen to commit, push
    // this is enough to prefrorm git operations
    const QStringList getCheckedFromCommitTree();

    const QStringList getCheckedFromPushTree();

    GitBridge gitBridge;

private slots:

    void slotShowCommitWindow();

    void slotShowPushWindow();

    void slotShowCloneWindow();

    void slotShowNewRepoWindow();

    void slotCommit();

    void slotPush();

    void slotClone();

    void slotNewRepo();

    void slotLookUpRepo();
};


#endif//EVOLUTION_IDE_GITDOCK_H
