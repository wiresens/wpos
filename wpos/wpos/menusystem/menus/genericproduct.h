/***************************************************************************
                          bslgenericproduct.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef GENERICPRODUCT_H
#define GENERICPRODUCT_H

#include "ui_freepricewidget.h"
#include <QObject>

class FloatKeyboardBox;
class XmlConfig;

class GenericProduct : public QWidget, private Ui::FreePriceWidget  {
    Q_OBJECT
public:
    explicit GenericProduct(const QString& product_name,
                      QWidget *parent=0,
                      const QString& name = QString());

    ~GenericProduct();

signals:
    void genericDataSignal(const QString& signal, XmlConfig *xml);

protected slots:
    void handleAccepted();
    void handleCancelled();
    void toggleButtonsState();

private:
    FloatKeyboardBox *key_pad;
    QString generic_product_name;
};

#endif
