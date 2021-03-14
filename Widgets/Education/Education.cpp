#include <QSettings>
#include <QIcon>
#include <QDebug>
#include <QFileInfo>

#include "icons/IconFactory.h"
#include "Education.h"

Education::Education(QWidget *parent) : QDockWidget(parent) {
    //setMinimumSize(400, 200);
    setContentsMargins(0, 0, 0, 0);
    //setWindowFlag(Qt::Dialog);
    setWindowTitle("Samples");
    //setWindowFlags(Qt::FramelessWindowHint);

    previewHighlighter = new Highlighter(":/highlights/languages.xml", this);

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

    CppCodeSamples->addItem("Introduction");
    CppCodeSamples->addItem("Main");
    CppCodeSamples->addItem("Variables");
    CppCodeSamples->addItem("Operators");
    CppCodeSamples->addItem("If Else");
    CppCodeSamples->addItem("Switch");
    CppCodeSamples->addItem("For Loop");
    CppCodeSamples->addItem("Do While loop");
    CppCodeSamples->addItem("Foreach loop");
    CppCodeSamples->addItem("Functions");
    CppCodeSamples->addItem("Classes");
    CppCodeSamples->addItem("Structs");
    CppCodeSamples->addItem("Containers");
    CppCodeSamples->addItem("Const");
    CppCodeSamples->addItem("Static");
    CppCodeSamples->addItem("Pointers");
    CppCodeSamples->addItem("References");
    CppCodeSamples->addItem("Lambdas");
    CppCodeSamples->addItem("Operator Overloading");

    CppCodeSamples->setMinimumHeight(400);
    CppUsersSamples->setMinimumHeight(400);
    preview->setReadOnly(true);
    preview->setEnabled(true);
    preview->setCenterOnScroll(true);
    preview->setUndoRedoEnabled(false);
    // lower font size, bot possible increase
    QFont font;
    font.setPixelSize(10);
    preview->setFont(font);

    connect(CppCodeSamples, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slotShowCppSampleInPreview(QListWidgetItem *)));

    titleBar = new QToolBar(this);
    language = new QToolButton(this);
    builtInSamples = new QToolButton(this);
    usersSamples = new QToolButton(this);
    removeSample = new QToolButton(this);
    auto *spacer = new QWidget(this);// align to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    titleBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    titleBar->setFixedHeight(35);
    // titleBar->setIconSize(QSize(25, 35));
    language->setText("C++");
    builtInSamples->setText("Built in");
    usersSamples->setText("Users");

    removeSample->setToolTip("Remove Sample");
    removeSample->setIcon(QIcon(IconFactory::Remove));

    connect(builtInSamples, SIGNAL(clicked()), this, SLOT(slotShowCppSamples()));
    connect(usersSamples, SIGNAL(clicked()), this, SLOT(slotShowCppUsersSamples()));
    connect(removeSample, SIGNAL(clicked()), this, SLOT(slotRemoveCppUsersSample()));

    stack->addWidget(CppCodeSamples);
    stack->addWidget(CppUsersSamples);

    titleBar->addWidget(language);
    titleBar->addWidget(builtInSamples);
    titleBar->addWidget(usersSamples);
    titleBar->addWidget(removeSample);
    titleBar->addWidget(spacer);
    titleBar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));
    titleBar->layout()->setContentsMargins(0, 0, 0, 0);
    titleBar->layout()->setSpacing(0);

    setTitleBarWidget(titleBar);
    stack->setCurrentWidget(CppCodeSamples);// starting widget list
    auto *MainWidget = new QWidget(this);
    auto *MainLayout = new QVBoxLayout();
    MainLayout->addWidget(stack);
    MainLayout->addWidget(preview);
    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);
    MainWidget->setLayout(MainLayout);
    setWidget(MainWidget);
}

void Education::loadUsersSamples() {
    QSettings settings("Evolution");
    opened_samples = settings.value("Evolution/users_samples").toStringList();

    if (opened_samples.empty()) {
        return;
    }

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
    for (unsigned int i = 0; i < contents.size(); i++) {
        addUserSample(contents[i], names[i]);
    }
}

void Education::saveUsersSamples() {
    if (CppUsersSamples->count() == 0) {
        return;
    }

    // user samples
    for (int i = 0; i < CppUsersSamples->count(); i++) {
        // sample name
        opened_samples.append(CppUsersSamples->item(i)->text());
        // sample content
        opened_samples.append(cpp_user_samples[i]);
    }
    QSettings settings("Evolution");
    settings.setValue("Evolution/users_samples", opened_samples);
}

void Education::closeEvent(QCloseEvent *event) {
    saveUsersSamples();
    QDockWidget::closeEvent(event);
}

void Education::addUserSample(const QString &content, const QString &sampleName) {
    CppUsersSamples->addItem(sampleName);
    cpp_user_samples.push_back(content);
}

void Education::slotShowCppSamples() {
    stack->setCurrentWidget(CppCodeSamples);
}
void Education::slotShowCppUsersSamples() {
    stack->setCurrentWidget(CppUsersSamples);
}

void Education::slotRemoveCppUsersSample() {
    if (CppUsersSamples->currentIndex().isValid() && CppUsersSamples->takeItem(CppUsersSamples->currentIndex().row())->isSelected()) {
        const QString Sname = CppUsersSamples->selectedItems()[0]->listWidget()->item(0)->text();
        delete CppUsersSamples->takeItem(CppUsersSamples->currentIndex().row());
        // find sample by its name
        cpp_user_samples.pop_back();
        cpp_user_samples.clear();
    } else {// cannot remove built in samples
        return;
    }
}

void Education::slotShowCppSampleInPreview(QListWidgetItem *item) {
    // CppUsersSamples->currentItem();
    // const QString sampleName = item->text();
    const int row = item->listWidget()->row(item);
    /*
    qDebug() << row;
    qDebug() << "\n";
    qDebug() << cpp_code_samples.size();
    qDebug() << "\n";
    qDebug() << cpp_code_samples[0].content.size();
    qDebug() << "\n";
    */
    const QString content = cpp_code_samples[row].content[0];// TODO: this should be file if more ...
    preview->setPlainText(content);
    // Highlighter for sample
    previewHighlighter->setDocument(preview->document());
    // in this case built in code have extensions, bit for user samples use rather ...
    previewHighlighter->setExtension(QFileInfo(cpp_code_samples[row].fileNames[0]).completeSuffix());
    // previewHighlighter->setExtension(".cpp");
    previewHighlighter->highlightBlock(content);
}
