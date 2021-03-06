#include "terminal.h"
#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Lägg till de font som används.
    QFontDatabase::addApplicationFont(":/fonts/resources/consolas.ttf");
    QFontDatabase::addApplicationFont(":/fonts/resources/opensans.ttf");
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    MC::MainWindow w;
    w.show();
    
    return a.exec();
}
