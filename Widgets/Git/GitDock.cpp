#include "GitDock.h"
#include <QSettings>
#include <QFileInfo>

GitDock::GitDock(QWidget *parent) : QDockWidget(parent) {
    setFeatures(AllDockWidgetFeatures);
    setVisible(false);
    setWindowTitle("Git");

    createCommitWindow();
    createPushWindow();
    createCloneWindow();
    createNewRepository();
    createWindow();

    setConnections();

    QList<QStringList>l;
    l.append(QStringList() <<"elem.txt" << "elem2.cpp" << "elem3.h" << "elem4.py");

    fillCommitWithFiles(QStringList() << "myrepo", l);
    fillPushWithFiles(QStringList() << "myrepo", l);
}

void GitDock::createWindow() {
    Stack = new QStackedWidget(this);
    Main_Layout = new QFormLayout();
    MainWidget = new QWidget(this);

    commitTo = new QPushButton("Make Commit", this);
    pushTo = new QPushButton("Make Push", this);
    cloneTo = new QPushButton("Clone Repository", this);
    createRepositoryTo = new QPushButton("Create New Repository", this);
    Branch = new QLabel("My Branch", this);
    Url = new QLabel("http://myrepo/github.com", this);
    Repository = new QLabel("My Repo", this);

    Main_Layout->setVerticalSpacing(20);

    Main_Layout->addRow("Branch: ", Branch);
    Main_Layout->addRow("link: ", Url);
    Main_Layout->addRow("Repository name: ", Repository);
    Main_Layout->addRow("Commit to Repository", commitTo);
    Main_Layout->addRow("Push to Repository", pushTo);
    Main_Layout->addRow(cloneTo);
    Main_Layout->addRow(createRepositoryTo);

    MainWidget->setLayout(Main_Layout);

    Stack->addWidget(MainWidget);
    Stack->addWidget(CommitWidget);
    Stack->addWidget(PushWidget);
    Stack->addWidget(CloneWidget);
    Stack->addWidget(NewRepoWidget);
    Stack->setCurrentWidget(MainWidget);

    setWidget(Stack);
}

void GitDock::createCommitWindow() {
    CommitLayout = new QVBoxLayout();
    CommitWidget = new QWidget(this);
    CommitList = new QTreeWidget(this);
    commit = new QPushButton("Commit", this);

    CommitLayout->addWidget(CommitList);
    CommitLayout->addWidget(commit);

    CommitWidget->setLayout(CommitLayout);
}

void GitDock::createPushWindow() {
    PushLayout = new QVBoxLayout();
    PushWidget = new QWidget(this);
    PushList = new QTreeWidget(this);
    push = new QPushButton("Push", this);

    PushLayout->addWidget(PushList);
    PushLayout->addWidget(push);

    PushWidget->setLayout(PushLayout);
}

void GitDock::createCloneWindow() {
    CloneLayout = new QFormLayout();
    CloneWidget = new QWidget(this);
    clone = new QPushButton("Clone Repository", this);
    cloneUrl = new QLineEdit(this);
    cloneDestination = new QLineEdit(this);


    cloneUrl->setPlaceholderText("Repository Url");

    QSettings settings("Evolution");
    const QString dest = settings.value("Evolution/Project_Root_Dir").toString();
    cloneDestination->setText(dest);

    CloneLayout->addRow("Url", cloneUrl);
    CloneLayout->addRow("Destination", cloneDestination);
    CloneLayout->addRow(clone);

    CloneWidget->setLayout(CloneLayout);
}

void GitDock::createNewRepository() {
    NewRepoLayout = new QFormLayout();
    NewRepoWidget = new QWidget(this);
    newRepository = new QLineEdit(this);
    createRepository = new QPushButton("Create Repository", this);
    newRepoDestination = new QLineEdit(this);

    newRepository->setPlaceholderText("Repository Name");

    QSettings settings("Evolution");
    const QString dest = settings.value("Evolution/Project_Root_Dir").toString();
    newRepoDestination->setText(dest);

    NewRepoLayout->addRow("Repository Name: ", newRepository);
    NewRepoLayout->addRow("Destination", newRepoDestination);
    NewRepoLayout->addRow(createRepository);

    NewRepoWidget->setLayout(NewRepoLayout);
}

void GitDock::setConnections() {
    connect(commitTo, SIGNAL(pressed()), this, SLOT(slotShowCommitWindow()));
    connect(pushTo, SIGNAL(pressed()), this, SLOT(slotShowPushWindow()));
    connect(cloneTo, SIGNAL(pressed()), this, SLOT(slotShowCloneWindow()));
    connect(createRepositoryTo, SIGNAL(pressed()), this, SLOT(slotShowNewRepoWindow()));

    connect(commit, SIGNAL(pressed()), this, SLOT(slotShowPushWindow()));
    connect(push, &QAbstractButton::pressed, this, [=]() { Stack->setCurrentWidget(MainWidget); });
    connect(clone, &QAbstractButton::pressed, this, [=]() { Stack->setCurrentWidget(MainWidget); });
    connect(createRepository, &QAbstractButton::pressed, this, [=]() { Stack->setCurrentWidget(MainWidget); });
}

void GitDock::fillCommitWithFiles(const QStringList &repositoriesPaths, const QList<QStringList>& filePaths) {
    QFileIconProvider provider;
    for(int repoPath = 0; repoPath < repositoriesPaths.length(); repoPath++) {
        QTreeWidgetItem *repoItem = new QTreeWidgetItem();
        repoItem->setText(0 ,QFileInfo(repositoriesPaths[repoPath]).fileName());
        // Qt::ItemIsAutoTristate will set all children checked, unchecked at once
        // NOTICE! we are not storing state before !!!
        repoItem->setFlags(Qt::ItemIsAutoTristate | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        repoItem->setCheckState(0, Qt::Unchecked);
        CommitList->addTopLevelItem(repoItem);
        // add related files for repo
        for(int filePath = 0; filePath < filePaths[repoPath].length(); filePath++) {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0 ,QFileInfo(filePaths[repoPath][filePath]).fileName());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            item->setCheckState(0, Qt::Unchecked);
            item->setBackground(0, QBrush(Qt::red));
            item->setIcon(0, provider.icon(QFileInfo(filePaths[repoPath][filePath])));
            repoItem->addChild(item);
        }
    }
}

void GitDock::fillPushWithFiles(const QStringList &repositoriesPaths, const QList<QStringList>& filePaths) {
    QFileIconProvider provider;
    for(int repoPath = 0; repoPath < repositoriesPaths.length(); repoPath++) {
        QTreeWidgetItem *repoItem = new QTreeWidgetItem();
        repoItem->setText(0 ,QFileInfo(repositoriesPaths[repoPath]).fileName());
        // Qt::ItemIsAutoTristate will set all children checked, unchecked at once
        // NOTICE! we are not storing state before !!!
        repoItem->setFlags(Qt::ItemIsAutoTristate | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        repoItem->setCheckState(0, Qt::Unchecked);
        PushList->addTopLevelItem(repoItem);
        // add related files for repo
        for(int filePath = 0; filePath < filePaths[repoPath].length(); filePath++) {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0 ,QFileInfo(filePaths[repoPath][filePath]).fileName());
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            item->setCheckState(0, Qt::Unchecked);
            item->setBackground(0, QBrush(Qt::red));
            item->setIcon(0, provider.icon(QFileInfo(filePaths[repoPath][filePath])));
            repoItem->addChild(item);
        }
    }
}

void GitDock::slotShowCommitWindow() {
    Stack->setCurrentWidget(CommitWidget);
}

void GitDock::slotShowPushWindow() {
    Stack->setCurrentWidget(PushWidget);
}

void GitDock::slotShowCloneWindow() {
    Stack->setCurrentWidget(CloneWidget);
}

void GitDock::slotShowNewRepoWindow() {
    Stack->setCurrentWidget(NewRepoWidget);
}
