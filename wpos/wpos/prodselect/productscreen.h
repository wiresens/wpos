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

class ProductScreen:
    public QWidget
{
    Q_OBJECT

public:
    enum ModeColor{
        Normal             = 0,
        Invitation         = 1,
        ProductAnulation   = 2,
        DefaultOffer       = 3
    };

    ProductScreen(
        const QString& screenName,
        XmlConfig *xmlDescription,
        QWidget *parent,
        const QString& name = QString{});

    ProductScreen(
        const QString& screenName,
        const QString& xmlDescriptionFile,
        QWidget *parent,
        const QString& name = QString{});

    ~ProductScreen();

    bool initScreen(const QString& screen_name, XmlConfig *xml);
    bool resetScreen();

    QString screenName();
    QString nextPreferredScreenName();

    void showAll();

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

    QString screenName_;
    QString next_preferred_screen;

    Product *selected_product{};

    int cols {DEFAULT_COLS};
    int rows {DEFAULT_ROWS};

    QList<Product*> *productList {new QList<Product*>};
    QMap<QString, Product*> *productMap {new QMap<QString, Product*>};
    QGridLayout *layout;

    QString default_fontfamily{"Arial"};
    QString default_text_fontfamily {"Arial Black"};
    int default_fontsize{ 8 };
    int default_text_fontsize { 10 };

    static int SCREEN_PRODUCT_SPACING;
};

#endif
