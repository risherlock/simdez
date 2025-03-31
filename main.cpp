#include "mainwindow.h"
#include <QApplication>

#include "simulation.h"
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    simulation *simulation_thread = new simulation();
    QObject::connect(simulation_thread, &simulation::data_generated, &w, &MainWindow::visualize);
    simulation_thread->start();
    return a.exec();
}
