/***************************************************************************
                          nupdateoptionwidget.cpp  -  description
                             -------------------
    begin                : Mon Apr 28 2004
    copyright            : (C) 2004 by Carlos Manzanedo
    email                : carlos@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "optionupdatewidget.h"
//#include "optionupdatewidget_adapter.h"

#include "productsmodule/optionsmodule/productoptionmodule.h"
#include <wposgui/keyboard/floatkeyboard.h>
#include <libbslxml/xmlconfig.h>

#include <QtDBus/QDBusConnection>
#include <QMessageBox>

static const QString& NUPDATE_ADVERTISE_TEXT = QObject::tr("If you want to apply the option to all products, click apply.\nWarnning: if you add a value to this option, all the products \n will add to their original price, the entered value.\nIf you want to set a value for each product, go to the products section.");

const QString OptionUpdateWidget::DBusObjectPath  = QString{"/wpos/wposbo/OptionUpdateModel"};

OptionUpdateWidget::OptionUpdateWidget(QWidget *parent, const QString& name ) :
    OptionEditionWidget(parent, name)
{
//    new OptionUpdateModelAdaptor(this);
//    QDBusConnection dbus = QDBusConnection::sessionBus();
//    dbus.registerObject(OptionUpdateWidget::DBusObjectPath, this);

    title_label->setText(tr("Modify Prices in Options"));
    option_lineedit->hide();
    add_type_button->hide();
    add_option_button->hide();
    type_lineedit->hide();
    del_type_button->hide();

    //unique
    group_box->setCheckable(false);
    advertise_label->setText(NUPDATE_ADVERTISE_TEXT);
    box_button->setText(tr("Insert Option\n in all\nproducts"));

    connect(option_listview, &QTreeWidget::itemSelectionChanged, this, &OptionUpdateWidget::optionClickedSlot); //idem
    connect(box_button,  &QPushButton::clicked, this, &OptionUpdateWidget::applyToAllProductsSlot);
}

void OptionUpdateWidget::optionClickedSlot(){
    group_box->setEnabled(checkAllValues());
    ok_button->setEnabled(checkAllValues());
}

void OptionUpdateWidget::getTypes(){
    OptionEditionWidget::getTypes();
    ok_button->setEnabled(checkAllValues());
    group_box->setEnabled(checkAllValues());
}

void OptionUpdateWidget::getOptionTypes(const QString& type){
    OptionEditionWidget::getOptionTypes(type);
    ok_button->setEnabled(checkAllValues());
    group_box->setEnabled(checkAllValues());
}

void OptionUpdateWidget::showEvent(QShowEvent *e){
    clear();
    getTypes();
    group_box->setEnabled(checkAllValues());
    ok_button->setEnabled(checkAllValues());
    QWidget::showEvent(e);
}

void OptionUpdateWidget::acceptSlot(){
    if (!checkAllValues()) return;
    auto option = option_listview->selectedItems().first()->text(0);
    auto type = type_listview->selectedItems().first()->text(0);

    double mul = 1.0;
    if ( !plus_button->isDown() )  mul = 1.0;
    else if ( !minus_button->isDown()) mul = -1.0;
    auto value = mul * float_kb->value();

    auto title = tr("Aplicar nuevo precio a opcion");
    auto msg = tr("Se va a proceder a la actualizacion de todos los productos\n que posean la opcion %1 - %2 Estos productos pasaran a valer: %3").arg(type).arg(option).arg(QString::number(value,'f', 2));
    if ( value > 0) msg += tr(" mas", "suma");
    msg += tr("sobre su valor, cuando se pulse esa opcion\n\n\Esta seguro de querer aplicar el cambio ?","Actualizacion de precios de productos");

    if( QMessageBox::question( this, title, msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No ) return;
    mod.updateAllProductOptionValue(type, option, QString::number(value, 'f', 2));
    clear();
    getTypes();
}

void OptionUpdateWidget::applyToAllProductsSlot(){

    if (!checkAllValues()) return;
    auto option = option_listview->selectedItems().first()->text(0);
    auto type = type_listview->selectedItems().first()->text(0);

    double mul = 1.0;
    if ( !plus_button->isDown())  mul = 1.0;
    else if ( !minus_button->isDown()) mul = -1.0;
    auto value = mul * float_kb->value();
    auto title = tr("Aplicar nuevo precio a opcion");

    auto msg = tr("Se va a proceder a insertar la opcion : %1 - %2 \n\en todos los productos. Estos productos pasaran a valer : %3").arg(type).arg(option).arg(QString::number(value,'f',2));
    if (value > 0) msg += tr(" mas");
    msg += tr(" sobre su valor, cuando se pulse esa opcion\n\n\Esta seguro de querer aplicar el cambio ?");

    if( QMessageBox::question( this, title, msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No ) return;

    mod.setAllProductOptionValue(type, option, QString::number(value,'f',2));
    clear();
    getTypes();
}

void OptionUpdateWidget::notify(){
//    QByteArray params;
//    QDataStream stream(params, IO_WriteOnly);
//    kapp->dcopClient()->emitDCOPSignal("optionChanged()", params);
    emit optionChanged();
}
