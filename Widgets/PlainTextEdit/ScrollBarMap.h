#ifndef SCROLLBARMAP_H
#define SCROLLBARMAP_H

#include <QWidget>

#include "SmallRoundedEdit.h"

class PlainTextEdit;


class ScrollBarMap : public QWidget {
Q_OBJECT
public:
    explicit ScrollBarMap(PlainTextEdit *edit, QWidget *parent = nullptr);

    ~ScrollBarMap() = default;


private:
    PlainTextEdit *m_Edit;
    SmallRoundedEdit *smallEdit;

    // center line mouse cursor points to, also middle
    void updateSmallEditGeometryAndScroll();


protected:
    // painting lines like in scrollBar
    void paintEvent(QPaintEvent *event) override;

    // go to line
    void mousePressEvent(QMouseEvent *event) override;

    // hide when leaving area; it is shown when mouse not moving for 2000ms (singleShot timer)
    void leaveEvent(QEvent *event) override;

    // show view according to mouse position
    void mouseMoveEvent(QMouseEvent *event) override;


signals:

};

#endif // SCROLLBARMAP_H
