#ifndef EVOLUTION_IDE_INVITATIONSCREEN_H
#define EVOLUTION_IDE_INVITATIONSCREEN_H

#include <QPlainTextEdit>

class InvitationScreen : public QPlainTextEdit {
    Q_OBJECT
public:
    explicit InvitationScreen(QWidget *parent = nullptr);
    ~InvitationScreen() = default;

private:
    void setInvitationMessage();

};


#endif//EVOLUTION_IDE_INVITATIONSCREEN_H
