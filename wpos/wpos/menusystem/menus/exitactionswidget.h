/***************************************************************************
                          bslexitactionswidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef EXITACTIONSWIDGET_H
#define EXITACTIONSWIDGET_H

#include "ui_exitactionswidget.h"
#include <QWidget>

/**
  *@author
  */

class XmlConfig;

class ExitActionsWidget : public QWidget, private Ui::ExitActionsWidget  {
    Q_OBJECT
public:
    explicit ExitActionsWidget(QWidget *parent=0, const QString& name = QString());
    ~ExitActionsWidget();

public slots:
    void returnToADMmenuSlot();
    void shutdownSlot();

    
signals:
    void genericSignal(const QString& signal_name);
    void genericDataSignal(const QString& signal_name,XmlConfig *xml);

protected:
    void initDialog();
};

#endif
