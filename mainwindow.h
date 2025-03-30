#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcustomplot.h"

#include <QMainWindow>
#include <QJsonObject>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Groundtrack
    QPixmap gt_pixmap;
    QPainter gt_painter;
    void gt_init(void);
    void gt_draw(const double la, const double lo);

public slots:
    void visualize(double t, double lla[3], double b[3], double q[4]);

private:
    Ui::MainWindow *ui;
    QJsonObject json_obj;
};

#endif // MAINWINDOW_H
