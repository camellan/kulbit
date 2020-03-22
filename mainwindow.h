#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCameraInfo>
#include <QTranslator>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QCloseEvent>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>
#include <QSettings>
#include <QSlider>
#include <QLabel>
#include <QHBoxLayout>
#include <QThread>
#include "opencv2/opencv.hpp"
#include <newcameraview.h>

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QList<QCameraInfo> cameras;
    QStringList groups;
    QStringList out;
    QMessageBox msgBox;
    QTranslator myTranslator;
    VideoCapture video;
    Mat frame, roi, gray;
    Rect2d zone;
    Rect rect;
    int zone_x;
    int zone_y;
    int zone_w;
    int zone_h;
    int a, b, c, d;
    QGraphicsPixmapItem pixmap;
    QCheckBox * camera_checkbox;
    QHBoxLayout * hbox;
    newCameraView *camView;
    bool start_detect = false;

private slots:
    void setAppUI();
    void saveSettings();
    void readVideoDevices();
    void updateZoneList();
    void updateModelList();
    void add_newCameraView();
    void findCircles(int, int, int, int);
    void startDetect();
    void startVideoView();

    void on_set_modeBtn_pressed();
    void on_wrk_modeBtn_pressed();
    void on_open_zoneBtn_pressed();
    void on_save_zoneBtn_pressed();
    void on_horizontalSlider_1_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_3_valueChanged(int value);
    void on_horizontalSlider_4_valueChanged(int value);
    void on_addZoneBtn_pressed();
    void on_deleteZoneBtn_pressed();
    void on_detectBtn_pressed();
    void on_close_zoneBtn_pressed();
    void on_helpBtn_pressed();
    void on_addModelBtn_pressed();
    void on_deleteModelBtn_pressed();
    void on_saveModelBtn_pressed();
    void on_canModelBtn_clicked();
};
#endif // MAINWINDOW_H
