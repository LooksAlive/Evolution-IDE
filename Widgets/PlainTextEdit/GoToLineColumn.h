#ifndef EVOLUTION_IDE_GOTOLINECOLUMN_H
#define EVOLUTION_IDE_GOTOLINECOLUMN_H

#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QString>

//class PlainTextEditExtra;
class PlainTextEdit;

class GoToLineColumn : public QWidget {
Q_OBJECT
public:
    explicit GoToLineColumn(PlainTextEdit *textEdit);
    //explicit GoToLineColumn(PlainTextEditExtra *textEdit);
    ~GoToLineColumn() = default;

private:
    QFormLayout *MainLayout;
    QLineEdit *line_input;
    void createWindow();

    PlainTextEdit *edit = nullptr;
    //PlainTextEditExtra *editExtra = nullptr;

private slots:
    void storeData();

};


#endif //EVOLUTION_IDE_GOTOLINECOLUMN_H
