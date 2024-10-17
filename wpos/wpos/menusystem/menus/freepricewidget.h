/***************************************************************************
                          bslfreepricewidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef FREEPRICEWIDGET_H
#define FREEPRICEWIDGET_H

#include "ui_freepricewidget.h"
#include <QWidget>

class XmlConfig;
class FloatKeyboard;
class QString;

class FreePriceWidget : public QWidget, private Ui::FreePriceWidget  {
    Q_OBJECT
public:
    explicit FreePriceWidget(QWidget *parent=0, const QString& name = QString{});
    ~FreePriceWidget();

signals:
    void genericDataSignal(const QString& signal_name,XmlConfig *_xml);

protected slots:
    void handleAccepted();
    void handleCancelled();
    void toggleButtonsState();

protected:
    FloatKeyboard *key_pad;
};

#endif
