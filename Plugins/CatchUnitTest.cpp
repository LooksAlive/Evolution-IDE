#include <QMessageBox>

#include "Widgets/PlainTextEdit/plaintextedit.h"
#include "DependencyInstaller.h"

#include "CatchUnitTest.h"


CatchUnitTest::CatchUnitTest(PlainTextEdit *e) : edit(e) {
    setVisible(false);
    setMinimumSize(300, 200);

    createWindow();
}

void CatchUnitTest::createWindow() {

}

void CatchUnitTest::designTest(const QPoint &posInText, const QStringList &variables, const QStringList &functions) {
    show();
}

void CatchUnitTest::insertSimpleTestCase(const QPoint &posInText, const QStringList &variables, const QStringList &functions) {
    QTextCursor cursor = edit->textCursor();
    edit->setCursorPosition(cursor, posInText.x(), posInText.y());
    // TODO: check formating
    cursor.insertText("\nTEST_CASE(\"Message\", \"[Identifier]\") { \n     "
                      "REQUIRE( 2*3 == 6 );\n"
                      "REQUIRE( 3*3 == 9 );\n}");
    edit->setTextCursor(cursor);
}



void CatchUnitTest::checkCatchStatus() {
    if(!alreadyInstalledCathFramework) {
        const int reply = QMessageBox::warning(edit, "Install Catch", "It seems that you have not installed catch framwork."
                             "Do you want to install it ?");
        if(reply) {
            DependencyInstaller::install(DependencyInstaller::Catch);
        }
        else {
            return;
        }
    }

    // check header and macro declared
    if(!edit->find("Catch.hpp") || !edit->find("#define CATCH_CONFIG_MAIN")) {
        const int reply = QMessageBox::warning(edit, "Include header", "It seems that you have not included Catch.hpp header or required macro."
                             "Do you want to include it ?");
        if(reply) {
            QTextCursor cursor = edit->textCursor();
            edit->setCursorPosition(cursor, 1, 1);
            cursor.insertText("\n#include <Catch.hpp>");
            // edit->setTextCursor(cursor);
        }
        else {
            return;
        }
    }
}
