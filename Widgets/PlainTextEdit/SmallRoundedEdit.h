#ifndef SMALLROUNDEDEDIT_H
#define SMALLROUNDEDEDIT_H

#include <QWidget>
#include <QPlainTextEdit>


class PlainTextEdit;
// class PlainTextEditExtra;


/*
 * shows collapsed code like edit
 * requires to set width, height, and geometry
*/

class SmallRoundedEdit : public QPlainTextEdit {
Q_OBJECT
public:
    explicit SmallRoundedEdit(PlainTextEdit *edit);
    // explicit SmallRoundedEdit(PlainTextEditExtra *edit);

    ~SmallRoundedEdit() = default;

    bool inWidget = false;
    int timesAplified = 1;

private:
    PlainTextEdit *m_Edit = nullptr;
    // PlainTextEditExtra *editExtra = nullptr;


protected:
    void enterEvent(QEvent *event) override;

    void leaveEvent(QEvent *event) override;

    // amplifying line range with CTRL
    void wheelEvent(QWheelEvent *event) override;


signals:

};

#endif // SMALLROUNDEDEDIT_H
