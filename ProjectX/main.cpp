#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include "formprojectx.h"
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FormProjectX *app = new FormProjectX();
    app->show();
    return a.exec();
}
