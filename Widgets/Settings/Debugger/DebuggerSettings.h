#ifndef EVOLUTION_IDE_DEBUGGERSETTINGS_H
#define EVOLUTION_IDE_DEBUGGERSETTINGS_H

#include <QWidget>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>

class DebuggerSettings  : public QWidget {
    Q_OBJECT
public:
    explicit DebuggerSettings(QWidget *parent = nullptr);
    ~DebuggerSettings() = default;

    void saveData();
    void loadData();

private:

    QFormLayout *Main_Layout;

    QLineEdit *executable;

    void createWindow();

};


#endif //EVOLUTION_IDE_DEBUGGERSETTINGS_H
