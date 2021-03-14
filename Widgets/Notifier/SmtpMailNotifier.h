#ifndef EVOLUTION_IDE_SMTPMAILNOTIFIER_H
#define EVOLUTION_IDE_SMTPMAILNOTIFIER_H


#include <QObject>
#include "simple-mail/src/server.h"
#include "simple-mail/src/emailaddress.h"
#include "simple-mail/src/mimemessage.h"
#include "simple-mail/src/sender.h"
#include "simple-mail/src/mimetext.h"
#include "simple-mail/src/serverreply.h"

class SmtpMailNotifier : public QObject {
Q_OBJECT
public:
    explicit SmtpMailNotifier(QObject *parent = nullptr);

    ~SmtpMailNotifier() = default;

    void sendMessage(const QString &msg, const QString &subject);

    SimpleMail::ServerReply *reply;

};


#endif //EVOLUTION_IDE_SMTPMAILNOTIFIER_H
