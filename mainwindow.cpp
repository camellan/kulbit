#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "zoneitemwidget.h"
#include "ui_zoneitemwidget.h"
#include "newcameraview.h"
#include "ui_newcameraview.h"
#include <QCameraInfo>
#include <QListWidget>
#include <QCheckBox>
#include <QMessageBox>
#include <QWidget>
#include <QCollator>
#include <QDir>

QSettings settings("config/zones.conf", QSettings::IniFormat);
QSettings modelsettings("config/models.conf", QSettings::IniFormat);
Q_DECLARE_METATYPE(QList<int>)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    setAppUI();
    updateZoneList();
    updateModelList();
    readVideoDevices();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setAppUI()
{
    ui->setupUi(this);
    hbox = new QHBoxLayout();
    hbox->addStretch(1);
    auto hbox_label = new QLabel(tr("Available cameras: "));
    hbox->addWidget(hbox_label);
    ui->checkboxFrame->setLayout(hbox);

    ui->zone_Numb->setValidator( new QIntValidator(0, 100, this) );
    //    ui->gridLayout_4->addWidget(ui->roiWidget, 0, 1);
    //    ui->gridLayout_4->addWidget(ui->sliderFrame, 2, 0);
    //    ui->gridLayout_4->addWidget(ui->add_modelFrame, 2, 1);

//    ui->modelFrame->hide();
    ui->sliderFrame->hide();
    ui->add_modelFrame->hide();
    ui->roiWidget->hide();
    ui->buttonBox_2->hide();
    ui->buttonBox->hide();

    ui->wrk_modeBtn->setEnabled(false);

    ui->horizontalSlider_1->setRange(1,250);
    ui->spinBox_1->setRange(1,250);

    ui->horizontalSlider_2->setRange(1,250);
    ui->spinBox_2->setRange(1,250);

    ui->horizontalSlider_3->setRange(0,50);
    ui->spinBox_3->setRange(1,50);

    ui->horizontalSlider_4->setRange(0,50);
    ui->spinBox_4->setRange(1,50);

}

void MainWindow::readVideoDevices()
{
    cameras = QCameraInfo::availableCameras();
    for( int i = 0; i < cameras.count(); i++)
    {
        ui->camera_comboBox->addItem(QString::number(i));
        auto camera_checkbox = new QCheckBox(QString::number(i),ui->checkboxFrame);
        connect(camera_checkbox, SIGNAL(stateChanged(int)),this, SLOT(add_newCameraView()));
        hbox->addWidget(camera_checkbox);
    }
}

void MainWindow::add_newCameraView()
{
    auto *camera_checkbox = qobject_cast<QCheckBox*>(sender());
    auto camera_index = camera_checkbox->text().toInt();

    if (camera_checkbox->isChecked())
    {
        camView = new newCameraView(this);
        camView->setObjectName(QStringLiteral("camView%1").arg(camera_index));
        camView->ui->label->setText(tr("Camera %1").arg(QString::number(camera_index)));
        ui->gridLayout_4->addWidget(camView);
        camView->video.open(camera_index);
        startVideoView();
    }
    else
    {
        auto *camView = this->findChild<newCameraView*>(QStringLiteral("camView%1").arg(camera_checkbox->text()));
        qDebug()<<"Start delete camView";
        ui->gridLayout_4->removeWidget(camView);
        delete camView;
        qDebug()<<"End delete camView";
    }
}

void MainWindow::startVideoView()
{
    auto *camView_0 = this->findChild<newCameraView*>("camView0");
    auto *camView_1 = this->findChild<newCameraView*>("camView1");
    auto *camView_2 = this->findChild<newCameraView*>("camView2");

    while(this->isVisible())
    {
        if (start_detect)
        {
            startDetect();
            msgBox.setWindowTitle(tr("Result"));
            msgBox.setText(tr("Discrepancies found:"));
            QString outstr;
            if(!out.empty())
            {
                foreach (QString u, out)
                {
                    outstr += u + "\n";
                }
//                qDebug()<<std::accumulate(out.cbegin(), out.cend(), outstr);
            }
            if (out.empty()) outstr.append(tr("none"));

            msgBox.setInformativeText(outstr);
            msgBox.show();
            out.clear();
            outstr.clear();
            start_detect = false;
            ui->detectBtn->setEnabled(true);
        }

        if (camView_0)
        {
            if(camView_0->video.isOpened())
            {
                camView_0->video>>camView_0->frame;
                if(!camView_0->frame.empty())
                {
                    QImage qimg(camView_0->frame.data,
                                camView_0->frame.cols,
                                camView_0->frame.rows,
                                camView_0->frame.step,
                                QImage::Format_RGB888);
                    camView_0->pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()));
                    camView_0->ui->graphicsView->fitInView(&camView_0->pixmap, Qt::KeepAspectRatio);
                }
            }
        }

        if (camView_1)
        {
            if(camView_1->video.isOpened())
            {
                //                qDebug()<<"Start camView1->frame";
                camView_1->video>>camView_1->frame;
                //                qDebug()<<"End camView1->frame";
                if(!camView_1->frame.empty())
                {
                    QImage qimg(camView_1->frame.data,
                                camView_1->frame.cols,
                                camView_1->frame.rows,
                                camView_1->frame.step,
                                QImage::Format_RGB888);
                    camView_1->pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()));
                    camView_1->ui->graphicsView->fitInView(&camView_1->pixmap, Qt::KeepAspectRatio);
                }
            }
        }

        if (camView_2)
        {
            if(camView_2->video.isOpened())
            {
                //                qDebug()<<"Start camView2->frame";
                camView_2->video>>camView_2->frame;
                //                qDebug()<<"End camView2->frame";
                if(!camView_2->frame.empty())
                {
                    QImage qimg(camView_2->frame.data,
                                camView_2->frame.cols,
                                camView_2->frame.rows,
                                camView_2->frame.step,
                                QImage::Format_RGB888);
                    camView_2->pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()));
                    camView_2->ui->graphicsView->fitInView(&camView_2->pixmap, Qt::KeepAspectRatio);
                }
            }
        }
        qApp->processEvents();
    }
}

void MainWindow::on_set_modeBtn_pressed()
{
//    ui->modelFrame->show();
    ui->buttonBox_2->show();
    ui->buttonBox->show();
    ui->roiWidget->show();
    ui->set_modeBtn->setEnabled(false);
    ui->wrk_modeBtn->setEnabled(true);
}

void MainWindow::on_wrk_modeBtn_pressed()
{
    ui->set_modeBtn->setEnabled(true);
    ui->wrk_modeBtn->setEnabled(false);
//    ui->modelFrame->hide();
    ui->add_modelFrame->hide();
    ui->buttonBox_2->hide();
    ui->buttonBox->hide();
    ui->sliderFrame->hide();
    ui->roiWidget->hide();
}

void MainWindow::on_save_zoneBtn_pressed()
{
//    qDebug() << "Save button pressed";
    saveSettings();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    start_detect = false;
    video.release();
    frame.release();
    roi.release();
    destroyAllWindows();
    qDebug()<< "Close event";
    event->accept();
    qApp->quit();
}

void MainWindow::on_horizontalSlider_1_valueChanged(int value)
{
    a = value;
}

void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    b = value;
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    c = value;
}

void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    d = value;
}

void MainWindow::on_open_zoneBtn_pressed()
{
    video.open(ui->camera_comboBox->currentIndex());

    if(!video.isOpened())
    {
        QMessageBox::critical(this,
                              tr("Camera Error"),
                              tr("Make sure you entered a correct camera index,"
                                 "<br>or that the camera is not being accessed by another program!"));
        return;
    }

    video >> frame;
    if(!frame.empty())
    {
        namedWindow("Select detection zone", 0);
        zone = selectROI("Select detection zone", frame, false, false);
        if(!zone.empty())
        {
            roi = frame(zone);
            ui->zone_X->setText(QString::number(zone.x));
            ui->zone_Y->setText(QString::number(zone.y));
            ui->zone_Width->setText(QString::number(zone.width));
            ui->zone_Height->setText(QString::number(zone.height));
            destroyAllWindows();
        }
    }
    findCircles(a, b, c, d);
}

void MainWindow::findCircles(int , int, int , int)
{
    while(video.isOpened())
    {
        video >> frame;
        if(!frame.empty())
        {
            if (!roi.empty())
            {
                Mat roi2;
                roi.copyTo(roi2);
                cvtColor(roi2, gray, COLOR_BGR2GRAY);
                medianBlur(gray, gray, 5);
                std::vector<Vec3f> circles;
                HoughCircles(gray, circles, HOUGH_GRADIENT, 2, gray.rows/8, a, b, c, d );

                qDebug() << "Detected: " << circles.size() << " circles" <<endl;

                for( size_t i = 0; i < circles.size(); i++ )
                {
                    Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                    int radius = cvRound(circles[i][2]);
                    circle( roi2, center, 3, Scalar(0,255,0), -1, 8, 0 );
                    circle( roi2, center, radius, Scalar(0,0,255), 1, 8, 0 );
                }
                QImage qimg(roi2.data,
                            roi2.cols,
                            roi2.rows,
                            roi2.step,
                            QImage::Format_RGB888);
                pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()));
                ui->roiView->setScene(new QGraphicsScene(this));
                ui->roiView->scene()->addItem(&pixmap);
                ui->roiView->fitInView(&pixmap, Qt::KeepAspectRatio);
            }
        }
        qApp->processEvents();
    }
}

void MainWindow::on_addZoneBtn_pressed()
{
    ui->sliderFrame->show();
    //    ui->roiWidget->show();
    qDebug()<<"ui->sliderFrame->show();";
}

void MainWindow::on_deleteZoneBtn_pressed()
{   
    QList<QListWidgetItem *> items = ui->zoneView->selectedItems();
    if(items.count() > 0)
    {
        int n = QMessageBox::critical(this, tr("Delete zone?"),
                                      tr("The selected zone will be deleted!\n"
                                         "Do you really want to do this?"),
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (n == QMessageBox::Yes)
        {
            QString str((qobject_cast<zoneItemWidget*>
                         (ui->zoneView->itemWidget(ui->zoneView->item(ui->zoneView->currentRow()))))->getZoneNumber());
            settings.beginGroup(str);
            settings.remove("");
            settings.endGroup();
            settings.sync();
            updateZoneList();
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Delete zone"), tr("Zone not selected!\nPlease select a zone to delete."));
        ui->addZoneBtn->setFocus();
        return;
    }
}

void MainWindow::on_close_zoneBtn_pressed()
{
    if(roi.data!=0)
    {
        ui->roiView->scene()->removeItem(&pixmap);
        qDebug()<<"ui->roiView->scene()->removeItem(&pixmap)";
        ui->roiView->scene()->clear();
        qDebug()<<"ui->roiView->scene()->clear()";
        ui->roiView->scene()->update();
        qDebug()<<"ui->roiView->scene()->update()";
        video.release();
        qDebug()<<"Video release";
        roi.release();
        qDebug()<<"Roi release";
        roi.empty();
        qDebug()<<"Roi empty";
        frame.release();
    }
    ui->sliderFrame->hide();
    //    ui->roiWidget->hide();
    destroyAllWindows();
    qDebug()<<"Destroy";
}

void MainWindow::on_detectBtn_pressed()
{
    start_detect = true;
    ui->detectBtn->setEnabled(false);
    startDetect();
    startVideoView();
}

void MainWindow::startDetect()
{
    out.clear();
    groups.clear();
    groups = settings.childGroups();
    QCollator collator;
    collator.setNumericMode(true);
    std::sort(groups.begin(), groups.end(), collator);
//    qDebug() << groups;
    QString str;
    QList<int> resultlist;
    QList<int> modellist = modelsettings.value(ui->detectlineEdit->text()).value<QList<int> >();

    foreach (str, groups)
    {
        settings.beginGroup(str);
        int a = settings.value("upper").toInt();
        int b = settings.value("center").toInt();
        int c = settings.value("minimum").toInt();
        int d = settings.value("maximum").toInt();
        int cam = settings.value("camera").toInt();
        int x = settings.value("x").toInt();
        int y = settings.value("y").toInt();
        int width = settings.value("width").toInt();
        int height = settings.value("height").toInt();
        settings.endGroup();

        auto *camView = this->findChild<newCameraView*>(QStringLiteral("camView%1").arg(QString::number(cam)));

        if(camView)
        {
            if (camView->video.isOpened())
            {
                camView->video >> camView->frame;
                if(!camView->frame.empty())
                {
                    Rect _rect(x, y, width, height);
                    Mat _roi = camView->frame(_rect);
                    Mat _gray;
                    cvtColor(_roi, _gray, COLOR_BGR2GRAY);
                    medianBlur(_gray, _gray, 5);
                    std::vector<Vec3f> circles;
                    HoughCircles(_gray, circles, HOUGH_GRADIENT, 2, _gray.rows/8, a, b, c, d );

                    if (!circles.empty())
                    {
                        rectangle(camView->frame, _rect, Scalar(0, 255, 0), 2, 8, 0);
                        putText(camView->frame, str.toStdString(), Point(_rect.x, _rect.y-10), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(0, 255, 0), 2);

                        qDebug() << "Detected: " << circles.size() << " circles" <<endl;

                        for( size_t i = 0; i < circles.size(); i++ )
                        {
                            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                            int radius = cvRound(circles[i][2]);
                            circle( _roi, center, 3, Scalar(0,255,0), -1, 8, 0 );
                            circle( _roi, center, radius, Scalar(0,0,255), 1, 8, 0 );
                        }
                        resultlist.append(1);
                    }

                    else
                    {
                        qDebug() << "Detected: " << circles.size() << " circles" <<endl;
                        rectangle(camView->frame, _rect, Scalar(0, 0, 255), 2, 8, 0);
                        putText(camView->frame, str.toStdString(), Point(_rect.x, _rect.y-10), FONT_HERSHEY_COMPLEX_SMALL, 0.9, Scalar(0, 0, 255), 2);
                        resultlist.append(0);
                    }

                    QImage qimg(camView->frame.data,
                                camView->frame.cols,
                                camView->frame.rows,
                                camView->frame.step,
                                QImage::Format_RGB888);
                    camView->pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
                    camView->ui->graphicsView->fitInView(&camView->pixmap, Qt::KeepAspectRatio);
                }
            }
        }
        qApp->processEvents();
    }
    qDebug()<< "Result:" << resultlist;
    qDebug()<< "Model:" << modellist;

    for (int pos = 0; pos < modellist.count(); ++pos)
    {
        if(modellist.at(pos) != resultlist.at(pos)&& modellist.at(pos)!= -1)
        {
            settings.beginGroup(QString::number(pos+1));
            out.append(settings.value("description").toString());
            settings.endGroup();
        }
    }

//    qDebug()<< out;
//    start_detect = false;
}

void MainWindow::saveSettings()
{
    if (ui->zone_Numb->text().size() <= 0)
    {
        QMessageBox::warning(this, tr("Zone setting"), tr("Enter zone number"));
        ui->zone_Numb->setFocus();
        return;
    }

    if (ui->zone_Dscprt->text().size() <= 0)
    {
        QMessageBox::warning(this, tr("Zone setting"), tr("Enter zone description"));
        ui->zone_Dscprt->setFocus();
        return;
    }

    if(roi.empty())
    {
        QMessageBox::warning(this, tr("Zone setting"), tr("Zone not selected!\nPlease select zone."));
        return;
    }

    qDebug()<< "Start write settings to config";
    settings.beginGroup(ui->zone_Numb->text());
    settings.setValue("camera", ui->camera_comboBox->currentIndex());
    settings.setValue("description", ui->zone_Dscprt->text());
    settings.setValue("x", ui->zone_X->text());
    settings.setValue("y", ui->zone_Y->text());
    settings.setValue("width", ui->zone_Width->text());
    settings.setValue("height", ui->zone_Height->text());
    settings.setValue("upper", ui->spinBox_1->value());
    settings.setValue("center", ui->spinBox_2->value());
    settings.setValue("minimum", ui->spinBox_3->value());
    settings.setValue("maximum", ui->spinBox_4->value());
    settings.setValue("image", "images/" + ui->zone_Numb->text() + ".png");
    settings.endGroup();
    qDebug()<< "End write settings to config";

    qDebug()<< "Start saving image file";
    if (!QDir(QApplication::applicationDirPath() + "/images").exists())
    {
        QDir().mkdir(QApplication::applicationDirPath()+"/images");
    }
    std::string img_name = "images/" + ui->zone_Numb->text().toStdString() + ".png";
    if(!roi.empty()) imwrite(img_name, roi);
    qDebug()<< "End saving image file";

    ui->sliderFrame->hide();
    //    ui->roiWidget->hide();
    ui->zone_Numb->clear();
    ui->zone_Dscprt->clear();

    roi.release();
    roi.empty();
    ui->roiView->scene()->removeItem(&pixmap);
    ui->roiView->scene()->clear();
    ui->roiView->scene()->update();
    updateZoneList();
}

void MainWindow::updateZoneList()
{
    qDebug()<<"Start update zone list";
    ui->zoneView->clear();
    groups.clear();
    groups = settings.childGroups();
    QCollator collator;
    collator.setNumericMode(true);
    std::sort(groups.begin(), groups.end(), collator);
    qDebug() << groups;
    QString str;
    foreach (str, groups)
    {
        auto * newItem =  new QListWidgetItem(ui->zoneView);
        newItem->setSizeHint(QSize(ui->zoneView->width() - 4, 100));
        zoneItemWidget *newZoneItem = new zoneItemWidget(this);
        newZoneItem->setObjectName(QStringLiteral("newZoneItem%1").arg(str));
        settings.beginGroup(str);
        auto *img = new QImage(settings.value("image").toString());
        auto *pix = new QGraphicsPixmapItem(QPixmap::fromImage(*img));
        newZoneItem->ui->zone_numb_label->setText(tr("Zone № %1").arg(str));
        newZoneItem->ui->zone_descr_label->setText(settings.value("description").toString());
        newZoneItem->ui->zone_camera_label->setText(tr("Camera: ") + settings.value("camera").toString());
        settings.endGroup();
        newZoneItem->ui->graphicsView->scene()->addItem(pix);
        newZoneItem->ui->graphicsView->fitInView(pix, Qt::KeepAspectRatio);
        ui->zoneView->setItemWidget(newItem, newZoneItem);
    }
    qDebug()<<"End update zone list";
}

void MainWindow::on_addModelBtn_pressed()
{
    groups.clear();
    groups = settings.childGroups();
    QCollator collator;
    collator.setNumericMode(true);
    std::sort(groups.begin(), groups.end(), collator);
    QString str;
    foreach (str, groups)
    {
        auto *modelButton =  new QPushButton();
        modelButton->setCheckable(true);
        modelButton->setObjectName(QStringLiteral("modelButton%1").arg(str));

        auto *buttonlabel = new QLabel(tr("Zone № %1").arg(str));
        auto *holelabel = new QLabel(tr("Hole:"));
        auto *buttoncheckbox = new QCheckBox();
        buttoncheckbox->setObjectName(QStringLiteral("buttoncheckbox%1").arg(str));
        auto *buttonhbox = new QHBoxLayout();

        buttonhbox->addWidget(buttonlabel);
        buttonhbox->addWidget(holelabel);
        buttonhbox->addWidget(buttoncheckbox);
        modelButton->setLayout(buttonhbox);
        modelButton->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding, QSizePolicy::PushButton));
        ui->verticalLayout_3->addWidget(modelButton);
    }
    ui->add_modelFrame->show();
}

void MainWindow::on_deleteModelBtn_pressed()
{
    QList<QListWidgetItem *> items = ui->modelView->selectedItems();
    QString selected = ui->modelView->currentItem()->text();
//    qDebug()<< selected.left(selected.indexOf(':'));

    if(items.count() > 0)
    {
        int n = QMessageBox::critical(this, tr("Delete a complectation?"),
                                      tr("The selected complectation will be deleted!\n"
                                         "Do you really want to do this?"),
                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (n == QMessageBox::Yes)
        {
            modelsettings.remove(selected.left(selected.indexOf(':')));
            modelsettings.sync();
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Delete complectation"), tr("Complectation not selected!\nPlease select a complectation to delete."));
        return;
    }
    updateModelList();
}

void MainWindow::on_saveModelBtn_pressed()
{
    qRegisterMetaTypeStreamOperators<QList<int> >("QList<int>");
    QList<int> modelmask;
    modelmask.clear();
    if (ui->modellineEdit->text().size() <= 0)
    {
        QMessageBox::warning(this, tr("Save complectation"), tr("Enter complectation number"));
        ui->modellineEdit->setFocus();
        return;
    }
    QLayoutItem *child;
    while ((child = ui->verticalLayout_3->takeAt(0)) != 0)
    {
        auto *modelButton = findChild<QPushButton*>(child->widget()->objectName());
        auto *buttoncheckbox = modelButton->findChild<QCheckBox*>();
        if (modelButton->isChecked() && buttoncheckbox->isChecked())
        {
            modelmask.append(1);
        }
        if (modelButton->isChecked() && !buttoncheckbox->isChecked())
        {
            modelmask.append(0);
        }
        if (!modelButton->isChecked() && !buttoncheckbox->isChecked())
        {
            modelmask.append(-1);
        }
        delete modelButton;
    }
    modelsettings.setValue(ui->modellineEdit->text(), QVariant::fromValue(modelmask));
    ui->add_modelFrame->hide();
    updateModelList();
}

void MainWindow::on_canModelBtn_clicked()
{
    QLayoutItem* child;
    while ((child = ui->verticalLayout_3->takeAt(0)) != 0) {
        if (child->widget())
            child->widget()->setParent(0);
        auto *modelButton = findChild<QPushButton*>(child->widget()->objectName());
        delete modelButton;
        ui->verticalLayout_3->removeItem(child);
    }
    ui->add_modelFrame->hide();
}

void MainWindow::updateModelList()
{
    qRegisterMetaTypeStreamOperators<QList<int> >("QList<int>");
    qDebug()<<"Start update model list";
    ui->modelView->clear();
    QStringList modelgroups = modelsettings.allKeys();
//    qDebug()<< modelgroups;
    QString str;
    foreach (str, modelgroups)
    {
        QList<int> myList2 = modelsettings.value(str).value<QList<int> >();
//        qDebug() << myList2;
        QStringList sl;
        foreach(int n, myList2) sl << QString("%1").arg(n);
        QString resultString = sl.join(",");
        auto * newItem =  new QListWidgetItem(ui->modelView);
        newItem->setText(str + ": " + resultString);
        ui->zoneView->addItem(newItem);
    }
    qDebug()<<"End update model list";
}

void MainWindow::on_helpBtn_clicked()
{
    QMessageBox::about(0, tr("About"), tr("Hardware-software complex KULBIT\n"
                                          "Source code: %1").arg(QString("https://github.com/camellan/kulbit")));
}
