#include "mainwindow.h"
#include <QApplication>

#include "simulation.h"
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QThread *simulation_thread = new QThread();
    simulation *simulation_worker = new simulation();
    simulation_worker->moveToThread(simulation_thread);
    QObject::connect(simulation_thread, &QThread::started, simulation_worker, &simulation::run);
    QObject::connect(simulation_worker, &simulation::data_generated, &w, &MainWindow::append_simdata);
    simulation_thread->start();

    return a.exec();
}
