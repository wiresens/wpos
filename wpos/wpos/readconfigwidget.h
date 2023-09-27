/***************************************************************************
                          readconfigwidget.h  -  description
                             -------------------
    begin                : may 12 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef READCONFIGWIDGET_H
#define READCONFIGWIDGET_H

#include <QWidget>
#include "ui_readconfigwidgetbase.h"

class XmlConfig;

class ReadConfigWidget : public QWidget, private Ui::ReadConfigWidgetBase  {
    Q_OBJECT
public: 
    ReadConfigWidget(QWidget *parent=nullptr, const QString& name = QString(""));
    ~ReadConfigWidget();

public slots:
    void genericDataSignalSlot(const QString& signal_name, XmlConfig* xml);

protected:
    void showEvent(QShowEvent *e);
    int pos;
};

#endif
