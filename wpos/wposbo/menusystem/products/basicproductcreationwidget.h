/***************************************************************************
                          bsladdproductswidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef BSLADDPRODUCTSWIDGET_H
#define BSLADDPRODUCTSWIDGET_H

#include "ui_basicproductcreationwidgetbase.h"

#include <QList>
#include <QObject>

class ProductModule;
class BslDDIconView;
class XmlConfig;
class IngredientData;
class FloatKeyboard;
class ProductOptionCreationWidget;
class ProductOfferCreationWidget;
class BslDDTable;
class ProductData;
class QListWidgetItem;

class BasicProductCreationWidget :
        public QWidget,
        protected Ui::BasicProductCreationWidgetBase{

    Q_OBJECT
public:
    explicit BasicProductCreationWidget(ProductModule *_product_mod,
                                  int _mode_product = 0,
                                  QWidget *parent=0,
                                  const QString& name= QString{});

    ~BasicProductCreationWidget() = default;

    enum ProductMode{
        UnitaryProduct = 0,
        CompositionProduct =1
    };

    QWidget* getIdVisibleWidget();
    void getLogo(const QString& file_path);

public slots:
    void acceptSlot();
    void cancelSlot();

    void showWidgetAction();
    void showInsertProduct();

    void productNameChanged(const QString &text);
    void productPriceChanged(double price);
    void taxChanged();
    void logoButtonClicked();

    void showPopOption();
    void hidePopOption();

    void showPopOffer();
    void hidePopOffer();

    void showPopLogo();
    void hidePopLogo();

    void draggedText(int x, int y, const QString& text);
    void searchButtonClicked();
    void searchEditChanged(const QString &text);
    void selectProduct(const QString& product, BslDDIconView *icon_view);

    void upButtonClicked();
    void downButtonClicked();
    void deleteButtonClicked();
    void nextSlot();
    void previousSlot();

    void newOptionClicked();
    void newOfferClicked();

    void getLogo(QListWidgetItem *item);

protected:
    virtual void hideEvent(QHideEvent *e) override;
    virtual void showEvent(QShowEvent *e) override;

    void init();
    void clear();

    void setModeProduct(ProductMode _mode_product);
    void setTax(const QString& tax);
    QString getCode();
    QList<IngredientData*> getComposition();
    QString getTax();

    void insertItems(BslDDIconView *icon_view, bool unitary);
    int productPosition(const QString& name, XmlConfig* xml);

    void initTaxes();
    void initLogos(BslDDIconView *icon_view);

    bool insertNewProduct();
    bool insertProductComposition(const QString& code);
    bool insertNewComposition();

    void clearTaxes();

    void activeButtons();
    void setEnabled(bool enabled, const QString& button_pressed);

    void clearCodeList();
    void clearButtonList();

protected:
    FloatKeyboard *float_keyboard_product{};
    ProductOptionCreationWidget *option_widget{};
    ProductOfferCreationWidget *offer_widget{};
    BslDDIconView *icon_view{};
    BslDDIconView *logo_view{};
    BslDDTable *table{};

    ProductData *product;
    QString logo;

    QList<IngredientData*> code_list;
    QList<QPushButton*> button_list;

private:
    ProductModule *product_mod{};
    QMenu *pop_logo_menu{};
    QMenu *pop_option_menu{};
    QMenu *pop_offer_menu{};
    QButtonGroup* tax_button_group{};
    ProductMode product_mode{UnitaryProduct};
};

#endif
