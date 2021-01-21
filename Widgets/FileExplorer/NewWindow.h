#ifndef EVOLUTION_IDE_NEWWINDOW_H
#define EVOLUTION_IDE_NEWWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class NewWindow : public QWidget{
    Q_OBJECT
public:
    NewWindow(QWidget *parent = nullptr);
    ~NewWindow() = default;

    QString new_dirname;
    QString new_cppname;
    QString new_hname;

private:
    void dir_Window();
    void CppOrH_Window();
    void CppAndH_Window();

    QHBoxLayout *mainLayout;

    // dir_Window
    QHBoxLayout *dir_layout;
    QLabel *dir_tag;
    QLineEdit *dir_input;

    // CppOrH_Window
    QHBoxLayout *CppOrH_layout;
    QLabel *fileName_tag;
    QLineEdit *fileName_input;

    // CppAndH_Window
    QHBoxLayout *CppAndH_layout;
    QLabel *FileName_tag;
    QLineEdit *FileName_input;

public slots:
    void slotDir();
    void slotCppOrH();
    void slotCppAndH();
};


#endif //EVOLUTION_IDE_NEWWINDOW_H
