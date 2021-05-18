#ifndef COMPILATIONFLAGS_H
#define COMPILATIONFLAGS_H

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QPlainTextEdit>
#include <QHBoxLayout>

class CompilationFlags : public QWidget
{
    Q_OBJECT
public:
    explicit CompilationFlags(QWidget *parent = nullptr);
    ~CompilationFlags() = default;










private:
    void buildWindow();

    QHBoxLayout *MainLayout;
    QListWidget *optionContainer;
    QStackedWidget *stack;
    // also editable, inserts already used flags from cmake or so + insert to it by emited signal
    QPlainTextEdit *constructedFlags;

    // stacked lists of flags
    // reacts to click
    QListWidget *templates; // first in col
    QListWidget *debugInformations;
    QListWidget *optimization;
    QListWidget *sanitizers;


private slots:
    void addFlag(QListWidgetItem *item);

signals:
    void insertFlags(const QString& flags);



private:
    const QString ReferencePage = "https://clang.llvm.org/docs/UsersManual.html";

    const QList<QPair<QString, QString>> TemplatesFlags = {

    };

    // flag, description(used as a hover information, tooltip)
    const QList<QPair<QString, QString>> DebugInformationsFlags = {
        QPair{ "-g0", "No debug information" },
        QPair{ "-g", "debug information" }
    };

    const QList<QPair<QString, QString>> OptimizationFlags = {
        QPair{ "-O0", "" },
        QPair{ "-O1", "" },
        QPair{ "-O2", "" },
        QPair{ "-O3", "" },
        QPair{ "-O4", "" },
        QPair{ "-Os", "" }
    };

    const QList<QPair<QString, QString>> SanitizersFlags = {

    };





};

#endif // COMPILATIONFLAGS_H
