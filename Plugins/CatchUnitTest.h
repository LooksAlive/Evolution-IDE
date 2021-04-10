#ifndef CATCHUNITTEST_H
#define CATCHUNITTEST_H


#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>


class PlainTextEdit;
// https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md

class CatchUnitTest : public QWidget
{
public:
    explicit CatchUnitTest(PlainTextEdit *e = nullptr);
    ~CatchUnitTest() = default;

    // shows interactive user friendly window to design certain test structure
    // with known scope symbols
    void designTest(const QPoint& posInText, const QStringList& variables = QStringList(), const QStringList& functions = QStringList());

    // TEST_CASE with 2 REQUIRE()
    void insertSimpleTestCase(const QPoint& posInText, const QStringList& variables = QStringList(), const QStringList& functions = QStringList());

    void insertTestStructure();


    // if linux, just install, windows (how ?, maybe download online) for now only message

    bool alreadyInstalledCathFramework = false;

private:
    void createWindow();
    PlainTextEdit *edit;

    void checkCatchStatus();

};

#endif // CATCHUNITTEST_H
