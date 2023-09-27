/***************************************************************************
                          readconfigwidget.h  -  description
                             -------------------
    begin                : may 12 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef READ_CONFIG_SCREEN_H
#define READ_CONFIG_SCREEN_H

#include <QWidget>
#include "ui_readconfigwidget.h"

class XmlConfig;

class ReadConfigScreen : public QWidget, private Ui::ReadConfigWidget {
    Q_OBJECT
public:
    explicit ReadConfigScreen(QWidget *parent, const QString& name = QString(""));
    ~ReadConfigScreen();

public slots:
    void genericDataSignalSlot(const QString& signal_name, XmlConfig* xml);

protected:
    void showEvent(QShowEvent *e);
    uint pos{0};
};

#endif
