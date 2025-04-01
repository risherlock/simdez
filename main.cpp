#include "mainwindow.h"
#include <QApplication>

#include "simulation.h"
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QMap<QString, QVariant> parameters = w.getParameters();

    QThread *simulation_thread = new QThread();
    simulation *simulation_worker = new simulation(nullptr, parameters);
    simulation_worker->moveToThread(simulation_thread);
    QObject::connect(simulation_thread, &QThread::started, simulation_worker, &simulation::run);
    QObject::connect(simulation_worker, &simulation::data_generated, &w, &MainWindow::append_simdata);

    QObject::connect(&w, &MainWindow::parameters_updated, simulation_worker, &simulation::updateParameters);


    simulation_thread->start();

    return a.exec();
}
