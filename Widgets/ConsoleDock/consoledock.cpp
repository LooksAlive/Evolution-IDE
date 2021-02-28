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

    ConsoleOutput->setHtml("<a href = http://google.com > moj text </a>");
    ConsoleOutput->append("<a href = http://google.com > moj text </a>");
    processText("/home/adam/Desktop/Qt5_forum \n");                                                                                                   // TODO: bad formatting preserve !!!!!!!!!!
    ConsoleOutput->append("<a style=color:red; href = /home/adam/Desktop/Qt5_forum > /home/adam/Desktop/Qt5_forum </a>");                             // works well
    ConsoleOutput->append("<a style='color:red;background-color:lightblue;' href = /home/adam/Desktop/Qt5_forum > /home/adam/Desktop/Qt5_forum </a>");// works well
    ConsoleOutput->append("<a style=color:lightblue; href = /home/adam/Desktop/Qt5_forum > /home/adam/Desktop/Qt5_forum </a>");                       // works well
}


void ConsoleDock::processText(const QString &text) {
    /*
    // separate to words + also for performance check for lenght
    const QString Stag = "<a href = ";
    const QString Mtag = " > ";
    QString tagText = "";  // here goes real text to be shown to user (html syntax)
    const QString Etag = " </a>";
    // right after <a {}        ++ possible background ,  !! hover <style> not working ...
    const QString styleWarning = "style=color:yellow";
    const QString styleError = "style=color:red";

    QString ProcessedText = text;
    QString link;
    QString temp_link;

    bool Warning = false;
    bool Error = false;

    if(text.contains("Warning")){
        Warning = true;
    }
    if(text.contains("Error")){
        Error = true;
    }

    // separate to words
    const QStringList words = text.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    for (int word = 0; word < words.length(); word++) {
        const QString CWORD = words[word];      // helper string

        if(CWORD.length() <= 5){
            ProcessedText.append(CWORD);
            break;
        }
        // is this condition necessary ??
        if(CWORD.contains("/")){
            // select whole link
            bool followTillEnd = false;
            int count = 0;
            for (int i = 0; i< CWORD.length(); i++) {
                // every path starts with /, on windows with opposite
                if(CWORD[i] == "/"){
                    count++;
                }
                if(CWORD[i] == "/" && !followTillEnd){
                    // we found path, follow to end
                    followTillEnd = true;
                    link.append("/");
                }
                if(followTillEnd){
                    link.append(CWORD[i]);
                }
                if(count && followTillEnd && QFileInfo(link).exists()){
                    temp_link = link;
                }
                // There is no way that 2 links can be merged so no need to clear variables
                // encapsulate into href link
                if(CWORD.end() && !QFileInfo(link).exists()){
                    // final part, take last valid link ... temp_link
                    tagText = temp_link;
                    temp_link.insert(0, Stag);
                    // change is text is good now
                    if(Warning){
                        temp_link.insert(Stag.length(), styleWarning + Mtag + " ");
                        temp_link.insert(Stag.length() + styleWarning.length() + Mtag.length(), tagText);
                        temp_link.insert(Stag.length() + styleWarning.length() + Mtag.length() + tagText.length(), Etag);
                    }
                    if(Error){
                        temp_link.insert(Stag.length(), styleError + Mtag +  " ");
                        temp_link.insert(Stag.length() + styleError.length() + Mtag.length(), tagText);
                        temp_link.insert(Stag.length() + styleError.length() + Mtag.length() + tagText.length(), Etag);
                    }
                    else
                        temp_link.insert(Stag.length(), Mtag + tagText + Etag);
                }
                if(CWORD.end() && QFileInfo(link).exists()){
                    // final part, take valid link ... link
                    tagText = temp_link;
                    link.insert(0, Stag);
                    // change is text is good now
                    if(Warning){
                        link.insert(Stag.length(), styleWarning + Mtag + " ");
                        link.insert(Stag.length() + styleWarning.length() + Mtag.length(), tagText);
                        link.insert(Stag.length() + styleWarning.length() + Mtag.length() + tagText.length(), Etag);
                    }
                    if(Error){
                        link.insert(Stag.length(), styleError + Mtag +  " ");
                        link.insert(Stag.length() + styleError.length() + Mtag.length(), tagText);
                        link.insert(Stag.length() + styleError.length() + Mtag.length() + tagText.length(), Etag);
                    }
                    else
                        link.insert(Stag.length(), Mtag + tagText + Etag);
                }
                // often after links follows position in format row:col ; or line: xx ; or line xx

                if(CWORD[i] != "/" && !followTillEnd){
                    ProcessedText.append(CWORD[i]);
                }
            }
            link.clear();
            temp_link.clear();
            tagText.clear();
            count = 0;
        }
        else{
            // no link there, just append it
            ProcessedText.append(words[word]);
        }
    }
    ConsoleOutput->append(ProcessedText);
    */
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
