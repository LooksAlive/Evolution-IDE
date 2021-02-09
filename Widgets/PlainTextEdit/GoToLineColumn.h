#ifndef EVOLUTION_IDE_GOTOLINECOLUMN_H
#define EVOLUTION_IDE_GOTOLINECOLUMN_H

#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QString>

#include <Widgets/PlainTextEdit/plaintextedit.h>

class GoToLineColumn : public QWidget{
Q_OBJECT
public:
    explicit GoToLineColumn(PlainTextEdit *textEdit, QWidget *parent = nullptr);
    ~GoToLineColumn() = default;

private:
    QFormLayout *MainLayout;
    QLineEdit *line_input;
    void createWindow();

    PlainTextEdit *edit = nullptr;

private slots:
    void storeData();

};


#endif //EVOLUTION_IDE_GOTOLINECOLUMN_H
