#include "RefactoringDock.h"
#include "icons/IconFactory.h"
#include <QShortcut>

RefactoringDock::RefactoringDock(QWidget *parent) : QDockWidget(parent) {
    setWindowTitle("Refactor");
    setFeatures(AllDockWidgetFeatures);
    setVisible(false);

    createWindow();

    MainWidget = new QWidget(this);
    MainWidget->setLayout(MainLayout);

    setWidget(MainWidget);
}
void RefactoringDock::createWindow() {
    MainLayout = new QVBoxLayout();
    TitleBar = new QToolBar(this);
    filePath = new QLabel("file path", this);
    RefactorItemLayout = new QHBoxLayout();
    Original = new QLineEdit(this);
    Replacement = new QLineEdit(this);
    Refactor = new QPushButton("Refactor", this);

    Original->setFixedWidth(250);
    Replacement->setFixedWidth(250);

    connect(Refactor, SIGNAL(clicked()), this, SLOT(slotReport()));

    RefactorItemLayout->addWidget(Original);
    RefactorItemLayout->addSpacing(5);
    RefactorItemLayout->addWidget(Replacement);
    RefactorItemLayout->addSpacing(10);
    RefactorItemLayout->addWidget(Refactor);

    TitleBar->addWidget(filePath);
    auto *spacer = new QWidget(this);// align to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    TitleBar->addWidget(spacer);
    TitleBar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));
    TitleBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    TitleBar->setFixedHeight(30);
    TitleBar->setOrientation(Qt::Horizontal);

    setTitleBarWidget(TitleBar);

    connect(new QShortcut(Qt::Key_Escape, this), &QShortcut::activated, [=] { setVisible(false); });

    MainLayout->addLayout(RefactorItemLayout);
}

void RefactoringDock::spawnNewRow() {
    auto *layout = new QHBoxLayout();
    auto *original = new QLineEdit(this);
    auto *replacement = new QLineEdit(this);
    auto *refactor = new QPushButton("Refactor", this);

    layout->addWidget(original);
    layout->addSpacing(5);
    layout->addWidget(replacement);
    layout->addSpacing(10);
    layout->addWidget(refactor);

    MainLayout->addLayout(layout);
    //const QString orig = qobject_cast<QLineEdit>(layout->itemAt(0)->widget()).text();
    //const QString repl = qobject_cast<QLineEdit>(layout->itemAt(1)->widget()).text();
    rows.append(layout);
    /*
    // how to clear layout ??
    MainLayout = new QVBoxLayout();
    for (int i = 0; i <= rows.count(); i++) {
        MainLayout->addLayout(rows[i]);
    }
    */
}

void RefactoringDock::slotReport() {
    emit refactorClicked(0);
}
