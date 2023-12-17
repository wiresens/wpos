/***************************************************************************
                          ordercontentview.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Jose Ignacio Ferrero-Carrera
Modified by Carlos Manzanedo Rueda.

%LICENCIA%
 ***************************************************************************/

#ifndef ORDERCONTENTVIEW_H
#define ORDERCONTENTVIEW_H

#include "global.h"
#include <wposcore/hlist.h>
#include <QTableWidget>


//  Order has the following initial appearance
//  ------------------------------------------------------
//  | UNITS | DESCRIPTION | PRIORITY | TIMESTAMP | TOTAL |
//  ------------------------------------------------------
//  |       |             |          |           |       |
//
//  Columns should be movables and their positions are saved
//  in the xml that comtains order description

class XmlConfig;
class VisualProduct;

class OrderContentView :
    public QTableWidget
{
    Q_OBJECT
public:
    OrderContentView(
        XmlConfig &contentXmlDesc,
        QWidget *parent,
        QString name
    );

    ~OrderContentView();

    void usePartialSumsByRow(bool use_it);
    void showColumn(const QString &name, bool visible);
    XmlConfig* orderAsXml() const;

public slots:
    void genericSignalSlot(const QString& signal);
    void genericDataSignalSlot(const QString& signal, XmlConfig *xml);

    void setDefaultColor();

    void selectUp();
    void selectDown();
    void selectFirst();
    void selectLast();

    void clearOrder();
    void reset();
    void delRow();
    void incrementQuantity();
    void decrementQuantity();

    // This slot is used to load an order given in an XML definition file
    //void loadProductsInfo(XmlConfig *xml);
    // Called each time a new product is clicked
    void updateProductsInfo(XmlConfig *xml);
    void printTicket();

signals:
    void totalChanged(float amount);
    void contentChanged(XmlConfig *xml);

private:
    void parseXmlDescription(XmlConfig &contentXmlDesc);
    void total();
    void buildProductsList();
    QString readOptions(QString optionsDomain);
    void setTimeStamp(const QString &domain);
    void selectLastOrderItem();
    void fillVisualOrderWithProductsList();

    //debug purposes
    void debugVProductList();
    void debug();

private:
    // This XML is received each time a product change in the CORE
    XmlConfig *local_products_xml{};

    // Here we maintain the internal products list extracted from the XML
    HList<VisualProduct> *products{};

    // This list holds the column position at visual table
    QMap<QString, int> columnPositions{};

    // Used to manage color state at the gsignal_wrong_product
    QColor last_color;
    uint color_state{0};

    uint row_height{Sizes::ROW_HEIGTH};

    // For partial total of each row , quantity*price
    bool use_partial_sums{false};
};

#endif
