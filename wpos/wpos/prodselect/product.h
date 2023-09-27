/***************************************************************************
                          product.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRODUCT_H
#define PRODUCT_H

#include <QWidget>

#include "productdata.h"

#include <QToolButton>
#include <QList>
#include <QMap>
#include <QWidget>
#include <xmlconfig.h>
#include <wposwidget/global.h>

/**
*       This is the product, is compossed by list of lists of articles. It implements a state machine to
*       control wich articles has been pressed. All the configuration is loaded from an XML
*/

class QString;
class ProductData;
class OptionNode;

class Product : public QToolButton, public ProductData {
    Q_OBJECT
public:

    static constexpr QSize MIN_SIZE = QSize{50,50};
    static constexpr QSize MAX_SIZE = QSize{105,105};

    explicit Product(const QString& product_name, QWidget *parent=0);
    Product(const QString& product_name, XmlConfig *xml, QWidget *parent=0);
    Product(const QString& product_name, const QString& file_name, QWidget *parent=0);
    ~Product();
public slots:

    /**
* It Makes the real init and contructor for the object, It tries to load the list of combinations
* from the XML and construct all the articles tree. It's done in the following way:
* First we search the Product at the XML , if it doesn't exist, the init returns false;
* If it exists at the XML it gather all the information to construct to data structs
* one is the QPtrList where the data is at, for real, the second is a QMap wich is used to access
* the QPtrList with hashed tables (implemented at QT Level)
* @return true if the product has an equivalence at the XML and it has configured all the data
* structs
* @param QString with the name and identifier of this product.
* @param XmlConfig wich has all the configuration of the products
*/

    virtual bool setButtonAspect();

    virtual bool setProductName(const QString& _name);
    virtual bool setProductPixmap(const QString& file_name);
    virtual bool setProductPixmap(const QPixmap& pixmap);
    virtual bool setTextInPixmap(bool on);

    virtual void toggleChanged(bool state);

    virtual void setToggleFilter(const bool& filter);
    virtual bool getToggleFilter();

    virtual XmlConfig* createProductDefinition();

    //received when it has been selected to be the leader.
    //return true if also it's the last product selected.
    virtual bool lead();
    //received when a combination has been clicked
    virtual void combinationClicked(Product *product);
    //received when a combination has been deleted
    virtual void combinationDeleted(Product *product);
    //disable only if it's not in toggle mode.
    virtual void disableProduct();
    virtual void enableProduct();

    virtual void defaultValues();
    virtual void setDefaultFontSize(int num);
    virtual void setDefaultTextFontSize(int num);
    virtual void setDefaultFontFamily(const QString& family);
    virtual void setDefaultTextFontFamily(const QString& family);

signals:
    //when a product is clicked
    void productClicked(Product *prod);
    //to emit wich articles it can combine with
    void combineWith(QStringList list);
    //to emit when the product has been defined
    void productDefinition(XmlConfig *xml);
    //to emit when the product is not the leader is de-toggled
    void deleteProduct(Product *prod);
    void genericDataSignal(const QString& signal_name,XmlConfig *xml);

protected:
    virtual void prepareSignals();
    virtual void setTextAtButton();
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    bool initProduct(const QString& productName, XmlConfig *xml);

    //the node will point to the current level of the tree to continue
    ProductData *productData{};

    //product selection
    QList<Product*> *product_list{};
    HList<OptionNode> *options_list{};

    QPixmap my_pixmap;
    QString prod_name;
    QString family;

    int default_font_size { Sizes::FONT_SIZE_8};
    bool toggle_filter{false};
    bool has_pixmap{false};

    int pad{4};
    int minSize{8};
};

#endif
