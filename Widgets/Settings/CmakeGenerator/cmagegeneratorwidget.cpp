#include "cmagegeneratorwidget.h"

CmageGeneratorWidget::CmageGeneratorWidget(QWidget *parent) : QWidget(parent)
{

}
CmageGeneratorWidget::~CmageGeneratorWidget(){}

void CmageGeneratorWidget::loadSettings() {
    QSettings settings("Evolution");
}

void CmageGeneratorWidget::saveSettings() {
    QSettings settings("Evolution");
    settings.setValue("flags", flags);
}
