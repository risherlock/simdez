#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../../../../qcustomplot.h"

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

private:
    Ui::MainWindow *ui;
    QJsonObject json_obj;
    QMap<QString, QVariant> parameters;

    bool validateJSONKeys();

private slots:
    void onParametersChanged();
    void on_save_clicked();
    void on_reload_clicked();
};

#endif // MAINWINDOW_H
