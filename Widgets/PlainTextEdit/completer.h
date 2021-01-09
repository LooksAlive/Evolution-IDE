#ifndef COMPLETER_H
#define COMPLETER_H

#include <QCompleter>
#include <QString>
#include <iostream>
#include <string>
#include <vector>


class Completer : public QCompleter
{
    Q_OBJECT
public:
    Completer(QObject *parent = nullptr);
    ~Completer() = default;

    void setItems(const std::vector<std::string> &data);



};

#endif // COMPLETER_H
