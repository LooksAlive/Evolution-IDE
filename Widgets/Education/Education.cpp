#include <QSettings>
#include <QIcon>
#include <QFormLayout>
#include <QToolBar>

#include "icons/IconFactory.h"
#include "Education.h"

Education::Education(Tab *tab, QWidget *parent) : QDockWidget(parent), m_Tab(tab)
{
    //setMinimumSize(400, 200);
    setContentsMargins(0, 0, 0, 0);
    //setWindowFlag(Qt::Dialog);
    setWindowTitle("Samples");
    //setWindowFlags(Qt::FramelessWindowHint);

    createWindow();

    // loadOpenedSamples();
    setFeatures(AllDockWidgetFeatures);
    setVisible(false);
    setWidget(CppCodeSamples);
}

void Education::createWindow() {
    CppCodeSamples = new QListWidget(this);

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

    connect(CppCodeSamples, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(slotOpenFile(QListWidgetItem *)));

    auto *t_bar = new QToolBar(this);
    auto *spacer = new QWidget(this);   // align to right with blank widget
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    t_bar->addWidget(spacer);
    t_bar->addAction(QIcon(IconFactory::Remove), "Close", this, SLOT(close()));
    t_bar->setToolButtonStyle(Qt::ToolButtonFollowStyle);
    t_bar->setFixedHeight(35);
    t_bar->setIconSize(QSize(25, 35));
    setTitleBarWidget(t_bar);
}

void Education::slotOpenFile(QListWidgetItem *item){
    // represents sample to take
    int index = item->listWidget()->currentRow();
    int num_files = samples[index].fileNames.size();

    if(num_files == 1){
        auto *edit = new PlainTextEdit;
        edit->setPlainText(samples[index].content[0]);  // 0 -> only 1 file
        m_Tab->addTab(edit, samples[index].fileNames[0]);
        m_Tab->setCurrentIndex(m_Tab->count()); // since we add new tab at the end
    }
    // 2 and more files
    if (num_files > 1){
        for (int i = 0; i <= num_files; i++) {
            auto *edit = new PlainTextEdit;
            edit->setPlainText(samples[index].content[i]);
            m_Tab->addTab(edit, samples[index].fileNames[i]);
            m_Tab->setCurrentIndex(m_Tab->count()); // since we add new tab at the end
        }
    }

    // set icon that it was opened + add its index into registry
    //item->setIcon(QIcon(IconFactory::Done));

    // also solve current widget in tab here !!!
    // take care of opening the same sample twice and more

}

void Education::loadOpenedSamples() {
    QSettings settings("Evolution-IDE");
    auto opened_samples = settings.value("Evolution-IDE/opened_samples").toList();
    /*
    for (int i = 0; i < CppCodeSamples->count(); i++) {
        if(opened_samples.at(i) == CppCodeSamples->itemAt(0, i)){
            CppCodeSamples->itemAt(0, i)->setIcon(IconFactory::Done);
        }
    }
    */
}

void Education::saveOpenedSamples() {
    QSettings settings("Evolution-IDE");
    settings.setValue("Evolution-IDE/opened_samples", QVariant::fromValue(opened_samples));
}

void Education::closeEvent(QCloseEvent *event) {
    //saveOpenedSamples();
    QWidget::closeEvent(event);
}
