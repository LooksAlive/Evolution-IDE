#ifndef ERRORCODELIST_H
#define ERRORCODELIST_H

/*
 * User defined error codes withing project, mainly used in unittesting
 * Each code has its own data structure.
 *
 * Provides a list of all registered codes, saves them into .json file and load with startup
*/

#include <QWidget>

class ErrorCodeList : public QWidget
{
    Q_OBJECT
public:
    explicit ErrorCodeList(QWidget *parent = nullptr);
    ~ErrorCodeList() = default;


private:
    void createWindow();


signals:

};

#endif // ERRORCODELIST_H
