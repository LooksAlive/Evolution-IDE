#include "ProgressBar.h"

ProgressBar::ProgressBar(QWidget *parent) : QProgressBar(parent){
    setOrientation(Qt::Horizontal);
    setVisible(true);   // false, only if something is pending ...
    setMouseTracking(true);
    setTabletTracking(true);
    setValue(0);        // also outside
    setFixedWidth(100);
    setFormat("%p");
}