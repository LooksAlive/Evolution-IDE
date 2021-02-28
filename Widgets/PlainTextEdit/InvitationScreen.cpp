#include "InvitationScreen.h"

InvitationScreen::InvitationScreen(QWidget *parent) : QPlainTextEdit(parent) {
    setInvitationMessage();
}

void InvitationScreen::setInvitationMessage() {
    const QString html = "<span style=color:gray;>New file: </span><span style=color:blue;>CTRL + N</span>";
    appendHtml(html);
}
