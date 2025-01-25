/***************************************************************************
                          ndeloptionwidget.cpp  -  description
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

#include "optiondeletionwidget.h"
// #include "optiondeletionwidget_adapter.h"

#include "productsmodule/optionsmodule/productoptionmodule.h"
#include <libbslxml/xmlconfig.h>
#include <wposgui/keyboard/floatkeyboard.h>

#include <QMessageBox>
#include <QtDBus/QDBusConnection>

const QString OptionDeletionWidget::DBusObjectPath = QString { "/wpos/wposbo/OptionDeletionModel" };

OptionDeletionWidget::OptionDeletionWidget(QWidget* parent, const QString& name)
    : OptionEditionWidget(parent, name)
{
    //    new OptionDeletionModelAdaptor(this);
    //    QDBusConnection dbus = QDBusConnection::sessionBus();
    //    dbus.registerObject(OptionDeletionWidget::DBusObjectPath, this);

    title_label->setText(tr("Delete Options"));
    option_lineedit->hide();
    add_type_button->hide();
    add_option_button->hide();
    type_lineedit->hide();
    group_box->hide();

    connect(option_listview, &QTreeWidget::itemSelectionChanged, this, &OptionDeletionWidget::optionClickedSlot); // idem
    connect(del_type_button, &QPushButton::clicked, this, &OptionDeletionWidget::delTypeSlot);
}

void OptionDeletionWidget::delTypeSlot()
{
    auto items = type_listview->selectedItems();
    if (items.isEmpty())
        return;

    auto type = items.first()->text(0);
    auto msg = tr("Se va a proceder al borrado de un tipo de opcion\n\esto implica que todas las opciones pertenecientes\n\a dicho tipo tambien seran borradas\n\Esta seguro de querer borrar el tipo de opcion %1 ?").arg(type);
    if (QMessageBox::question(this, tr("Borrar tipo de opcion"), msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    if (!mod.deleteOptionType(type)) {
        msg = tr("No se ha podido borrar el tipo de opcion");
        QMessageBox::information(this, tr("fallo al borrar el tipo de opcion"), msg, QMessageBox::Ok);
    }
    clear();
    getTypes();
}

bool OptionDeletionWidget::delOption(const QString& type, const QString& option)
{

    XmlConfig xml;
    xml.createElementSetDomain("options.option");
    xml.createElement("option_type", type);
    xml.createElement("option_name", option);
    auto xml_string = xml.toString();

    auto title = tr("fallo al borrar la opcion");
    if (!mod.existOption(xml_string)) {
        auto msg = tr("No existe esa opcion con ese tipo de opcion\n");
        QMessageBox::information(this, title, msg, QMessageBox::Ok);
        return false;
    }

    if (!mod.deleteOption(xml_string)) {
        auto msg = tr("No se ha podido borrar la opcion\n quiza no exista una opcion con ese nombre\n");
        QMessageBox::information(this, title, msg, QMessageBox::Ok);
        return false;
    }
    return true;
}

void OptionDeletionWidget::optionClickedSlot()
{
    ok_button->setEnabled(checkAllValues());
}

void OptionDeletionWidget::getTypes()
{
    OptionEditionWidget::getTypes();
    ok_button->setEnabled(checkAllValues());
}

void OptionDeletionWidget::getOptionTypes(const QString& type)
{
    OptionEditionWidget::getOptionTypes(type);
    ok_button->setEnabled(checkAllValues());
}

void OptionDeletionWidget::showEvent(QShowEvent* e)
{
    clear();
    getTypes();
    ok_button->setEnabled(checkAllValues());
    QWidget::showEvent(e);
}

void OptionDeletionWidget::acceptSlot()
{
    if (!checkAllValues())
        return;
    auto option = option_listview->selectedItems().first()->text(0);
    auto type = type_listview->selectedItems().first()->text(0);

    auto msg = tr("Se va a proceder al borrado de una opcion\n\Esta seguro de querer borrar la opcion %1 - %2 ?").arg(type).arg(option);
    if (QMessageBox::question(this, tr("Borrar opcion"), msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        return;

    delOption(type, option);
    getOptionTypes(type);
    ok_button->setEnabled(checkAllValues());
}

void OptionDeletionWidget::notify()
{
    //    QByteArray params;
    //    QDataStream stream(params, IO_WriteOnly);
    //    kapp->dcopClient()->emitDCOPSignal("optionChanged()", params);
    emit optionChanged();
}
