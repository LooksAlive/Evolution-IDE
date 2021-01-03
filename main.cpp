#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    QFile file("/home/adam/Desktop/sources/Evolution-IDE/stylesheets/Combinear.qss");
    QString style; // Adaptic.qss, SyNet.qss
    if(file.open(QFile::ReadOnly)){
        style = file.readAll();
    }
    window.setStyleSheet(style);
    window.show();

    return app.exec();
}
