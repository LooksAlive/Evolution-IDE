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
    ~HexView();

    QString filePath() const;
    void open(const QString &path);
    bool save();
    void setText(const QByteArray &data);
};

#endif // HEXEDIT_H
