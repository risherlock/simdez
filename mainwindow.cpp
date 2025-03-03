#include "satellite.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQuickView>
#include <QWidget>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Register the satellite type with QML before loading the QML file
    qmlRegisterType<satellite>("Satellite", 1, 0, "Satellite");

    // QQuickView to load the QML file
    QQuickView *view = new QQuickView;
    view->setSource(QUrl::fromLocalFile("../../main.qml"));

    // A container widget for the QQuickView
    QWidget *container = QWidget::createWindowContainer(view, this);
    if (!container) {
        qWarning("Failed to create QQuickView container!");
        return;
    }

    // Set the container as the central widget of the main window
    setCentralWidget(container);
    container->setMinimumSize(640, 480);
    container->setFocusPolicy(Qt::StrongFocus);
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}
