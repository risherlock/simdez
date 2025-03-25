#include "satellite.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QVBoxLayout>
#include <QQuickView>
#include <QWidget>
#include <QUrl>
#include <QPen>

void MainWindow::gt_init(void)
{
    QPixmap img("../../../assets/earth.png");
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Register the class for QML to access variables
    qmlRegisterType<satellite>("Satellite", 1, 0, "Satellite");

    // Load the QML file to QQuickView
    QQuickView *view = new QQuickView;
    view->setSource(QUrl::fromLocalFile("../../../main.qml"));

    // A container widget to link QQuickView to QWidget
    QWidget *container = QWidget::createWindowContainer(view, this);
    QVBoxLayout *layout = new QVBoxLayout(ui->quickContainer);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    ui->quickContainer->setLayout(layout);
    ui->quickContainer->layout()->addWidget(container);

    QFile json_file("../../../default.json");
    json_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray json_data = json_file.readAll();
    json_file.close();
    QJsonDocument json_doc = QJsonDocument::fromJson(json_data);
    json_obj = json_doc.object();

    if (json_obj.contains("moi_xx"))
    {
        double moi_xx = json_obj["moi_xx"].toDouble();
        qDebug() << "Original moi_xx:" << moi_xx;
    }
    else
    {
        qDebug() << "Json file error!";
    }

    ui->lineEdit_moi_xx->setText(QString::number(json_obj["moi_xx"].toDouble()));
    ui->lineEdit_moi_xy->setText(QString::number(json_obj["moi_xy"].toDouble()));
    ui->lineEdit_moi_xz->setText(QString::number(json_obj["moi_xz"].toDouble()));
    ui->lineEdit_moi_yx->setText(QString::number(json_obj["moi_yx"].toDouble()));
    ui->lineEdit_moi_yy->setText(QString::number(json_obj["moi_yy"].toDouble()));
    ui->lineEdit_moi_yz->setText(QString::number(json_obj["moi_yz"].toDouble()));
    ui->lineEdit_moi_zx->setText(QString::number(json_obj["moi_zx"].toDouble()));
    ui->lineEdit_moi_zy->setText(QString::number(json_obj["moi_zy"].toDouble()));
    ui->lineEdit_moi_zz->setText(QString::number(json_obj["moi_zz"].toDouble()));

    ui->lineEdit_em_turns->setText(QString::number(json_obj["em_turns"].toDouble()));
    ui->lineEdit_em_core_length->setText(QString::number(json_obj["em_core_length"].toDouble()));
    ui->lineEdit_em_core_area->setText(QString::number(json_obj["em_core_area"].toDouble()));
    ui->lineEdit_em_max_current->setText(QString::number(json_obj["em_max_current"].toDouble()));

    ui->lineEdit_att_y0->setText(QString::number(json_obj["att_y0"].toDouble()));
    ui->lineEdit_att_p0->setText(QString::number(json_obj["att_p0"].toDouble()));
    ui->lineEdit_att_r0->setText(QString::number(json_obj["att_r0"].toDouble()));
    ui->lineEdit_att_w10->setText(QString::number(json_obj["att_w10"].toDouble()));
    ui->lineEdit_att_w20->setText(QString::number(json_obj["att_w20"].toDouble()));
    ui->lineEdit_att_w30->setText(QString::number(json_obj["att_w30"].toDouble()));

    ui->lineEdit_sim_stop_time->setText(QString::number(json_obj["sim_stop_time"].toDouble()));
    ui->lineEdit_sim_step_time->setText(QString::number(json_obj["sim_step_time"].toDouble()));

    float latitude = -22.5752;
    float longitude = -144.0848;

    gt_init();
    gt_draw(latitude, longitude);
    gt_draw(0.0, 0.0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
