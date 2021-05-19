#ifndef COMPILATIONFLAGS_H
#define COMPILATIONFLAGS_H

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QToolBar>
#include <QToolButton>

class CompilationFlags : public QWidget
{
    Q_OBJECT
public:
    explicit CompilationFlags(QWidget *parent = nullptr);
    ~CompilationFlags() = default;










private:
    void buildWindow();

    QHBoxLayout *MainLayout;
    QToolBar *MainBar;
    QToolButton *close;
    QToolButton *clear;
    QToolButton *copy;
    QToolButton *reference;

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
    static constexpr auto ReferencePage = "https://clang.llvm.org/docs/UsersManual.html";

    static constexpr auto TemplatesFlags = {
        QPair{ "Debug 1", "" },
        QPair{ "Debug 2", "" },
        QPair{ "Debug 3", "" },
        QPair{ "Optimize 1", "" },
        QPair{ "Optimize 2", "" },
        QPair{ "Optimize 3", "" }
    };

    // flag, description(used as a hover information, tooltip)
    static constexpr auto DebugInformationsFlags = {
        QPair{ "-g0", "No debug information" },
        QPair{ "-g", "debug information" }
    };
    // const QList<QPair<QString, QString>>
    static constexpr auto OptimizationFlags = {
        QPair{ "-O0", "" },
        QPair{ "-O1", "" },
        QPair{ "-O2", "" },
        QPair{ "-O3", "" },
        QPair{ "-O4", "" },
        QPair{ "-Os", "" }
    };

    static constexpr auto SanitizersFlags = {
        QPair{ "-fsanitize-address", "" }
    };





};

#endif // COMPILATIONFLAGS_H
