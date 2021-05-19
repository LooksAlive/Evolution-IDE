#include "icons/IconFactory.h"
#include <QClipboard>
#include <QDesktopServices>
#include "CompilationFlags.h"

CompilationFlags::CompilationFlags(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::Dialog);
    setMinimumSize(250, 120);
    setVisible(false);
    buildWindow();
}

void CompilationFlags::buildWindow() {
    MainLayout = new QHBoxLayout(this);
    MainBar = new QToolBar(this);
    close = new QToolButton(this);
    clear = new QToolButton(this);
    copy = new QToolButton(this);
    reference = new QToolButton(this);

    optionContainer = new QListWidget(this);
    stack = new QStackedWidget(this);
    constructedFlags = new QPlainTextEdit(this);

    templates = new QListWidget(this); // first in col
    debugInformations = new QListWidget(this);
    optimization = new QListWidget(this);
    sanitizers = new QListWidget(this);

    MainBar->setOrientation(Qt::Vertical);
    MainBar->setFixedWidth(25);
    close->setIcon(QIcon(IconFactory::Remove));
    close->setToolTip("Hide");
    connect(close, &QAbstractButton::clicked, this, [=] { hide(); });
    MainBar->addWidget(close);
    copy->setIcon(QIcon(IconFactory::Copy));
    copy->setToolTip("Copy");
    connect(copy, &QAbstractButton::clicked, this, [=] { QApplication::clipboard()->setText(constructedFlags->toPlainText()); });
    MainBar->addWidget(copy);
    reference->setIcon(QIcon(IconFactory::Search));
    reference->setToolTip("Reference Page");
    connect(reference, &QAbstractButton::clicked, this, [=] { QDesktopServices::openUrl(QUrl(ReferencePage, QUrl::TolerantMode)); });
    MainBar->addWidget(reference);
    clear->setIcon(QIcon(IconFactory::Cut)); // TODO: find for clear
    clear->setToolTip("Reference Page");
    connect(clear, &QAbstractButton::clicked, this, [=] { constructedFlags->clear(); });
    MainBar->addWidget(clear);


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
