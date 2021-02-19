#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include "Qt-Frameless-Window-DarkStyle/DarkStyle.h"
#include "Qt-Frameless-Window-DarkStyle/framelesswindow/framelesswindow.h"
#include <QSettings>

int main(int argc, char *argv[]) {

    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);

    QApplication app(argc, argv);
    MainWindow window;
    // /home/adam/Desktop/sources/Evolution-IDE/stylesheets/Combinear.qss
    QFile file("/home/adam/Desktop/sources/Evolution-IDE/stylesheet.qss");
    QString darktheme;// Adaptic.qss, SyNet.qss
    if (file.open(QFile::ReadOnly)) {
        darktheme = file.readAll();
    }

    QString theme;

    // if it goes out of scope, will destroy !
    {
        QSettings settings("Evolution");
        theme = settings.value("Evolution/theme").toString();
    }
    /*
    theme = "Dark";
    if(theme == "Dark"){
        //app.setStyleSheet(darktheme);
        DarkStyle *style = new DarkStyle();
        app.setStyle(style);
    }
    */

    window.show();
    return app.exec();
}
