#ifndef EVOLUTION_IDE_REFACTORINGDOCK_H
#define EVOLUTION_IDE_REFACTORINGDOCK_H

#include <QDockWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QPushButton>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWidget>

class RefactoringDock : public QDockWidget {
    Q_OBJECT
public:
    explicit RefactoringDock(QWidget *parent = nullptr);
    ~RefactoringDock() = default;

    QWidget *MainWidget;
    QVBoxLayout *MainLayout;

    QToolBar *TitleBar;
    QLabel *filePath;

    QHBoxLayout *RefactorItemLayout;

    // we can add as many refactors as we want, spawn widgets in layouts
    QList<QHBoxLayout *> rows;

    QLineEdit *Original;
    QLineEdit *Replacement;
    QPushButton *Refactor;

private:
    void createWindow();

    // if widget before is fully filled, we can spawn another
    void spawnNewRow();

signals:
    void refactorClicked(const int &);

public slots:
    void slotReport();
};


#endif//EVOLUTION_IDE_REFACTORINGDOCK_H
