#ifndef EVOLUTION_IDE_HOVERINFO_H
#define EVOLUTION_IDE_HOVERINFO_H

#include <QWidget>
#include <QLabel>

class HoverInfo : public QWidget {
Q_OBJECT
public:
    HoverInfo(QWidget *parent = nullptr);

    ~HoverInfo() = default;

    void setLabelText(const QString &label, const QIcon &icon);

private:
    QLabel *Label;

};


#endif //EVOLUTION_IDE_HOVERINFO_H
