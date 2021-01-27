#ifndef EVOLUTION_IDE_PROGRESSBAR_H
#define EVOLUTION_IDE_PROGRESSBAR_H

#include <QProgressBar>
#include <QWidget>

class ProgressBar : public QProgressBar{
Q_OBJECT
public:
    explicit ProgressBar(QWidget *parent = nullptr);
    ~ProgressBar() = default;


private:
    void createProgressBar();
};


#endif //EVOLUTION_IDE_PROGRESSBAR_H
