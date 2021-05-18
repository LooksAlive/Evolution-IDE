#include "CompilationFlags.h"

CompilationFlags::CompilationFlags(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::Dialog);
    setMinimumSize(250, 120);
    setVisible(false);
    buildWindow();
}

void CompilationFlags::buildWindow() {
    MainLayout = new QHBoxLayout(this);
    optionContainer = new QListWidget(this);
    stack = new QStackedWidget(this);
    constructedFlags = new QPlainTextEdit(this);

    templates = new QListWidget(this); // first in col
    debugInformations = new QListWidget(this);
    optimization = new QListWidget(this);
    sanitizers = new QListWidget(this);

    stack->addWidget(templates);
    stack->addWidget(debugInformations);
    stack->addWidget(optimization);
    stack->addWidget(sanitizers);
    stack->setCurrentIndex(0);

    MainLayout->addWidget(optionContainer);
    MainLayout->addWidget(stack);
    MainLayout->addWidget(constructedFlags);
    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);

    connect(optionContainer, &QListWidget::currentRowChanged, stack, &QStackedWidget::setCurrentIndex);
    // double-clicks on flags
    connect(templates, SIGNAL(itemDoubleClicked(QListWidgetItem *item)), this, SLOT(addFlag(QListWidgetItem *item)));
    connect(debugInformations, SIGNAL(itemDoubleClicked(QListWidgetItem *item)), this, SLOT(addFlag(QListWidgetItem *item)));
    connect(optimization, SIGNAL(itemDoubleClicked(QListWidgetItem *item)), this, SLOT(addFlag(QListWidgetItem *item)));
    connect(sanitizers, SIGNAL(itemDoubleClicked(QListWidgetItem *item)), this, SLOT(addFlag(QListWidgetItem *item)));



    // fill lists
    for(const auto& t : TemplatesFlags) {
        auto *item = new QListWidgetItem();
        item->setText(t.first);
        item->setToolTip(t.second);
        templates->addItem(item);
    }
    optionContainer->addItem("Templates");
    for(const auto& t : DebugInformationsFlags) {
        auto *item = new QListWidgetItem();
        item->setText(t.first);
        item->setToolTip(t.second);
        templates->addItem(item);
    }
    optionContainer->addItem("Debug");
    for(const auto& t : OptimizationFlags) {
        auto *item = new QListWidgetItem();
        item->setText(t.first);
        item->setToolTip(t.second);
        templates->addItem(item);
    }
    optionContainer->addItem("Optimization");
    for(const auto& t : SanitizersFlags) {
        auto *item = new QListWidgetItem();
        item->setText(t.first);
        item->setToolTip(t.second);
        templates->addItem(item);
    }
    optionContainer->addItem("Sanitization");

}

void CompilationFlags::addFlag(QListWidgetItem *item) {
    constructedFlags->appendPlainText(" " + item->text());
}
