#ifndef EVOLUTION_IDE_EDUCATION_H
#define EVOLUTION_IDE_EDUCATION_H

/*
 * This dock show widget with language specific samples:
 * double click will open a sample in tab window
 * one click will show a sample into preview
 
 * default C/C++ samples are Built In, they show code style, syntax, functionalities, 
 * with in some of them, comprehensive description.
 * they should serve as developers template and for learning purposes
 
 * user can create its own sample(with name)from editor menu with selected text,
 * later sample is formatted and added into list
 * samples can be renamed, edited in list 

 * in preview window you cannot modify or delete sample, user have to 
 * open it and them do desired actions



*/

#include <QDockWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QList>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QStackedWidget>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidget>

#include <iostream>
#include <vector>

#include "CodeSamples.h"
#include "NewSampleWindow.h"
#include "highlighter.h"

class Education : public QDockWidget {
    Q_OBJECT
public:
    explicit Education(QWidget *parent = nullptr);
    ~Education() = default;

    QListWidget *CppCodeSamples;
    // supports only simple 1 file sample created by selected text -> formatted and saved in editor
    QListWidget *CppUsersSamples;

    // TODO: consider preview widget under list with spacer
    // again i cannot import my custim edit, so it will be blank QPlainTextEdit
    // + readonly (possible copying, etc.), also can prompt highlighter
    // or can it be done some other way, from outside maybe ???

    // add users own sample, in separate view
    void addUserSample(const QString &content, const QString &sampleName);

    struct sample {
        // content of file sample, can be more that one related to files
        const std::vector<QString> content;
        // fileNames for each content -> mostly sample name + .cpp, .h, but
        // in the future there might actually be more files
        const std::vector<QString> fileNames;
    };

    // if there will be more that 1 content element
    const std::vector<sample> cpp_code_samples = {
            sample{{CodeSamples::Introduction_CPP_01}, {"Introduction.cpp"}},
            sample{{CodeSamples::Main_CPP_02}, {"Main.cpp"}},
            sample{{CodeSamples::Variables_CPP_03}, {"Variables.cpp"}},
            sample{{CodeSamples::For_Loop_CPP_04}, {"For_Loop.cpp"}},
    };

    // contents
    std::vector<QString> cpp_user_samples;

    // only for first file, if there are more, later consider some arrows to switch between them
    QPlainTextEdit *preview;
    // TODO: for if there will be also preview, have to inherit brain new widget with preview and list layout
    // TODO: 2 objects from this class, and they will be in stack
    // only readable, copyable
    void setPreviewHighlighter(Highlighter *highlighter) { previewHighlighter = highlighter; }

private:
    // double click on item will open new tab (not saved) with exact code sample dependent on type
    // sample which will be opened, will be marked with icon as done
    QStackedWidget *stack;
    QToolBar *titleBar;
    QToolButton *language;// click shows menu to switch between languages, for now only C++
    QToolButton *builtInSamples;

    QToolButton *usersSamples;

    // 1. sample name, 2. content  ---> at least 2 elements
    QStringList opened_samples;

    void createWindow();

    Highlighter *previewHighlighter;

    void loadUsersSamples();
    void saveUsersSamples();

protected:
    void closeEvent(QCloseEvent *event) override;


private slots:

    void slotShowCppSamples();
    void slotShowCppUsersSamples();
};

#endif //EVOLUTION_IDE_EDUCATION_H
