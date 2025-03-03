#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWidget>
#include <QQuickView>
#include <QQuickItem>
#include <QQmlContext>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QQuickView *view = new QQuickView;
    view->setSource(QUrl::fromLocalFile("../../main.qml"));

    QWidget *container = QWidget::createWindowContainer(view, this);

    if (!container)
    {
        qWarning("Failed to create QQuickView container!");
        return;
    }

    setCentralWidget(container);
    container->setMinimumSize(640, 480);
    container->setFocusPolicy(Qt::StrongFocus);
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}
