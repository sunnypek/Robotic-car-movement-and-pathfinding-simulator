#include "simulation.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Simulation w;
    w.show();
    return a.exec();
}
