#include "SmtpMailNotifier.h"

SmtpMailNotifier::SmtpMailNotifier(QObject *parent) : QObject(parent) {

}

void SmtpMailNotifier::sendMessage(const QString &msg, const QString &subject) {
    auto server = new SimpleMail::Server(this);

    // We will use the Gmail's smtp server (smtp.gmail.com, port 465, ssl)
    server->setHost("smtp.gmail.com");
    server->setPort(465);
    server->setConnectionType(SimpleMail::Server::SslConnection);

    // We need to set the username (your email address) and the password for smtp authentication.
    server->setUsername("myEmail");
    server->setPassword("myPass");

    // Now we create a MimeMessage object. This will be the email.
    SimpleMail::MimeMessage message;
    SimpleMail::EmailAddress sender("myEmail");
    message.setSender(sender);

    SimpleMail::EmailAddress to(sender);
    message.addTo(to);

    if (subject.isEmpty()) {
        message.setSubject("anonymous user");
    } else {
        message.setSubject(subject);
    }

    // First we create a MimeText object.
    // This must be created with new otherwise it will be deleted once we leave the scope.
    auto text = new SimpleMail::MimeText;

    // Now add some text to the email.
    text->setText(msg + ". Hello its me.");

    // Now add it to the mail
    message.addPart(text);

    reply = server->sendMail(message);
}
