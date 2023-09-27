/***************************************************************************
                          productscreen.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRODUCT_SCREEN_H
#define PRODUCT_SCREEN_H

#include <QWidget>
#include <QList>
#include <QMap>

#define DEFAULT_COLS 4
#define DEFAULT_ROWS 4

class QGridLayout;
class Product;
class XmlConfig;

class ProductScreen : public QWidget  {
    Q_OBJECT
public: 

//    static const QColor PRODUCT_SCREEN_BG_COLOR         = QColor{170,170,170};
//    static const QColor PRODUCT_SCREEN_ESPECIAL_COLOR   = QColor(239,224,59);
//    static const QColor PRODUCT_SCREEN_DEFAULT_OFFER    = QColor(237,173,128);
//    static const QColor PRODUCT_SCREEN_ANULATION_COLOR  = QColor(250,75,75);

    enum ModeColor{
        Normal = 0,
        Invitation = 1,
        Product_anulation = 2,
        Default_offer = 3
    };

    ProductScreen(const QString& screen_name, XmlConfig *xml,
                  QWidget *_parent, const QString& name = QString());

    ProductScreen(const QString& screen_name, const QString& file_name,
                  QWidget *_parent, const QString& name = QString());

    ~ProductScreen();

    virtual bool initScreen(const QString& screen_name, XmlConfig *xml);
    virtual bool resetScreen();

    virtual QString screenName();
    virtual void setScreenName(const QString& screen_name);
    virtual QString nextPreferredScreenName();

    virtual void showAll();

public slots:
    void disableAll();
    void defaultValuesAll();

    void productClickedSlot(Product *prod);
    void deleteProductSlot(Product *prod);
    void combineWithSlot(QStringList list);
    void productDefinitionSlot(XmlConfig *xml);

    void setSpecialModeColor(ModeColor mode);

signals:
    void productDefinition(XmlConfig *xml);
    void defaultValue();
protected:
    void prepareConnects(Product *prod);

    QString name;
    int cols {DEFAULT_COLS}, rows{DEFAULT_ROWS};

    Product *selected_product{};

    QString next_preferred_screen;

    QList<Product*> *product_list {new QList<Product*>};
    QMap<QString, Product*> *product_dict {new QMap<QString,Product*>};
    QGridLayout *layout;

    int default_fontsize{ 8 };
    int default_text_fontsize { 10 };
    QString default_fontfamily{"Arial"};
    QString default_text_fontfamily {"Arial Black"};

    static int SCREEN_PRODUCT_SPACING;
};

#endif
