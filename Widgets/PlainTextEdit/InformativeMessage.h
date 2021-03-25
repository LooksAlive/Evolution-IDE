#ifndef INFORMATIVEMESSAGE_H
#define INFORMATIVEMESSAGE_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>

class InformativeMessage : public QWidget {
Q_OBJECT
public:
    explicit InformativeMessage(QWidget *parent = nullptr);

    ~InformativeMessage() = default;

    void setMessage(const QString &message);

    QLabel *label;

protected:
    // void paintEvent(QPaintEvent *event) override;

private:
    QString Message;


};

#endif // INFORMATIVEMESSAGE_H
