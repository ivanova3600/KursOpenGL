#include "mainwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.setWindowTitle("NURBS-поверхность вращения");
    w.show();
    return a.exec();
}
