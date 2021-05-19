#ifndef EVOLUTION_IDE_HOVERINFOWIDGET_H
#define EVOLUTION_IDE_HOVERINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QIcon>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextBrowser>

class HoverInfoWidget : public QWidget {
Q_OBJECT
public:
    HoverInfoWidget(QWidget *parent = nullptr);
    ~HoverInfoWidget() = default;

    // already formated html
    void
    setLabelText(const QString &label, const QString &position = QString(), const QString &documentation = QString(),
                 const QIcon &icon = QIcon());

    // text size from fontMetrics before html has to be set before formating
    QTextBrowser *MainWidget;

private:
    QVBoxLayout *MainLayout;

    // href:  style=text-decoration:none;  || line-through  || underline   ||   overline

    // each label owns it own link with filePath:posx:posy   ; when clicked it is separated and opened for user

    void createWidnow();

};


#endif //EVOLUTION_IDE_HOVERINFOWIDGET_H
