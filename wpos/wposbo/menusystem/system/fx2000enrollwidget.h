/***************************************************************************
                          fx2000enrollwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef FX2000ENROLLWIDGET_H
#define FX2000ENROLLWIDGET_H

#include <QWidget>
#include "ui_fx2000enrollwidgetbase.h"

class PosLabel;

class FX2000EnrollWidget :
        public QWidget,
        private Ui::FX2000EnrollWidgetBase
{
    Q_OBJECT

public:
    explicit FX2000EnrollWidget(QWidget *parent=0, const QString& name = QString());
    ~FX2000EnrollWidget();

    QString getId();
    void setReturnApplication(QString app_name,QString interface_name, QString method_name);

public slots:
    void setId(QString);

    void fingerprintAcquireSlot();
    void fingerprintReadSlot(QString xml_str);

protected slots:
    void doubleClickedAtLeft(QString);
    void doubleClickedAtRight(QString);
    void clearFingerLabel();

protected:
    virtual void showEvent(QShowEvent *e) override;

private:
    PosLabel *right_hand_label{};
    PosLabel *left_hand_label{};

    QString app_name;
    QString interface_name;
    QString method_name;
    QString id;

    int finger_number;
};

#endif
