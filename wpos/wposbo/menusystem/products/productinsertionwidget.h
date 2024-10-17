/***************************************************************************
                          naddproductwidget.h  -  description
                             -------------------
    begin                : jue abr 29 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef NADDPRODUCTWIDGET_H
#define NADDPRODUCTWIDGET_H

#include "ui_productinsertionwidgetbase.h"

#include "productsmodule/optionsmodule/productoptionmodule.h"
#include <wposcore/hlist.h>

class ProductModule;
class FloatKeyboard;
class NumKeyboard;
class ProductModule;
class BslDDIconView;
class BslDDTable;
class QMenu;
class QString;
class QListWidgetItem;

struct NOData{

    enum State{
        Original = 0,
        New = 1,
        Modified = 2,
        Deleted = 3
    };

    QString name;
    QString value{"0.0"};
    bool is_default{false};
    State state;
};

struct NOTypes {
    QString type;
    HList<NOData> list;
};

class ProductInsertionWidget :
        public QWidget,
        protected Ui::ProductInsertionWidgetBase  {
    Q_OBJECT

public:
    enum ProductMode{
        Unitary = 0,
        Composed =1
    };

    enum OfferColumn{

    };

    enum OfferTypeColumn{

    };

    enum OptionColumn{

    };

    enum OptionTypeColumn{

    };

    explicit ProductInsertionWidget(ProductModule *product_module ,int mode=0, QWidget *parent=0, const QString &name=QString{});
    ~ProductInsertionWidget();

    virtual int getMode();

public slots:
    virtual void setMode(int mode);

    virtual void clear();
    void clearMainTab();
    void clearComposedTab();
    void clearOptionTab();
    void clearOfferTab();

    void getTaxesButtons();
    void clearTaxesButtons();
    void taxChanged();

    //numkeys
    virtual void numkeyChangedSlot(double);
    void optionNumkeyChangedSlot(double);
    void offerNumkeyChangedSlot(double);
    void offerNumkeyChangedSlot(int);

    //main tab section slots
    void logoButtonClicked();
    void showPopLogo();
    void hidePopLogo();
    void logoClickedSlot(QListWidgetItem* item);

    //composed tab section slots
    void searchButtonClickedSlot();
    void searchEditChangedSlot(const QString &text);
    void upScrollUnitaryViewSlot();
    void downScrollUnitaryViewSlot();
    void draggedText(int x, int y, const QString& text);
    void upTableButtonClicked();
    void downTableButtonClicked();
    void deleteTableButtonClicked();

    void upOptionTypeSlot();
    void downOptionTypeSlot();
    void upOptionSlot();
    void downOptionSlot();
    void upOfferTypeSlot();
    void downOfferTypeSlot();
    void upOfferSlot();
    void downOfferSlot();

    void addOptionTypeSlot();
    void delOptionTypeSlot();
    void addOfferTypeSlot();
    void delOfferTypeSlot();

    void addOptionSlot();
    void delOptionSlot();
    void addOfferSlot();
    void delOfferSlot();

    void optionTypeSelectionChangedSlot();
    void offerTypeSelectionChangedSlot();

    void clickedOptionSlot(QTreeWidgetItem * item, int column );
    void offerSelectionChangedSlot();

    void applyOptionPriceSlot();
    void applyOfferPriceSlot();

    virtual void acceptSlot();
    virtual void cancelSlot();

signals:
    void progressSignal(int num, const QString& product);
    void cancelSignal();
    void acceptSignal();

protected slots:
    void showAction();
    void setProgressSlot(int num, const QString& product);
    void selectProduct(const QString& product);
    void nameChangedSlot(const QString& text);
    void tabChangedSlot(int index);

    void timerSlot();

protected:
    virtual void showEvent(QShowEvent *e);

    void getLogos();
    void getUnitaryLogos();

    //methods to fill the comboboxes
    void getOptionTypes();
    void getOfferTypes();
    void getOptionsFromType(const QString& type);
    void getOffersFromType(const QString& type);

    //method to show the lists at the listviews
    void showOptionTypes();
    void showOfferTypes();
    void showOptionsFromType(const QString& type);
    void showOffersFromType(const QString& type);

    NOData* getDefaultFromOptionType(const QString& type);
    QString getActualTax();
    QString getCurrentCppOperator();
    QString getCurrentProductCode();

    bool checkAllValues();
    bool isOptionSelected();
    bool isOfferSelected();
    void clearOptions();
    void clearOffers();

    virtual void save();
    virtual bool saveProduct();
    virtual bool saveProductComposition();
    virtual bool saveOptions();
    virtual bool saveOffers();

private:
    void fillOptions(const QString& domain, const QString& sub_domain, const QString& tag, QComboBox* cbx, const QString& data);
    QTreeWidgetItem* selectedItem() const;
protected:
    ProductOptionModule product_option_model;
    ProductModule *product_model{};
    FloatKeyboard *float_kb{};
    QButtonGroup *tax_button_group;
    BslDDTable *table{};
    HList<NOTypes> *options_list{};
    HList<NOTypes> *offers_list{};

    QString logo;
    int aux_counter;
    ProductMode mode{Unitary};

private:
    FloatKeyboard *float_kb_options{};
    FloatKeyboard *float_kb_offers{};
    NumKeyboard *num_kb_offers{};
    BslDDIconView *logo_view{};
    BslDDIconView *icon_view{};

    QMenu *pop_logo{};
    QTimer *timer{};
};

#endif
