#ifndef EVOLUTION_IDE_CLANGFORMAT_H
#define EVOLUTION_IDE_CLANGFORMAT_H

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QWidget>

class ClangFormat : public QWidget {
    Q_OBJECT
public:
    explicit ClangFormat(QWidget *parent = nullptr);
    ~ClangFormat() = default;

    void saveData();
    void loadData();

private:
    QFormLayout *MainLayout;
    QComboBox *style;
    QCheckBox *afterFileClosed;

    void createWindow();
    void setDefaultSettings();
};


#endif//EVOLUTION_IDE_CLANGFORMAT_H
