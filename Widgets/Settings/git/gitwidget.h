#ifndef GITWIDGET_H
#define GITWIDGET_H

#include <QWidget>

class GitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GitWidget(QWidget *parent = nullptr);
    void loadData();    // loading data from settings

signals:

};

#endif // GITWIDGET_H
