#include "satellite.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QQuickView>
#include <QWidget>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    QPixmap img("../../assets/earth.png");
    ui->label_map->setPixmap(img);
    ui->label_map->setScaledContents(true);

    resize(1280, 720);
}

MainWindow::~MainWindow()
{
    delete ui;
}
