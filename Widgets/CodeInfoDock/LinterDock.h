#ifndef EVOLUTION_IDE_LINTERDOCK_H
#define EVOLUTION_IDE_LINTERDOCK_H

#include <QDockWidget>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

#include "Diagnostics.h"    // diagnostic level for item highlight

class LinterDock : public QDockWidget {
Q_OBJECT
public:
    explicit LinterDock(QWidget *parent = nullptr);

    ~LinterDock() = default;

    QToolBar *TitleBar;
    QLabel *filePath;

    // DoubleClick open location connected outside
    QListWidget *Items;

    void addItem(const QString &content, const QString &filePath, const clang::DiagnosticsEngine::Level &level) const;

private:
    void createWindow();
};


#endif//EVOLUTION_IDE_LINTERDOCK_H
