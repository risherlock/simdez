#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "satellite.h"
#include "simulation.h"

#include <QMainWindow>
#include <QJsonObject>
#include <QPainter>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Groundtrack
    QPixmap gt_pixmap;
    QPainter gt_painter;
    void gt_init(void);
    void gt_draw(const double la, const double lo);
    void simulator();
    void initializeParameters();
    void addConnections();

    QMap<QString, QVariant> getParameters() const;

signals:
    void parameters_updated(QMap<QString, QVariant> parameters);

public slots:
    void append_simdata(double t, double lla[3], double b[3], double q[4]);

private:
    satellite *sat_cad;
    QTimer *timer_plot_mag;
    Ui::MainWindow *ui;
    QJsonObject json_obj;
    QMap<QString, QVariant> parameters;
    QThread *simulation_thread = new QThread();
    simulation *simulation_worker = new simulation();

    bool validateJSONKeys();
    void startSimulation();

private slots:
    void onParametersChanged();
    void on_pushButton_save_clicked();
    void on_pushButton_reload_clicked();
    void on_pushButton_start_simulation_clicked();
    void on_pushButton_set_simulation_param_clicked();
    void on_pushButton_stop_simulation_clicked();
};

#endif // MAINWINDOW_H
