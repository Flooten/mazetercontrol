#include "terminal.h"
#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Lägg till de font som används.
    QFontDatabase::addApplicationFont(":/fonts/resources/consolas.ttf");

    MC::MainWindow w;
    w.show();
    
    return a.exec();
}
