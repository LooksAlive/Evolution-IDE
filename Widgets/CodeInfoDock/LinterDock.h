#ifndef EVOLUTION_IDE_LINTERDOCK_H
#define EVOLUTION_IDE_LINTERDOCK_H

#include <QDockWidget>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

class LinterDock : public QDockWidget {
    Q_OBJECT
public:
    explicit LinterDock(QWidget *parent = nullptr);
    ~LinterDock() = default;

    QToolBar *TitleBar;
    QLabel *filePath;

    // DoubleClick open location connected outside
    QListWidget *Items;

    enum Background {
        Warning = 0,
        Error
    };

    void addItem(const QString &content, const Background &background) const;

private:
    void createWindow();
};


#endif//EVOLUTION_IDE_LINTERDOCK_H
