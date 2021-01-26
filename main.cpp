#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include "Qt-Frameless-Window-DarkStyle/DarkStyle.h"
#include "Qt-Frameless-Window-DarkStyle/framelesswindow/framelesswindow.h"
#include <QSettings>

int main(int argc, char *argv[]) {

    QSettings settings("Evolution");
    bool def = settings.value("Evolution/SetDefaultSettings").toBool();
    if(!def){
        settings.setValue("Evolution/SetDefaultSettings", true);
    }

    QApplication app(argc, argv);
    MainWindow window;
    QFile file("/home/adam/Desktop/sources/Evolution-IDE/stylesheets/Combinear.qss");
    QString darktheme; // Adaptic.qss, SyNet.qss
    if(file.open(QFile::ReadOnly)){
        darktheme = file.readAll();
    }


    QString theme = settings.value("Evolution/theme").toString();
    /*
    theme = "Dark";
    if(theme == "Dark"){
        // app.setStyleSheet(darktheme);
        DarkStyle *style = new DarkStyle();
        app.setStyle(style);
    }
    */

    window.show();
    return app.exec();
}
