#ifndef NEWCAMERAVIEW_H
#define NEWCAMERAVIEW_H

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>
#include "opencv2/opencv.hpp"
#include "ui_newcameraview.h"

using namespace cv;

namespace Ui {
class newCameraView;
}

class newCameraView : public QWidget
{
    Q_OBJECT

public:
    explicit newCameraView(QWidget *parent = nullptr);
    ~newCameraView();
    Ui::newCameraView *ui;
    VideoCapture video;
    Mat frame;
    QGraphicsPixmapItem pixmap;
};

#endif // NEWCAMERAVIEW_H
