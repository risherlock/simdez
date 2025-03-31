#include "satellite.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qcustomplot.h"
#include <iostream>
#include "dcm.h"
#include "SGP4.h"
#include "TLE.h"
#include "igrf.h"
#include "time.h"
#include "frame.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QVBoxLayout>
#include <QQuickView>
#include <QWidget>
#include <QUrl>
#include <QPen>
#include <QtConcurrent>


TLE tle;
QVector<double> x, bx, by, bz, q0, q1, q2, q3, lat, lon;
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
    // gt_painter.drawPixmap(&gt_pixmap);

    // Groundtrack configuration
    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::red);
    gt_painter.setPen(pen);
    QBrush brush(Qt::red);
    gt_painter.setBrush(brush);

    double dcm[3][3];
    // dcm_x(22.4, dcm);
    dcm_unit(dcm);
    qDebug() << dcm[0][0];
}

// Draw latitude [deg] and longitude [deg] as groundtrack
void MainWindow::gt_draw(const double la, const double lo)
{
    // Dimension of the image (not the label)
    int map_width = gt_pixmap.width();
    int map_height = gt_pixmap.height();

    // Corrected longitude/latitude to pixel conversion
    int x = static_cast<int> (map_width * 0.5 - (map_width / 360.0f) * lo);
    int y = static_cast<int> (map_height * 0.5 - (map_height / 180.0f) * la);

    // Draw the point at correct position
    int radius = 5;
    gt_painter.drawEllipse(QPoint(x, y), radius, radius);
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

void quat_add_data(QCustomPlot *p, double t, double q[0])
{
    x.append(t);
    q0.append(q[0]);
    q1.append(q[1]);
    q2.append(q[2]);
    q3.append(q[3]);
    p->graph(0)->setData(x, q0);
    p->graph(1)->setData(x, q1);
    p->graph(2)->setData(x, q2);
    p->graph(2)->setData(x, q3);

    quat_plot(p);
}

void mag_plot(QCustomPlot *p)
{
    p->rescaleAxes();
    p->replot();
}

void mag_add_data(QCustomPlot *p, double t, double b[3])
{
    x.append(t);
    bx.append(b[0]);
    by.append(b[1]);
    bz.append(b[2]);
    p->graph(0)->setData(x, bx);
    p->graph(1)->setData(x, by);
    p->graph(2)->setData(x, bz);

    mag_plot(p);
}


void MainWindow::simulator()
{
    char line1[70];
    char line2[70];
    double stepmin = .01;
    double startmin = 0;
    double stopmin = 200;
    strncpy(line1, parameters["orb_tle_1"].toString().toUtf8().constData(), 69); line1[69] = '\0';
    strncpy(line2, parameters["orb_tle_2"].toString().toUtf8().constData(), 69); line2[69] = '\0';
    tle.parseLines(line1, line2);


    int steps = (stopmin - startmin) / stepmin;
    double dcm[3][3], r_ecef[3], r[3], v[3], lla[3], b[3];

    for (int i = 0; i < steps; i++)
    {
        double t = startmin + i * stepmin;

        // Geodetic coordinates of satellite
        tle.getRV(t, r, v);
        frame_eci_to_ecef_dcm(tle.dt, dcm);
        dcm_rotate(dcm, r, r_ecef);
        frame_ecef_to_lla(r_ecef, lla);

        // Compute magnetic field
        // Compute magnetic field
        if (!igrf(tle.dt, lla, IGRF_GEODETIC, b))
        {
            qDebug() << "IGRF date error!" << (int)tle.dt.year;
        }
        else
        {
            qDebug() << t << b[0] << b[1] << b[2];
            mag_add_data(ui->widget_plot_mag, (double)i, b);
        }

        qDebug() << r[0] << "," << r[1] << "," << r[2];

        gt_draw(lla[0], lla[1]);
    }
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
    try{
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
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    ui->quickContainer->setLayout(layout);
    ui->quickContainer->layout()->addWidget(container);

    gt_init();
    mag_init_plot(ui->widget_plot_mag);

    qDebug() << "Done!!";

    // simulator is run in parallel
    // to update the data in realtime using simulator, run the update function for the visualization within the simulator
    QtConcurrent::run([this](){
        simulator();
    });

    } catch(QException e){
        qDebug() << e.what();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_save_clicked()
{
    qDebug() << "save clicked";
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
}


void MainWindow::on_pushButton_start_simulation_clicked()
{
    onParametersChanged();
}


void MainWindow::on_pushButton_set_simulation_param_clicked()
{
    onParametersChanged();
}

