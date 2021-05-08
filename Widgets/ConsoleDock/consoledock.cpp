#include <QDebug>
#include <QFileInfo>
#include <QScrollBar>
#include <QShortcut>
#include <QString>
#include <QSettings>
#include <iostream>

#include "icons/IconFactory.h"
#include "filemanager.h"
#include "consoledock.h"
/* enum QTabBar::Shape */

ConsoleDock::ConsoleDock(QWidget *parent) : QDockWidget(parent)
{
    /* QDockWidget inherited functions */
    setWindowTitle("Console");
    setVisible(false);
    setFeatures(AllDockWidgetFeatures);
    setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);

    // escape shortcut  -> will close the window
    connect(new QShortcut(Qt::Key_Escape, this), &QShortcut::activated, [=] { this->setVisible(false); });

    BuildConsole();
    // setTitleBarWidget(title_bar);
    QWidget* lEmptyWidget = new QWidget(this);
    lEmptyWidget->setFixedHeight(3);
    setTitleBarWidget(lEmptyWidget);    // MainTab works fine either for draging

    Links.reserve(3);   // :)


    // FIXME: moving not possible when set title bar widget like this
}

// change to QListWidget most probably, bc. of specific widget shows its own data and do not know
// how to do that with tabs, at least yet
void ConsoleDock::BuildConsole() {
    consoleLayout = new QHBoxLayout();
    tool_bar = new QToolBar(this);
    console = new QWidget(this);
    ConsoleOutput = new QTextBrowser(this);
    processMemoryPlot = new ProcessDataPlot(this);

    MainTab = new QTabWidget(this);
    MainTab->setTabPosition(QTabWidget::North);
    MainTab->setTabShape(QTabWidget::Rounded);
    MainTab->setFocusPolicy(Qt::ClickFocus);
    QFont tabFont;
    tabFont.setItalic(true);
    tabFont.setFamily("Ubuntu Mono");
    MainTab->tabBar()->setFont(tabFont);
    hideWindow = new QToolButton(this);
    hideWindow->setIcon(QIcon(IconFactory::Remove));
    hideWindow->setToolTip("Close");
    connect(hideWindow, SIGNAL(clicked()), this, SLOT(hide()));
    MainTab->setCornerWidget(hideWindow);

    linter = new QWidget(this);
    linterLayout = new QHBoxLayout();
    linterList = new QListWidget(this);
    linterLayout->addWidget(linterList);
    linterLayout->setContentsMargins(0, 0, 0, 0);
    linterLayout->setSpacing(0);
    linter->setLayout(linterLayout);

    logger = new QWidget(this);
    loggerLayout = new QHBoxLayout();
    loggerTree = new QTreeWidget(this);
    loggerLayout->addWidget(loggerTree);
    loggerLayout->setContentsMargins(0, 0, 0, 0);
    loggerLayout->setSpacing(0);
    logger->setLayout(loggerLayout);

    tests = new QWidget(this);
    testsLayout = new QHBoxLayout();
    testsLayout->setContentsMargins(0, 0, 0, 0);
    testsLayout->setSpacing(0);
    testList = new QListWidget(this);
    testList->addItems(QStringList() << "halabala" << "balahala");
    testsLayout->addWidget(testList);
    tests->setLayout(testsLayout);

    fuzzers = new QWidget(this);
    fuzzersLayout = new QHBoxLayout();
    fuzzersLayout->setContentsMargins(0, 0, 0, 0);
    fuzzersLayout->setSpacing(0);
    fuzzers->setLayout(fuzzersLayout);

    memoryTracker = new QWidget(this);
    memoryTrackerLayout = new QHBoxLayout();
    memoryTrackerLayout->setContentsMargins(0, 0, 0, 0);
    memoryTrackerLayout->setSpacing(0);
    memoryTree = new QTreeWidget(this);
    memoryTrackerLayout->addWidget(memoryTree);
    memoryTracker->setLayout(memoryTrackerLayout);

    ConsoleOutput->setReadOnly(true);
    // we have ours custom signal and slot handling for clicked link, do not want to open browser
    ConsoleOutput->setOpenLinks(false);
    ConsoleOutput->setOpenExternalLinks(false);
    ConsoleOutput->setTextInteractionFlags(Qt::TextBrowserInteraction);
    // invitation message
    ConsoleOutput->append("This is Evolution-IDE");
    ConsoleOutput->setUpdatesEnabled(true);

    run = new QToolButton(this);
    run->setToolTip("Run");
    run->setIcon(QIcon(IconFactory::Run));
    rerun = new QToolButton(this);
    rerun->setToolTip("Rerun");
    rerun->setIcon(QIcon(IconFactory::Run));
    stop = new QToolButton(this);
    stop->setToolTip("Stop");
    stop->setIcon(QIcon(IconFactory::Stop));
    tool_bar->addWidget(run);
    tool_bar->addWidget(rerun);
    tool_bar->addWidget(stop);
    tool_bar->addAction(QIcon(IconFactory::ScrollUp), "Scroll Up", this, SLOT(slotScrollUp()));
    tool_bar->addAction(QIcon(IconFactory::ScrollDown), "Scroll Down", this, SLOT(slotScrollDown()));
    tool_bar->addAction(QIcon(IconFactory::TrashBin), "Clear", this, SLOT(clearConsole()));

    tool_bar->setOrientation(Qt::Vertical);
    tool_bar->setMovable(false);
    tool_bar->setFixedWidth(35);
    tool_bar->setFloatable(false);
    tool_bar->setAcceptDrops(false);
    //tool_bar->setIconSize(QSize(35, 25));
    tool_bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    tool_bar->layout()->setContentsMargins(0, 0, 0, 0);
    tool_bar->layout()->setSpacing(0);

    consoleLayout->addWidget(tool_bar);
    consoleLayout->addWidget(ConsoleOutput);
    consoleLayout->addWidget(processMemoryPlot);

    consoleLayout->setContentsMargins(0, 0, 0, 0);
    consoleLayout->setSpacing(0);

    console->setLayout(consoleLayout);


    MainTab->addTab(console, "Console");
    MainTab->addTab(linter, "Linter");
    MainTab->addTab(logger, "Logger");
    MainTab->addTab(tests, "Tests");
    MainTab->addTab(fuzzers, "Fuzzers");
    MainTab->addTab(memoryTracker, "Memory Tracker");


    setWidget(MainTab);

    //ConsoleOutput->setHtml("<a href = http://google.com > moj text </a>");
    //ConsoleOutput->append("<a href = http://google.com > moj text </a>");
    processText("warning /home/adam/Desktop/Qt5_forum:5:5 kjskldafjhk kjsadfhkjsdlhakjf gfdsgakhfg");
    processText("error /home/adam/Desktop/GITHUB sjkdfh b");
    processText("kkwjrlwhetjhlew jkew /home/adam/Desktop/GITHUB lahfjakslhdj ");
    //processText("kkwjrlwhetjhlew    /home/adam/Desktop/GITHUB");
    //ConsoleOutput->append("<a style=color:red; href = /home/adam/Desktop/Qt5_forum > /home/adam/Desktop/Qt5_forum </a>");                             // works well
    //ConsoleOutput->append("<a style='color:red;background-color:lightblue;' href = /home/adam/Desktop/Qt5_forum > /home/adam/Desktop/Qt5_forum </a>");// works well
    //ConsoleOutput->append("<a style=color:lightblue; href = /home/adam/Desktop/Qt5_forum > /home/adam/Desktop/Qt5_forum </a>");                       // works well
}

// sentence processing
void ConsoleDock::processText(const QString &text) {
    if (text.isEmpty()) {
        return;
    }

    // separate to words + also for performance check for lenght
    // spaces are critical, we do not want to merge with " "
    const QString Stag = "<a ";
    const QString href = " href = ";
    const QString Mtag = ">";
    const QString Etag = "</a> ";
    // right after <a {}        ++ possible background ,  !! hover <style> not working ...
    const QString styleWarning = "style=color:yellow;";
    const QString styleError = "style=color:red;";

    QString ProcessedText;
    QString link;
    int row; // y
    int col; // x

    bool Warning = false;
    bool Error = false;

    // these words might be after filename, pos
    if (text.contains("warning")) {
        Warning = true;
    }
    if (text.contains("error")) {
        Error = true;
    }

    // separate to words
    //const QStringList words = text.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    // select whole link
    for (int i = 0; i < text.length(); i++) {
        if (text[i] == "/") {
            // take whole path, find space from index i
            const int pos = text.indexOf(" ", i);
            link = text.mid(i, pos - i);
            qDebug() << "link first:" + link;
            // often after links follows position in format row:col ; or line: xx ; or line xx
            // with clang there is no space for ex.: error_file.cpp:25:45 or in cmake only row
            if (link.contains(":")) {
                // we have a position too, this will be the first occurence so go only forward
                const int endOfLink = link.indexOf(":");
                const QString position = link.mid(endOfLink + 1);   // till end
                const int secondPos = position.indexOf(":");
                link = link.mid(0, endOfLink);  // real, final link
                qDebug() << "link position:" + position;
                qDebug() << "link after:" + link;


                row = QString(position.mid(0, secondPos)).toInt();
                qDebug() << "row: " + QString::number(row);
                // col is the rest
                col = QString(position.mid(secondPos + 1)).toInt();
                qDebug() << "col: " + QString::number(col);
            }

            if (QFileInfo(link).exists() && !QFileInfo(link).isDir()) {
                i = pos; // jump over link, also position
                Links.push_back(Link{link, ConsoleOutput->textCursor().blockNumber() + 1,
                                     QPoint(col + 1, row + 1)}); // starts from 0
            } else {
                link.clear();
            }
        }
        // There is no way that 2 links can be merged so no need to clear variables
        if (!link.isEmpty()) {
            QString tagText = link;
            link.insert(0, Stag);
            // change is text is good now
            if (Warning) {
                link = Stag + styleWarning + href + tagText + Mtag + tagText + Etag;// <a style=...; href= ... > (real text)
                //qDebug() << link;
                Warning = false;
            }
            if (Error) {
                link = Stag + styleError + href + tagText + Mtag + tagText + Etag;// <a style=...; href= ... > (real text)
                //qDebug() << link;
                Error = false;
            } else
                link = Stag + href + tagText + Mtag + tagText + Etag;

            ProcessedText.append(link);
            link.clear();
            tagText.clear();
        } else {
            ProcessedText.append(text[i]);
        }
    }

    ConsoleOutput->append(ProcessedText);
}

bool ConsoleDock::containsTests(const QString &text) {
    // in console starts with { Success } ; { Failure }
    if(text.contains("{ Success }") || text.contains("{ Failure }")) {
        // process and add to list with proper icon for state and other data

        return true;
    }
    else {
        return false;
    }

}

bool ConsoleDock::containsBenchmarks(const QString &text) {
    if(text.contains("{ BENCHMARK }")) {
        // process and add to list with proper icon for state and other data

        return true;
    }
    else {
        return false;
    }
}

bool ConsoleDock::containsFuzzers(const QString &text) {
    return false;
}

void ConsoleDock::processFromFile() {
    // separate to sentences and call standart function for console, works fine
    QSettings settings("Evolution");
    const QString basePath = settings.value("Evolution/executable_path").toString();
    FileDirManager file_manager;

    // tests
    QString content = file_manager.read(basePath + "Tests.txt");
    QStringList sentences = content.split("\n");
    for(const auto& s : sentences) {
        containsTests(s);
    }

    // benchmarks
    content = file_manager.read(basePath + "Benchmarks.txt");
    sentences = content.split("\n");
    for(const auto& s : sentences) {
        containsBenchmarks(s);
    }

    // TODO: fuzzers
}

ConsoleDock::Link ConsoleDock::findLink(const QString &filepath, const Direction &direction) const {
    for (auto it = Links.begin(); it != Links.end(); it++) {
        if (it->filePath == filepath) {
            if (direction == Direction::Current)
                return *it;
            if (direction == Direction::Next)
                return *it++;
            if (direction == Direction::Previous)
                return *it--;
        }
    }
    return Link{};
}

void ConsoleDock::clearConsole() const {
    ConsoleOutput->clear();
}

void ConsoleDock::slotScrollUp() const {
    ConsoleOutput->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
}

void ConsoleDock::slotScrollDown() const {
    ConsoleOutput->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
}
