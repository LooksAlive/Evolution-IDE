#include "HoverInfoWidget.h"
#include <QDebug>

HoverInfoWidget::HoverInfoWidget(QWidget *parent) : QWidget(parent) {
    setVisible(false);
    // setMouseTracking(true);
    //setTabletTracking(true);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //Label text if formated --> no need to set to set colors etc.
    setStyleSheet("border: 1px solid rgb(65, 65, 65); background-color: rgb(50, 30, 50); padding: 2px;");
    //setMinimumSize(100, 50);

    createWidnow();
}

void HoverInfoWidget::setLabelText(const QString &label, const QString &position, const QString &documentation,
                                   const QIcon &icon) {
    MainWidget->clear();
    MainWidget->setWindowIcon(icon);

    MainWidget->append(label);
    if (!position.isEmpty())
        MainWidget->append("<br>" + position);
    if (!documentation.isEmpty())
        MainWidget->append("<br>" + documentation);

    // change widget dimensions according to text width
    setMinimumSize(MainWidget->fontMetrics().boundingRect(MainWidget->toPlainText()).width() + 15,
                   MainWidget->fontMetrics().boundingRect(MainWidget->toPlainText()).height() + 15);
    // qDebug() << MainWidget->toPlainText();
}

void HoverInfoWidget::createWidnow() {
    MainLayout = new QVBoxLayout();
    MainWidget = new QTextBrowser(this);

    //QPalette newPal;
    // newPal.setColor(QPalette::Link, Qt::white);
    //newPal.setColor(QPalette::LinkVisited, Qt::red);

    // for html text formatting
    MainWidget->setOpenExternalLinks(false);
    MainWidget->setOpenLinks(false);
    MainWidget->setTextInteractionFlags(Qt::TextBrowserInteraction);
    MainWidget->setReadOnly(true);
    MainWidget->setUpdatesEnabled(true);
    MainWidget->setWordWrapMode(QTextOption::WordWrap);
    MainWidget->setLineWrapMode(QTextEdit::WidgetWidth);
    MainWidget->setMarkdown("kjhdsf");
    //MainWidget->setPalette(newPal);
    MainWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    MainLayout->addWidget(MainWidget);
    MainLayout->setContentsMargins(0, 0, 0, 0);
    MainLayout->setSpacing(0);

    //auto *widget = new QWidget(this);
    //widget->setLayout(MainLayout);
    //auto *layout = new QVBoxLayout();
    //layout->setContentsMargins(0, 0, 0, 0);
    //layout->setSpacing(0);
    setLayout(MainLayout);
}

