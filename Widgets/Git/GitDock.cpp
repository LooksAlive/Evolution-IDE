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
    createLookUpWindow();
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
    TitleBar = new QToolBar(this);

    commitTo = new QPushButton("Make Commit", this);
    pushTo = new QPushButton("Make Push", this);
    cloneTo = new QPushButton("Clone Repository", this);
    createRepositoryTo = new QPushButton("Create New Repository", this);
    Branch = new QLabel("My Branch", this);
    Url = new QLabel("http://myrepo/github.com", this);
    Repository = new QLabel("My Repo", this);
    lookUpRepository = new QPushButton("Look up Repository", this);

    Main_Layout->setVerticalSpacing(20);
    //Branch->setText(gitBridge.getBranch());

    Main_Layout->addRow("Branch: ", Branch);
    Main_Layout->addRow("link: ", Url);
    Main_Layout->addRow("Repository name: ", Repository);
    Main_Layout->addRow("Commit to Repository", commitTo);
    Main_Layout->addRow("Push to Repository", pushTo);
    Main_Layout->addRow(cloneTo);
    Main_Layout->addRow(createRepositoryTo);
    Main_Layout->addRow(lookUpRepository);

    MainWidget->setLayout(Main_Layout);

    Stack->addWidget(MainWidget);
    Stack->addWidget(CommitWidget);
    Stack->addWidget(PushWidget);
    Stack->addWidget(CloneWidget);
    Stack->addWidget(NewRepoWidget);
    Stack->addWidget(LookUpWidget);
    Stack->setCurrentWidget(MainWidget);

    auto *spacer = new QWidget(this);// align to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    TitleBar->addWidget(new QLabel("Git Manager", this));
    TitleBar->addWidget(spacer);
    TitleBar->addAction(QIcon(IconFactory::Back), "Back", this, [=]() { Stack->setCurrentWidget(MainWidget); });
    TitleBar->addSeparator();
    TitleBar->addSeparator();
    TitleBar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));
    TitleBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    TitleBar->setFixedHeight(30);
    setTitleBarWidget(TitleBar);

    setWidget(Stack);
}

void GitDock::createCommitWindow() {
    CommitLayout = new QVBoxLayout();
    CommitWidget = new QWidget(this);
    CommitList = new QTreeWidget(this);
    Comment = new QPlainTextEdit(this);
    commit = new QPushButton("Commit", this);

    Comment->setFixedHeight(150);

    CommitLayout->addWidget(CommitList);
    CommitLayout->addWidget(Comment);
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
    newRepoDestination = new QLineEdit(this);
    InitialCommit = new QCheckBox("Create Initial Commit", this);
    createRepository = new QPushButton("Create Repository", this);

    InitialCommit->setCheckable(true);
    InitialCommit->setChecked(true);
    newRepository->setPlaceholderText("Repository Name");

    QSettings settings("Evolution");
    const QString dest = settings.value("Evolution/Project_Root_Dir").toString();
    newRepoDestination->setText(dest);

    NewRepoLayout->addRow("Repository Name: ", newRepository);
    NewRepoLayout->addRow("Destination", newRepoDestination);
    NewRepoLayout->addRow(InitialCommit);
    NewRepoLayout->addRow(createRepository);

    NewRepoWidget->setLayout(NewRepoLayout);
}

void GitDock::createLookUpWindow() {
    LookUpLayout = new QVBoxLayout();
    LookUpWidget = new QWidget(this);
    LookUpList = new QListWidget(this);
    lookUpName = new QLineEdit(this);
    lookUp = new QPushButton("Find", this);

    lookUpName->setPlaceholderText("Repository name");

    LookUpLayout->addWidget(lookUpName);
    LookUpLayout->addWidget(lookUp);
    LookUpLayout->addWidget(LookUpList);

    LookUpWidget->setLayout(LookUpLayout);
}

void GitDock::setConnections() {
    connect(commitTo, SIGNAL(pressed()), this, SLOT(slotShowCommitWindow()));
    connect(pushTo, SIGNAL(pressed()), this, SLOT(slotShowPushWindow()));
    connect(cloneTo, SIGNAL(pressed()), this, SLOT(slotShowCloneWindow()));
    connect(createRepositoryTo, SIGNAL(pressed()), this, SLOT(slotShowNewRepoWindow()));
    connect(lookUpRepository, &QAbstractButton::pressed, this, [=]() { Stack->setCurrentWidget(LookUpWidget); });

    connect(commit, SIGNAL(pressed()), this, SLOT(slotCommit()));
    connect(push, SIGNAL(pressed()), this, SLOT(slotPush()));
    connect(clone, SIGNAL(pressed()), this, SLOT(slotClone()));
    connect(createRepository, SIGNAL(pressed()), this, SLOT(slotNewRepo()));
    connect(lookUp, SIGNAL(pressed()), this, SLOT(slotLookUpRepo()));
}

void GitDock::fillCommitWithFiles(const QStringList &repositoriesPaths, const QList<QStringList>& filePaths) {
    QFileIconProvider provider;
    for(int repoPath = 0; repoPath < repositoriesPaths.length(); repoPath++) {
        QTreeWidgetItem *repoItem = new QTreeWidgetItem();
        repoItem->setText(0, QFileInfo(repositoriesPaths[repoPath]).fileName());
        repoItem->setToolTip(0, repositoriesPaths[repoPath]);
        // Qt::ItemIsAutoTristate will set all children checked, unchecked at once
        // NOTICE! we are not storing state before !!!
        repoItem->setFlags(Qt::ItemIsAutoTristate | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        repoItem->setCheckState(0, Qt::Unchecked);
        CommitList->addTopLevelItem(repoItem);
        // add related files for repo
        for(int filePath = 0; filePath < filePaths[repoPath].length(); filePath++) {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, QFileInfo(filePaths[repoPath][filePath]).fileName());
            item->setToolTip(0, filePaths[repoPath][filePath]);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            item->setCheckState(0, Qt::Unchecked);
            item->setIcon(0, provider.icon(QFileInfo(filePaths[repoPath][filePath])));
            repoItem->addChild(item);
        }
    }
    CommitList->expandAll();
}

void GitDock::fillPushWithFiles(const QStringList &repositoriesPaths, const QList<QStringList>& filePaths) {
    QFileIconProvider provider;
    for(int repoPath = 0; repoPath < repositoriesPaths.length(); repoPath++) {
        QTreeWidgetItem *repoItem = new QTreeWidgetItem();
        repoItem->setText(0, QFileInfo(repositoriesPaths[repoPath]).fileName());
        repoItem->setToolTip(0, repositoriesPaths[repoPath]);
        // Qt::ItemIsAutoTristate will set all children checked, unchecked at once
        // NOTICE! we are not storing state before !!!
        repoItem->setFlags(Qt::ItemIsAutoTristate | Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        repoItem->setCheckState(0, Qt::Unchecked);
        PushList->addTopLevelItem(repoItem);
        // add related files for repo
        for (int filePath = 0; filePath < filePaths[repoPath].length(); filePath++) {
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, QFileInfo(filePaths[repoPath][filePath]).fileName());
            item->setToolTip(0, filePaths[repoPath][filePath]);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            item->setCheckState(0, Qt::Unchecked);
            item->setIcon(0, provider.icon(QFileInfo(filePaths[repoPath][filePath])));
            repoItem->addChild(item);
        }
    }
    PushList->expandAll();
}

const QStringList GitDock::getCheckedFromCommitTree() {
    QStringList list;
    for (int item = 0; item < CommitList->topLevelItemCount(); item++) {
        auto topItem = CommitList->topLevelItem(item);
        if (topItem->checkState(item) == Qt::Checked) {
            // every child is also checked
            for (int child = 0; child < topItem->childCount(); child++) {
                list.append(topItem->toolTip(0));
            }
            continue;
        }
        for (int child = 0; child < topItem->childCount(); child++) {
            auto childItem = topItem->child(child);
            if (childItem->checkState(child) == Qt::Checked) {
                list.append(childItem->toolTip(0));
            }
        }
    }
    return list;
}

const QStringList GitDock::getCheckedFromPushTree() {
    QStringList list;
    for (int item = 0; item < PushList->topLevelItemCount(); item++) {
        auto topItem = PushList->topLevelItem(item);
        if (topItem->checkState(item) == Qt::Checked) {
            // every child is also checked
            for (int child = 0; child < topItem->childCount(); child++) {
                list.append(topItem->toolTip(0));
            }
            continue;
        }
        for (int child = 0; child < topItem->childCount(); child++) {
            auto childItem = topItem->child(child);
            if (childItem->checkState(child) == Qt::Checked) {
                list.append(childItem->toolTip(0));
            }
        }
    }
    return list;
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


void GitDock::slotCommit() {
    gitBridge.commit(Comment->toPlainText().toLatin1().data());
}

void GitDock::slotPush() {
    gitBridge.push();   // where to push ?
}

void GitDock::slotClone() {
    // Stack->setCurrentWidget(MainWidget);
    // clone->setText("jsld");
    gitBridge.clone(cloneTo->text().toLatin1().data(), cloneUrl->text().toLatin1().data());
}

void GitDock::slotNewRepo() {
    gitBridge.init(newRepository->text().toLatin1().data(), InitialCommit->isChecked());
}

void GitDock::slotLookUpRepo() {
    const char *repo_url = gitBridge.findRepo(lookUpName->text().toLatin1().data());
    // if i manage to get more results... make in loop.
    LookUpList->addItem(repo_url);
}
