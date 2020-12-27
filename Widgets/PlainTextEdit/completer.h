#ifndef COMPLETER_H
#define COMPLETER_H

#include <QCompleter>

class Completer : public QCompleter
{
    Q_OBJECT
public:
    Completer(QObject *parent = nullptr);
    ~Completer();
};

#endif // COMPLETER_H
