#include "satellite.h"
#include "simulation.h"

#include "mainwindow.h"
#include "qcustomplot.h"
#include "ui_mainwindow.h"

#include <QUrl>
#include <QPen>
#include <QtConcurrent>
#include <QFile>
#include <QThread>
#include <QWidget>
#include <QJsonArray>
#include <QQuickView>
#include <QVBoxLayout>
#include <QJsonDocument>

extern QVector<double> t, bx, by, bz;
extern bool stop_flag;
QVector<double> q0, q1, q2, q3;

QStringList parameterKeys = {
                            "moi_xx",  "moi_xy",  "moi_xz",
                            "moi_yx",  "moi_yy",  "moi_yz",
                            "moi_zx",  "moi_zy",  "moi_zz",
                             "em_turns",
                             "em_core_length",
                             "em_core_area",
                             "em_max_current",
                            "orb_tle_1",  "orb_tle_2",  "orb_is_tle",
                            "orb_inclination",  "orb_eccentricity",  "orb_semi_major",  "orb_semi_minor",
                            "att_y0",  "att_p0",  "att_r0",  "att_w10",  "att_w20",  "att_w30",
                            "sim_stop_time",  "sim_step_time"
                            };

void MainWindow::gt_init(void)
{
    QPixmap img("../../assets/earth.png");
    ui->label_map->setPixmap(img);
    ui->label_map->setScaledContents(true);

    // Paints over the map
    gt_pixmap = ui->label_map->pixmap(Qt::ReturnByValue);
    gt_painter.begin(&gt_pixmap);

    // Groundtrack configuration
    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::red);
    gt_painter.setPen(pen);
    QBrush brush(Qt::red);
    gt_painter.setBrush(brush);
}

// Draw latitude [deg] and longitude [deg] as groundtrack
void MainWindow::gt_draw(const double la, const double lo)
{
    // Dimension of the image (not the label)
    int map_width = gt_pixmap.width();
    int map_height = gt_pixmap.height();

    // Corrected longitude/latitude to pixel conversion
    int x_axis = static_cast<int> (map_width * 0.5 - (map_width / 360.0f) * lo);
    int y_axis = static_cast<int> (map_height * 0.5 - (map_height / 180.0f) * la);

    // Draw the point at correct position
    int radius = 5;
    gt_painter.drawEllipse(QPoint(x_axis, y_axis), radius, radius);
    ui->label_map->setPixmap(gt_pixmap);
}

void mag_init_plot(QCustomPlot *p)
{
    // Graph colour and width
    QPen pen_x(QColor(0, 114, 189));
    QPen pen_y(QColor(217, 83, 25));
    QPen pen_z(QColor(237, 177, 32));
    pen_x.setWidth(2);
    pen_y.setWidth(2);
    pen_z.setWidth(2);

    // Labels on the magnetic field
    p->xAxis->setLabel("Time [s]");
    p->yAxis->setLabel("uT");
    p->xAxis->setLabelFont(QFont("Courier New", 12));
    p->yAxis->setLabelFont(QFont("Courier New", 12));
    p->xAxis->setLabelColor(Qt::blue);
    p->yAxis->setLabelColor(Qt::blue);
    p->addGraph(); // Graph 0 -> x-axis
    p->addGraph(); // Graph 1 -> y-axis
    p->addGraph(); // Graph 2 -> z-axis
    p->graph(0)->setName("x-axis");
    p->graph(1)->setName("y-axis");
    p->graph(2)->setName("z-axis");
    p->legend->setVisible(true);
    p->graph(0)->setPen(pen_x);
    p->graph(1)->setPen(pen_y);
    p->graph(2)->setPen(pen_z);
    p->replot();

    p->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void quat_init_plot(QCustomPlot *p)
{
    // Graph colour and width
    QPen pen_q0(QColor(0, 114, 189));
    QPen pen_q1(QColor(217, 83, 25));
    QPen pen_q2(QColor(237, 177, 32));
    QPen pen_q3(QColor(126, 47, 142));
    pen_q0.setWidth(2);
    pen_q1.setWidth(2);
    pen_q2.setWidth(2);
    pen_q3.setWidth(2);

    // Labels on the magnetic field
    p->xAxis->setLabel("Time [s]");
    p->xAxis->setLabelFont(QFont("Courier New", 12));
    p->yAxis->setLabelFont(QFont("Courier New", 12));
    p->xAxis->setLabelColor(Qt::blue);
    p->yAxis->setLabelColor(Qt::blue);
    p->addGraph(); // Graph 0 -> q0
    p->addGraph(); // Graph 1 -> q1
    p->addGraph(); // Graph 2 -> q2
    p->addGraph(); // Graph 2 -> q3
    p->graph(0)->setName("q0");
    p->graph(1)->setName("q1");
    p->graph(2)->setName("q3");
    p->graph(3)->setName("q4");
    p->legend->setVisible(true);
    p->graph(0)->setPen(pen_q0);
    p->graph(1)->setPen(pen_q1);
    p->graph(1)->setPen(pen_q2);
    p->graph(2)->setPen(pen_q3);
    p->replot();
}

void quat_plot(QCustomPlot *p)
{
    p->rescaleAxes();
    p->replot();
}

void quat_add_data(QCustomPlot *p, double time, double q[0])
{
    t.append(time);
    q0.append(q[0]);
    q1.append(q[1]);
    q2.append(q[2]);
    q3.append(q[3]);
    //p->graph(0)->setData(time, q0);
    //p->graph(1)->setData(time, q1);
    //p->graph(2)->setDafalseta(time, q2);
    //p->graph(2)->setData(time, q3);
    //quat_plot(p);
}

void MainWindow::append_simdata(double t, double lla[3], double b[3], double q[4])
{
    static bool is_first = true;

    if(is_first)
    {
        is_first = false;
        timer_plot_mag->start(33);
    }

    gt_draw(lla[0], lla[1]);
}

void MainWindow::onParametersChanged()
{
    parameters["moi_xx"] = ui->lineEdit_moi_xx->text().toDouble();
    parameters["moi_xy"] = ui->lineEdit_moi_xy->text().toDouble();
    parameters["moi_xz"] = ui->lineEdit_moi_xz->text().toDouble();
    parameters["moi_yx"] = ui->lineEdit_moi_yx->text().toDouble();
    parameters["moi_yy"] = ui->lineEdit_moi_yy->text().toDouble();
    parameters["moi_yz"] = ui->lineEdit_moi_yz->text().toDouble();
    parameters["moi_zx"] = ui->lineEdit_moi_zx->text().toDouble();
    parameters["moi_zy"] = ui->lineEdit_moi_zy->text().toDouble();
    parameters["moi_zz"] = ui->lineEdit_moi_zz->text().toDouble();

    parameters["em_turns"] = ui->lineEdit_em_turns->text().toDouble();
    parameters["em_core_length"] = ui->lineEdit_em_core_length->text().toDouble();
    parameters["em_core_area"] = ui->lineEdit_em_core_area->text().toDouble();
    parameters["em_max_current"] = ui->lineEdit_em_max_current->text().toDouble();

    parameters["att_y0"] = ui->lineEdit_att_y0->text().toDouble();
    parameters["att_p0"] = ui->lineEdit_att_p0->text().toDouble();
    parameters["att_r0"] = ui->lineEdit_att_r0->text().toDouble();
    parameters["att_w10"] = ui->lineEdit_att_w10->text().toDouble();
    parameters["att_w20"] = ui->lineEdit_att_w20->text().toDouble();
    parameters["att_w30"] = ui->lineEdit_att_w30->text().toDouble();

    parameters["orb_tle_1"] = ui->textEdit_orb_tle_1->toPlainText();
    parameters["orb_tle_2"] = ui->textEdit_orb_tle_1->toPlainText();

    parameters["sim_stop_time"] = ui->lineEdit_sim_stop_time->text().toDouble();
    parameters["sim_step_time"] = ui->lineEdit_sim_step_time->text().toDouble();
}

void MainWindow::initializeParameters()
{

    QFile json_file("../../default.json");
    json_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray json_data = json_file.readAll();
    json_file.close();
    QJsonDocument json_doc = QJsonDocument::fromJson(json_data);
    json_obj = json_doc.object();
    bool isCorrectJSON = true;

    for (const QString &key : parameterKeys) {
        if (!json_obj.contains(key)) {
            qDebug() << "Json file error!";
            qDebug() << key;
            isCorrectJSON=false;
        }
    }

    parameters = json_obj.toVariantMap();
    ui->lineEdit_moi_xx->setText(QString::number(parameters["moi_xx"].toDouble()));
    ui->lineEdit_moi_xy->setText(QString::number(parameters["moi_xy"].toDouble()));
    ui->lineEdit_moi_xz->setText(QString::number(parameters["moi_xz"].toDouble()));
    ui->lineEdit_moi_yx->setText(QString::number(parameters["moi_yx"].toDouble()));
    ui->lineEdit_moi_yy->setText(QString::number(parameters["moi_yy"].toDouble()));
    ui->lineEdit_moi_yz->setText(QString::number(parameters["moi_yz"].toDouble()));
    ui->lineEdit_moi_zx->setText(QString::number(parameters["moi_zx"].toDouble()));
    ui->lineEdit_moi_zy->setText(QString::number(parameters["moi_zy"].toDouble()));
    ui->lineEdit_moi_zz->setText(QString::number(parameters["moi_zz"].toDouble()));

    ui->lineEdit_em_turns->setText(QString::number(parameters["em_turns"].toDouble()));
    ui->lineEdit_em_core_length->setText(QString::number(parameters["em_core_length"].toDouble()));
    ui->lineEdit_em_core_area->setText(QString::number(parameters["em_core_area"].toDouble()));
    ui->lineEdit_em_max_current->setText(QString::number(parameters["em_max_current"].toDouble()));

    ui->lineEdit_att_y0->setText(QString::number(parameters["att_y0"].toDouble()));
    ui->lineEdit_att_p0->setText(QString::number(parameters["att_p0"].toDouble()));
    ui->lineEdit_att_r0->setText(QString::number(parameters["att_r0"].toDouble()));
    ui->lineEdit_att_w10->setText(QString::number(parameters["att_w10"].toDouble()));
    ui->lineEdit_att_w20->setText(QString::number(parameters["att_w20"].toDouble()));
    ui->lineEdit_att_w30->setText(QString::number(parameters["att_w30"].toDouble()));

    ui->textEdit_orb_tle_1->setPlainText(parameters["orb_tle_1"].toString());
    ui->textEdit_orb_tle_2->setPlainText(parameters["orb_tle_2"].toString());

    ui->lineEdit_sim_stop_time->setText(QString::number(parameters["sim_stop_time"].toDouble()));
    ui->lineEdit_sim_step_time->setText(QString::number(parameters["sim_step_time"].toDouble()));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
        qDebug()<<"connecting to parameter changed";

    ui->setupUi(this);
    qDebug()<<"initializing parameters";
    initializeParameters();

    // Register the class for QML to access variables
    qmlRegisterType<satellite>("Satellite", 1, 0, "Satellite");

    // Load the QML file to QQuickView
    QQuickView *view = new QQuickView;
    view->setSource(QUrl::fromLocalFile("../../main.qml"));

    // A container widget to link QQuickView to QWidget
    QWidget *container = QWidget::createWindowContainer(view, this);
    QVBoxLayout *layout = new QVBoxLayout(ui->quickContainer);
    layout->setSpacing(0);
    ui->quickContainer->setLayout(layout);
    ui->quickContainer->layout()->addWidget(container);

    gt_init();
    mag_init_plot(ui->widget_plot_mag);

    timer_plot_mag = new QTimer(this);
    connect(timer_plot_mag, &QTimer::timeout, this, [this]()
            {
                ui->widget_plot_mag->graph(0)->setData(t, bx);
                ui->widget_plot_mag->graph(1)->setData(t, by);
                ui->widget_plot_mag->graph(2)->setData(t, bz);
                ui->widget_plot_mag->rescaleAxes();
                ui->widget_plot_mag->replot();
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_save_clicked()
{
    qDebug() << "save clicked";
    onParametersChanged();
    QJsonDocument json_doc = QJsonDocument::fromVariant(parameters);
    QFile json_file("../../default.json");
    json_file.open(QIODevice::WriteOnly | QIODevice::Text);
    json_file.write(json_doc.toJson());
    json_file.close();
}


void MainWindow::on_pushButton_reload_clicked()
{
    qDebug() << "parameters reinitialized";
    initializeParameters();
    stop_flag = true;
}


void MainWindow::on_pushButton_start_simulation_clicked()
{
    qDebug() << "start simulation clicked";
    onParametersChanged();
    emit parameters_updated(parameters);

    t.clear();
    bx.clear();
    by.clear();
    bz.clear();
    stop_flag = false;
}

void MainWindow::on_pushButton_set_simulation_param_clicked()
{
    stop_flag= true;
    onParametersChanged();
}
