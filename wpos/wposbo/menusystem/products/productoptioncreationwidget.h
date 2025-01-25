/***************************************************************************
                          bsladdoptionwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#ifndef BSLADDOPTIONWIDGET_H
#define BSLADDOPTIONWIDGET_H

#include "ui_productoptioncreationwidgetbase.h"

#include "productsmodule/optionsmodule/productoptionmodule.h"

#include <QList>
#include <QStringList>
#include <QWidget>

class FloatKeyboard;

class ProductOptionCreationWidget : public QWidget,
                                    private Ui::ProductOptionCreationWidgetBase {
    Q_OBJECT
public:
    explicit ProductOptionCreationWidget(double _product_price, QWidget* parent = 0, const QString& name = QString {});
    ~ProductOptionCreationWidget() = default;

    void setProductPrice(double _product_price);
    void initOptionTypes();
    void setEditable(bool editable);
    bool saveOptionsProduct(const QString& product_code);
    void saveOptions();
    void checkDefaultOptions();
    const QList<ProductOptionData>& getOptions() const;

public slots:
    void optionTypeActivated(const QString& text);
    void saveOptionButtonClicked();

protected:
    void updateOptions();
    void clearDefaults();
    void clear();
    void initLogos();

private:
    FloatKeyboard* float_keyboard_option {};
    ProductOptionModule model;
    QList<ProductOptionData> options;
    double product_price;
};

#endif
