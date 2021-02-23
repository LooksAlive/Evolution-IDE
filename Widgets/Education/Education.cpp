#include <QSettings>
#include <QIcon>

#include "icons/IconFactory.h"
#include "Education.h"

Education::Education(QWidget *parent) : QDockWidget(parent) {
    //setMinimumSize(400, 200);
    setContentsMargins(0, 0, 0, 0);
    //setWindowFlag(Qt::Dialog);
    setWindowTitle("Samples");
    //setWindowFlags(Qt::FramelessWindowHint);

    createWindow();
    loadUsersSamples();

    setFeatures(AllDockWidgetFeatures);
    setVisible(false);
}

void Education::createWindow() {
    stack = new QStackedWidget(this);
    CppCodeSamples = new QListWidget(this);
    CppUsersSamples = new QListWidget(this);
    preview = new QPlainTextEdit(this);
    cpp_user_samples.reserve(2);

    CppCodeSamples->insertItem(0, "Introduction");
    CppCodeSamples->insertItem(1, "Main");
    CppCodeSamples->insertItem(2, "Variables");
    CppCodeSamples->insertItem(3, "Data types");
    CppCodeSamples->insertItem(4, "For loop");
    CppCodeSamples->insertItem(5, "While loop");
    CppCodeSamples->insertItem(6, "Do While loop");
    CppCodeSamples->insertItem(7, "Foreach loop");
    CppCodeSamples->insertItem(8, "Functions");
    CppCodeSamples->insertItem(9, "Classes");
    CppCodeSamples->insertItem(10, "Structs");
    CppCodeSamples->insertItem(11, "Containers");
    CppCodeSamples->insertItem(12, "Const");
    CppCodeSamples->insertItem(13, "Static");
    CppCodeSamples->insertItem(14, "Pointers");
    CppCodeSamples->insertItem(15, "References");
    CppCodeSamples->insertItem(16, "Lambdas");
    CppCodeSamples->insertItem(17, "Operator Overloading");

    CppCodeSamples->setMinimumHeight(400);
    CppUsersSamples->setMinimumHeight(400);
    preview->setReadOnly(true);
    preview->setCenterOnScroll(true);
    preview->setUndoRedoEnabled(false);
    preview->hide();
    // lower font size, bot possible increase
    QFont font;
    font.setPixelSize(3);
    preview->setFont(font);

    titleBar = new QToolBar(this);
    language = new QToolButton(this);
    builtInSamples = new QToolButton(this);
    usersSamples = new QToolButton(this);
    auto *spacer = new QWidget(this);// align to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    titleBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    titleBar->setFixedHeight(35);
    // titleBar->setIconSize(QSize(25, 35));
    language->setText("C++");
    builtInSamples->setText("Built in");
    usersSamples->setText("Users");

    connect(builtInSamples, SIGNAL(clicked()), this, SLOT(slotShowCppSamples()));
    connect(usersSamples, SIGNAL(clicked()), this, SLOT(slotShowCppUsersSamples()));

    stack->addWidget(CppCodeSamples);
    stack->addWidget(CppUsersSamples);

    titleBar->addWidget(language);
    titleBar->addWidget(builtInSamples);
    titleBar->addWidget(usersSamples);
    titleBar->addWidget(spacer);
    titleBar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));

    setTitleBarWidget(titleBar);
    stack->setCurrentWidget(CppCodeSamples);// starting widget list
    setWidget(stack);
}

void Education::loadUsersSamples() {
    QSettings settings("Evolution-IDE");
    opened_samples = settings.value("Evolution-IDE/users_samples").toStringList();

    std::vector<QString> names;
    std::vector<QString> contents;

    for (int i = 0; i < opened_samples.size(); i++) {
        if (i % 2 == 0) {
            // sample content, each second
            contents.push_back(opened_samples[i]);
        } else {
            // sample name
            names.push_back(opened_samples[i]);
        }
    }
    for (int i = 0; i < contents.size(); i++) {
        addUserSample(contents[i], names[i]);
    }
}

void Education::saveUsersSamples() {
    // user samples
    for (int i = 0; i < CppUsersSamples->count(); i++) {
        // sample name
        opened_samples.append(CppUsersSamples->item(i)->text());
        // sample content
        opened_samples.append(cpp_user_samples[i]);
    }

    QSettings settings("Evolution-IDE");
    settings.setValue("Evolution-IDE/users_samples", opened_samples);
}

void Education::closeEvent(QCloseEvent *event) {
    saveUsersSamples();
    QDockWidget::closeEvent(event);
}

void Education::addUserSample(const QString &content, const QString &sampleName) {
    int index = CppUsersSamples->count();// place to last position index
    CppUsersSamples->insertItem(index, sampleName);
    cpp_user_samples.push_back(content);
}

void Education::slotShowCppSamples() {
    stack->setCurrentWidget(CppCodeSamples);
}
void Education::slotShowCppUsersSamples() {
    stack->setCurrentWidget(CppUsersSamples);
}
