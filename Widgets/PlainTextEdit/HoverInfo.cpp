#include "HoverInfo.h"

HoverInfo::HoverInfo(QWidget *parent) : QWidget(parent) {
    setVisible(false);
    setMouseTracking(true);
    setTabletTracking(true);

    // main widget
    Label = new QLabel(this);
    // for html text formatting
    Label->setTextFormat(Qt::RichText);
}

void HoverInfo::setLabelText(const QString &label, const QIcon &icon) {
    Label->setText(label);
    Label->setWindowIcon(icon);

    // change widget dimensions according to text width
    setFixedWidth(Label->width());
    setFixedHeight(Label->height());
}
