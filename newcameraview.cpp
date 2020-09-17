#include "newcameraview.h"



newCameraView::newCameraView(QWidget *parent):
    QWidget(parent),
    ui(new Ui::newCameraView)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pixmap);
}

newCameraView::~newCameraView()
{
    qDebug()<< "destroy";
    video.release();
    delete ui;
}

