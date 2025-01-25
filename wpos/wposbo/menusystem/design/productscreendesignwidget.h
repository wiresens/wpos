/***************************************************************************
                          bslscreenproductsdesignwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRODUCTSCREENDESIGNWIDGET_H
#define PRODUCTSCREENDESIGNWIDGET_H

#include "ui_screenproductsdesigninterface.h"

#include <libbslxml/xmlconfig.h>
#include <wposcore/hlist.h>

class BslDDTable;
class BslDDIconView;
class ScreenNode;
class ProductData;

class ProductScreenDesignWidget : public QWidget,
                                  private Ui::ScreenProductsDesignInterface {

    Q_OBJECT
public:
    explicit ProductScreenDesignWidget(QWidget* parent = 0, const QString& name = QString {});
    ~ProductScreenDesignWidget() = default;

signals:
    void cancel();
    void productReaded(int num, const QString& screen_name, const QString& product_name, const QString& logo);

protected slots:
    void forwardReleased();
    void backReleased();
    void acceptReleased();

    void nameScreenChanged(const QString& text);
    void numColumnsChanged(int value);
    void numRowsChanged(int value);

    void draggedText(int x, int y, const QString& text);
    void clear();

    void startShowing();
    void productReadedSlot(int num, const QString& screen_name, const QString& product_name, const QString& logo);

    void deleteSlot();

    void nextScreenSlot();
    void prevScreenSlot();

    void upSlot();
    void downSlot();

protected:
    void showEvent(QShowEvent* e);
    void hideEvent(QHideEvent* e);

    void getScreens();
    void initScreens();
    void writeScreensAtXml();

    void initUnitaryProducts();
    void setProductScreen(const QString& screen_name);

    void clearTable();

    ProductData* getProduct(const QString& product_code);
    ProductData* getProductWithName(const QString& product_name);
    ProductData* xmlToProduct(const QString& xml_str);

private:
    BslDDTable* screen {};
    BslDDIconView* products {};
    HList<ScreenNode>* screen_list {};

    QString actual_namestring;
    int actual_size;
};

#endif
