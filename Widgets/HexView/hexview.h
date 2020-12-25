#ifndef HEXEDIT_H
#define HEXEDIT_H

#include <QWidget>
#include "QHexView/qhexview.h"

class HexView : public QWidget
{
    Q_OBJECT
private:
    QString m_FilePath;
    QHexView *m_HexView;
public:
    explicit HexView(QWidget *parent = nullptr);
    QString filePath();
    void open(const QString &path);
    bool save();
};

#endif // HEXEDIT_H
