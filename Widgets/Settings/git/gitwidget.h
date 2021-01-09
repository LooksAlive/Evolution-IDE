#ifndef GITWIDGET_H
#define GITWIDGET_H

#include <QWidget>

class GitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GitWidget(QWidget *parent = nullptr);
    ~GitWidget();

    void loadData();    // loading data from settings
    void saveData();

signals:

};

#endif // GITWIDGET_H
