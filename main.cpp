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

    // set application details.
    QApplication::setApplicationName(QString("Evolution-IDE"));
    QApplication::setApplicationVersion(QString("1.0.0"));
    //QApplication::setOrganizationName("");
    //QApplication::setOrganizationDomain("");

    // set application effects
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, true);
    QApplication::setEffectEnabled(Qt::UI_FadeMenu, true);
    QApplication::setEffectEnabled(Qt::UI_AnimateToolBox, true);
    QApplication::setEffectEnabled(Qt::UI_AnimateTooltip, true);
    QApplication::setEffectEnabled(Qt::UI_FadeTooltip, true);

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

    theme = "Dark";
    if(theme == "Dark") {
        app.setStyleSheet(darktheme);
        //DarkStyle *style = new DarkStyle();
        //app.setStyle(style);
    }


    window.show();
    return app.exec();
}
