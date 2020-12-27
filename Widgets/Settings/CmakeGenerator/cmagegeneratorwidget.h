#ifndef CMAGEGENERATORWIDGET_H
#define CMAGEGENERATORWIDGET_H

/*

  settings window as widget
  #TODO: option -j -> cores + all of options defined in cmakegenerator.h

*/


#include <QWidget>

#include "cmakegenerator.h"

class CmageGeneratorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CmageGeneratorWidget(QWidget *parent = nullptr);
    ~CmageGeneratorWidget();

    void loadData();    // loading data from settings

signals:

};

#endif // CMAGEGENERATORWIDGET_H
