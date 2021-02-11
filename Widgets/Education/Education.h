#ifndef EVOLUTION_IDE_EDUCATION_H
#define EVOLUTION_IDE_EDUCATION_H

#include <QWidget>
#include <QListWidget>
#include <QList>

#include <iostream>
#include <vector>

#include "Widgets/Tab/tab.h"
#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "CodeSamples.h"

class Education : public QWidget{
Q_OBJECT
public:
    explicit Education(Tab *tab, QWidget *parent = nullptr);
    ~Education() = default;   // change later


private:
    // double click on item will open new tab (not saved) with exact code sample dependent on type
    // sample which will be opened, will be marked with icon as done
    QListWidget *CppCodeSamples;

    QList<int> opened_samples;

    void createWindow();
    Tab *m_Tab = nullptr;

    struct sample{
        // content of file sample
        const std::vector<QString> content;
        // fileNames for each content -> mostly sample name + .cpp, .h, but in the future there might be more files
        const std::vector<QString> fileNames;
    };
    // if there will be more that 1 content element
    const std::vector<sample> samples =
            {sample{{CodeSamples::Introduction_CPP_01}, {"Introduction.cpp"}},
             sample{{CodeSamples::Main_CPP_02}, {"Main.cpp"}},
             sample{{CodeSamples::Variables_CPP_03}, {"Variables.cpp"}},
             sample{{CodeSamples::For_Loop_CPP_04}, {"For_Loop.cpp"}},
             sample{{"asgf", "jkasdhf"}, {"Main.cpp"}},
             sample{{"asgf", "jkasdhf"}, {"Main.cpp"}},
             sample{{"asgf", "jkasdhf"}, {"Main.cpp"}},
             sample{{"asgf", "jkasdhf"}, {"Main.cpp"}},
            };

    void loadOpenedSamples();
    void saveOpenedSamples();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // index -> position in list
    void slotOpenFile(QListWidgetItem *item);

};



#endif //EVOLUTION_IDE_EDUCATION_H
