#include "InvitationScreen.h"

InvitationScreen::InvitationScreen(QWidget *parent) : QPlainTextEdit(parent) {
    setInvitationMessage();
}

void InvitationScreen::setInvitationMessage() {
    const QString general = "<h1 style=color:gray;> General </h1>"
                            "<br>"
                            "<span style=color:gray;> New file: </span> &nbsp;&nbsp;&nbsp; <span style=color:lightblue;> CTRL + N </span>"
                            "<br>"
                            "<span style=color:gray;> Open file: </span> &nbsp;&nbsp;&nbsp; <span style=color:lightblue;> CTRL + O </span>"
                            "<br>"
                            "<span style=color:gray;> Open Settings: </span> &nbsp;&nbsp;&nbsp; <span style=color:lightblue;> CTRL + P </span>"
                            "<br>"
                            "<span style=color:gray;> Show FullScreen: </span> &nbsp;&nbsp;&nbsp; <span style=color:lightblue;> F11 </span>"
                            "<br>"
                            "<span style=color:gray;> Restart: </span> &nbsp;&nbsp;&nbsp; <span style=color:lightblue;> CTRL + SHIFT + R </span>"
                            "<br>"
                            "<span style=color:gray;> Exit: </span> &nbsp;&nbsp;&nbsp; <span style=color:lightblue;> CTRL + Q </span>"
                            "<br><br>"
                            "<span style=color:gray;> Drop Files here </span>"
                            "<br><br><br><br>";

    appendHtml(general);

    const QString editorManual = "<h1 style=color:gray;> Editor </h1>"
                                 "<br>"
                                 "<span style=color:lightblue;> CTRL + SHIFT + UP/DOWN </span>&nbsp;&nbsp;&nbsp; <span style=color:gray;> Move whole line </span>"
                                 "<br>"
                                 "<span style=color:lightblue;> CTRL + S </span>&nbsp;&nbsp;&nbsp; <span style=color:gray;> Save File </span>"
                                 "<br>"
                                 "<span style=color:lightblue;> CTRL + C </span>&nbsp;&nbsp;&nbsp; <span style=color:gray;> Copy whole line </span>"
                                 "<br>"
                                 "<span style=color:lightblue;> CTRL + X </span>&nbsp;&nbsp;&nbsp; <span style=color:gray;> Remove whole line </span>"
                                 "<br>"
                                 "<span style=color:lightblue;> CTRL + U </span>&nbsp;&nbsp;&nbsp; <span style=color:gray;> To Upper case </span>"
                                 "<br>"
                                 "<span style=color:lightblue;> CTRL + SHIFT + U </span>&nbsp;&nbsp;&nbsp; <span style=color:gray;> To Lower case </span>"
                                 "<br>"
                                 "<span style=color:lightblue;> CTRL + W </span>&nbsp;&nbsp;&nbsp; <span style=color:gray;> Close Current Tab </span>"
                                 "<br>"
                                 "<span style=color:lightblue;> CTRL + SCROLL/ + - </span>&nbsp;&nbsp;&nbsp; <span style=color:gray;> Increase, Decrease text size </span>"
                                 "<br>"
                                 "<span style=color:lightblue;> CTRL + UP/DOWN </span>&nbsp;&nbsp;&nbsp; <span style=color:gray;> Move view 1 line Up/Down </span>"
                                 "<br>";
    appendHtml(editorManual);

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
    font.setFamily("Ubuntu Mono");
    font.setItalic(true);
    font.setBold(true);
    // font.setKerning(true);
    setFont(font);
    // setViewportMargins(200, 200, 200, 200);
    document()->setDocumentMargin(50);
}
