#include "completer.h"

Completer::Completer(QObject *parent) : QCompleter(parent)
{
    setCaseSensitivity(Qt::CaseInsensitive);
    setMaxVisibleItems(8);
    setCompletionMode(InlineCompletion);
}

void Completer::setItems(const std::vector<std::string> &data) {
    for (int i = 0; i < data.size(); i++) {
        setCompletionPrefix(QString::fromStdString(data[i]));
    }
}
