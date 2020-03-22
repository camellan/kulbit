#include "zoneitemwidget.h"

zoneItemWidget::zoneItemWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::zoneItemWidget)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene(this));

    this->setStyleSheet("QFrame { border: 1px solid gray;"
                        "border-radius: 2px;"
                        "margin-top: 1ex; }");
}

zoneItemWidget::~zoneItemWidget()
{
    delete ui;
}

