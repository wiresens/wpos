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

#include "relatedproductgroup.h"
#include "database/barcoredb.h"

#include <QToolButton>
#include <QList>
#include <QMap>
#include <QWidget>
#include <xmlconfig.h>
#include <wposgui/common/global.h>

/**
*       This is the product, is compossed by list of lists of articles. It implements a state machine to
*       control wich articles has been pressed. All the configuration is loaded from an XML
*/

class QString;
class RelatedProductGroup;
class ProductExtraInfo;

class Product :
    public QToolButton,
    public RelatedProductGroup
{
    Q_OBJECT
public:

    static constexpr QSize MIN_SIZE = QSize{50,50};
    static constexpr QSize MAX_SIZE = QSize{105,105};

    explicit Product(
        const QString& name,
        QWidget *parent=0);

    Product(const QString& name,
        XmlConfig *xmlDescription,
        QWidget *parent=0);

    Product(const QString& name,
        const QString& xmlDescriptionFile,
        QWidget *parent=0);

    ~Product();

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

public slots:
    virtual void setProductName(const QString& _name);

    virtual bool setButtonAspect();
    virtual bool setProductPixmap(const QString& file_name);
    virtual bool setProductPixmap(const QPixmap& pixmap);
    virtual void setTextInPixmap(bool on);

    virtual void toggleChanged(bool state);

    virtual void setToggleFilter(const bool& filter);
    virtual bool getToggleFilter() const;

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

protected:
    void prepareSignals();
    void setTextAtButton();
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    /**
* Makes the real init and contructor for the object.
* Tries to load the list of combinations from the XML and construct
* all the articles tree. It's done in the following way:
*   1) We search the Product in the XML ,
*       if it doesn't exist, return false;
*       if it exists, gather all the information to construct to data structures.
*       The first one is the QList<Product*> where the data is at for real,
*       The second is a QMap wich is used to access the QList<Product*> with hashed tables (implemented at QT Level)
* @return true if the product has an equivalent in the XML and it has configured all the data structs
* @param QString with the name that identifies this product.
* @param XmlConfig wich has all the configuration of the products
*/
    bool initProduct(const QString& name, XmlConfig *xml);

private:
    //product selection
    QList<Product*> productVariants;
    HList<ProductExtraInfo> productExtraInfos;

    //the node will point to the current level of the tree to continue
    RelatedProductGroup *productData{};

    QString prod_name;
    QString family;
    int default_font_size { Sizes::FONT_SIZE_8};
    bool toggle_filter{false};

    static BarCoreDB db;
};

#endif
