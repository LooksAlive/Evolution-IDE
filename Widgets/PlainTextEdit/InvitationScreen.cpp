#include "InvitationScreen.h"

InvitationScreen::InvitationScreen(QWidget *parent) : QPlainTextEdit(parent) {
    setInvitationMessage();
}

void InvitationScreen::setInvitationMessage() {
    const QString html = "<span style=color:gray;> New file: </span> &nbsp;&nbsp;&nbsp; <span style=color:lightblue;> CTRL + N </span>"
                         "<br>"
                         "<span style=color:gray;> Open file: </span> &nbsp;&nbsp;&nbsp; <span style=color:lightblue;> CTRL + O </span>"
                         "<br>"
                         "<span style=color:gray;> Open Settings: </span> &nbsp;&nbsp;&nbsp; <span style=color:lightblue;> CTRL + P </span>"
                         "<br>"
                         "<span style=color:gray;> Show FullScreen: </span> &nbsp;&nbsp;&nbsp; <span style=color:lightblue;> F11 </span>"
                         "<br>"
                         "<span style=color:gray;> Restart: </span> &nbsp;&nbsp;&nbsp; <span style=color:lightblue;> CTRL + SHIFT + R </span>"
                         "<br>"
                         "<span style=color:gray;> Exit: </span> &nbsp;&nbsp;&nbsp; <span style=color:lightblue;> CTRL + Q </span>";

    appendHtml(html);

    setReadOnly(true);
    setEnabled(false);
    /*
    QTextBlockFormat bf = textCursor().blockFormat();
    bf.setLineHeight(400, QTextBlockFormat::ProportionalHeight);
    auto cursor = textCursor();
    cursor.select(QTextCursor::Document);
    cursor.setBlockFormat(bf);
    cursor.mergeBlockFormat(bf);
    cursor.clearSelection();
    setTextCursor(cursor);
    */
    // center
    setLineWrapMode(NoWrap);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setAttribute(Qt::WA_DeleteOnClose);

    QFont font;
    font.setPointSize(13);
    font.setFamily("Ubuntu");
    font.setItalic(true);
    font.setBold(true);
    // font.setKerning(true);
    setFont(font);
    // setViewportMargins(200, 200, 200, 200);
    document()->setDocumentMargin(250);
}
