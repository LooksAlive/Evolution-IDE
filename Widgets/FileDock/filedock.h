#ifndef FILEDOCK_H
#define FILEDOCK_H

#include <QDockWidget>
#include <QListWidget>

class FileDock : public QDockWidget
{
    Q_OBJECT
public:
    explicit FileDock(QWidget *parent = nullptr);
    ~FileDock();

    QListWidget *DockerFileList;
};

#endif // FILEDOCK_H
