#ifndef EVOLUTION_IDE_NEWSAMPLEWINDOW_H
#define EVOLUTION_IDE_NEWSAMPLEWINDOW_H

/*
 * small class shows the window with user defined sample name sa input
 * this name is set and sample is created and inserted into education dock
*/

#include <QFormLayout>
#include <QLineEdit>
#include <QWidget>


class NewSampleWindow : public QWidget {
    Q_OBJECT
public:
    explicit NewSampleWindow(QWidget *parent = nullptr);
    ~NewSampleWindow() = default;


    void newUserSampleWindow();
    QLineEdit *sampleName_input;
    QString newSampleName;

public slots:
    void slotSampleNameRegister();

signals:
    void sampleNameRegisterDestroyed();
};


#endif//EVOLUTION_IDE_NEWSAMPLEWINDOW_H
