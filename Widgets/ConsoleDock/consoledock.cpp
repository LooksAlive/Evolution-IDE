#include <QDebug>
#include <QFileInfo>
#include <QScrollBar>
#include <QShortcut>
#include <QString>
#include <iostream>

#include "icons/IconFactory.h"
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
    connect(new QShortcut(Qt::Key_Escape, this), &QShortcut::activated, [=] {this->setVisible(false);});

    BuildConsole();
    setTitleBarWidget(title_bar);

    auto *window = new QWidget(this);
    window->setLayout(MainLayout);
    window->setContentsMargins(0, 0, 0, 0);

    setWidget(window);

}

// change to QListWidget most probably, bc. of specific widget shows its own data and do not know
// how to do that with tabs, at least yet
void ConsoleDock::BuildConsole() {
    MainLayout = new QHBoxLayout();
    tool_bar = new QToolBar(this);
    title_bar = new QToolBar(this);
    ConsoleOutput = new QTextBrowser(this);

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
    tool_bar->setContentsMargins(0, 0, 0, 0);

    title_bar->setMovable(false);
    title_bar->setFixedHeight(30);
    title_bar->setFloatable(false);
    title_bar->setAcceptDrops(false);
    //title_bar->setIconSize(QSize(25, 35));
    title_bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    auto *spacer = new QWidget(this);// align title_bar to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    title_bar->addWidget(spacer);
    title_bar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));


    MainLayout->addWidget(tool_bar);
    MainLayout->addWidget(ConsoleOutput);

    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);

    //ConsoleOutput->setHtml("<a href = http://google.com > moj text </a>");
    //ConsoleOutput->append("<a href = http://google.com > moj text </a>");
    processText("Warning /home/adam/Desktop/Qt5_forum kjskldafjhk kjsadfhkjsdlhakjf gfdsgakhfg");
    processText("Error /home/adam/Desktop/GITHUB sjkdfh b");
    processText("kkwjrlwhetjhlew jkew /home/adam/Desktop/GITHUB lahfjakslhdj ");
    //processText("kkwjrlwhetjhlew    /home/adam/Desktop/GITHUB");
    //ConsoleOutput->append("<a style=color:red; href = /home/adam/Desktop/Qt5_forum > /home/adam/Desktop/Qt5_forum </a>");                             // works well
    //ConsoleOutput->append("<a style='color:red;background-color:lightblue;' href = /home/adam/Desktop/Qt5_forum > /home/adam/Desktop/Qt5_forum </a>");// works well
    //ConsoleOutput->append("<a style=color:lightblue; href = /home/adam/Desktop/Qt5_forum > /home/adam/Desktop/Qt5_forum </a>");                       // works well
}


void ConsoleDock::processText(const QString &text) const {
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

    bool Warning = false;
    bool Error = false;

    if (text.contains("Warning")) {
        Warning = true;
    }
    if (text.contains("Error")) {
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

            if (QFileInfo(link).exists() && !QFileInfo(link).isDir()) {
                i = pos;// jump over link
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
        }
        // often after links follows position in format row:col ; or line: xx ; or line xx

        else {
            ProcessedText.append(text[i]);
        }
    }

    ConsoleOutput->append(ProcessedText);
}

void ConsoleDock::clearConsole() const {
    ConsoleOutput->clear();
}

void ConsoleDock::slotScrollUp() const {
    for (int i = 0; i <= 5; i++) {
        ConsoleOutput->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
    }
}

void ConsoleDock::slotScrollDown() const {
    for (int i = 0; i <= 5; i++) {
        ConsoleOutput->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
    }
}
