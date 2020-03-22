#ifndef ZONEITEMWIDGET_H
#define ZONEITEMWIDGET_H

#include "ui_zoneitemwidget.h"

namespace Ui {
class zoneItemWidget;
}

class zoneItemWidget : public QFrame
{
    Q_OBJECT

public:
    explicit zoneItemWidget(QWidget *parent = nullptr);
    ~zoneItemWidget();
    Ui::zoneItemWidget *ui;
    QString getZoneNumber()
    {
        return ui->zone_numb_label->text().remove(0, 7);
    }

};

#endif // ZONEITEMWIDGET_H
