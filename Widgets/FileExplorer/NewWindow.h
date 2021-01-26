#ifndef EVOLUTION_IDE_NEWWINDOW_H
#define EVOLUTION_IDE_NEWWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>

class NewWindow : public QWidget{
    Q_OBJECT
public:
    explicit NewWindow(QWidget *parent = nullptr);
    ~NewWindow() = default;

    void dir_Window();
    void file_Window();
    void C_CPP_CLASS_Window();
    void rename_Window(const QString &old);

    QString getDir() const;   // dir name
    QString getName() const;    // file name
    QString getCPP() const;   // cpp name
    QString getH() const;     // header name
    QString getNewName() const; // rename

    QString new_dirname;
    QString new_cpp;
    QString new_h;
    QString new_file;
    QString new_name;   // rename


private:

    QHBoxLayout *mainLayout{};

    // dir_Window
    QFormLayout *dir_layout{};
    QLineEdit *dir_input{};

    // CppOrH_Window
    QFormLayout *file_layout{};
    QLineEdit *file_input{};

    // CppAndH_Window
    QFormLayout *C_CPP_CLASS_layout{};
    QLineEdit *ClassName_input{};
    QLabel *CPP_tag{};
    QLabel *H_tag{};

    // rename
    QFormLayout *rename_layout{};
    QLineEdit *rename_input{};
    QLabel *old_tag{};

public slots:
    void slotDir();
    void slotFile();
    void slotC_CPP_CLASS();
    void slotC_CPP_CLASS_LABEL_UPDATE();
    void slotRename();

signals:
    // my own signal since im creating different widgets in 1 class cannot use only destroyed (qobject) signal
    void PartialWidgetDestroyed();
};


#endif //EVOLUTION_IDE_NEWWINDOW_H
