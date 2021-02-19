#ifndef EVOLUTION_IDE_INSTALLER_H
#define EVOLUTION_IDE_INSTALLER_H

/*
 *
*/

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QWidget>


class Installer : public QWidget {
    Q_OBJECT
public:
    explicit Installer(QWidget *parent = nullptr);
    ~Installer() = default;

    void saveData();
    void loadData();

private:
    QFormLayout *MainLayout;

    // TODO: try to get maximum available versions, but also track if user change this paths
    QLineEdit *clang;
    QLineEdit *gcc;
    QLineEdit *llvm;
    QLineEdit *gdb;
    QLineEdit *lldb;
    QLineEdit *qt5_default;
    QLineEdit *python3;

    void createWindow();
};


#endif//EVOLUTION_IDE_INSTALLER_H
