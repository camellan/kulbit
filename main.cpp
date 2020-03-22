#include "mainwindow.h"

#include <QApplication>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator myTranslator;
    if (myTranslator.load(QLocale::system().name()))
    {
        qDebug() << "qtTranslator ok";
        a.installTranslator(&myTranslator);
    }

    QTranslator qtBaseTranslator;
    if (qtBaseTranslator.load("qtbase_" + QLocale::system().name()/*,
                QLibraryInfo::location(QLibraryInfo::TranslationsPath)*/))
    {
        qDebug() << "qtBaseTranslator ok";
        a.installTranslator(&qtBaseTranslator);
    }
    MainWindow w;
    w.show();
    return a.exec();
}
