/***************************************************************************
                          devicesconfig.cpp  -  description
                             -------------------
    begin                : lun mar 28 2005
    copyright            : (C) 2005 by Carlos Manzanedo
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

#include "devicesconfigwidget.h"

#include <libbslxml/xmlconfig.h>

#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QButtonGroup>
#include <QLineEdit>
#include <QRadioButton>
#include <QLocale>

#include <iostream>
using namespace std;

static const QString& DEVICES_CONFIG {"/etc/ntpv/devices_config.xml"};
static const QString& DCOPPRINTER_CONFIG {"/etc/ntpv/dcopprinter_config.xml"};
static const QString& TEMPLATES_LOCATION {"/etc/ntpv/templates/"};
static const QString& SAVED_CONFIG {"/etc/ntpv/company_ticket_data.xml"};
static const QString& MAIN_TICKET_FILE {"/etc/ntpv/printerhtml.xml"};
static const QString& MAIN_INVOICE_FILE {"/etc/ntpv/invoice.xml"};

static const int CHAR_42 = 42;
static const int CHAR_40 = 40;

DevicesConfig::DevicesConfig(QWidget *parent, const QString& name ):
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);

    drawer_type_map = {
        {"serial", Serial},
        {"cash_drawer",Drawer},
        {"p_samsung_350", Samsung}
     };

    printer_type_map = {
        {"direct", Direct},
        {"ipp", Ipp}
     };

    device_map = {
        {"/dev/ttyS0", S0},
        {"/dev/ttyS1" , S1},
        {"/dev/ttyS2" , S2},
        {"/dev/ttyS3" , S3},
        {"/dev/lp0" , P0},
        {"/dev/lp1", P1},
        {"/dev/ttyUSB0", USB0},
        {"/dev/ttyUSB1", USB1},
        {"/dev/ttyUSB2", USB2},
        {"/dev/ttyUSB3", USB3}
    };

    cashbox_icon->setPixmap(QPixmap("/usr/share/ntpv_backoffice/apps/64x64/ddbb.png"));
    printer_icon->setPixmap(QPixmap("/usr/share/ntpv_backoffice/apps/64x64/printer4.png"));
    kitchen_icon->setPixmap(QPixmap("/usr/share/ntpv_backoffice/apps/64x64/printer4.png"));
    ok_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/button_ok_48.png"));

    printer_lineedit->hide();
    kitchen_lineedit->hide();

    //connection between buttons
    connect(printer_type_combobox,SIGNAL(activated(int)),this,SLOT(printerTypeChanged(int)));
    connect(kitchen_type_combobox,SIGNAL(activated(int)),this,SLOT(kitchenTypeChanged(int)));

    connect(printer_device_combobox,SIGNAL(activated(int)),this,SLOT(printerDeviceChanged(int)));
    connect(kitchen_device_combobox,SIGNAL(activated(int)),this,SLOT(kitchenDeviceChanged(int)));

    connect(cashbox_type_combobox, QOverload<int>::of(&QComboBox::activated),this, QOverload<int>::of(&DevicesConfig::somethingChanged));
    connect(cashbox_device_combobox, QOverload<int>::of(&QComboBox::activated), this,  QOverload<int>::of(&DevicesConfig::somethingChanged));
    connect(printer_chars_groupbox, &QGroupBox::clicked, this, QOverload<bool>::of(&DevicesConfig::somethingChanged));
    connect(kitchen_chars_groupbox, &QGroupBox::clicked,this, QOverload<bool>::of(&DevicesConfig::somethingChanged));

    connect(printer_lineedit,SIGNAL(textChanged(const QString&)),this,SLOT(printerIppDeviceChanged(const QString &)));
    connect(kitchen_lineedit,SIGNAL(textChanged(const QString&)),this,SLOT(kitchenIppDeviceChanged(const QString &)));

    connect(ok_button,SIGNAL(released()),this,SLOT(acceptSlot()));

}

void DevicesConfig::readDevicesConfig(){
    readCashboxConfig();
    readPrinterConfig();
}

void DevicesConfig::readCashboxConfig(){

    XmlConfig xml(DEVICES_CONFIG);
    if ( !xml.isValid() ) return;

    bool writexml = false;
    if ( !xml.setDomain("cashbox") ){
        xml.createElementSetDomain("cashbox");
        xml.createElement("type","cash_drawer");
        xml.createElement("dev","/dev/lp0");
        writexml = true;
    }

    auto type = xml.readString("type");
    if ( type.isEmpty() ){
        type = "cash_drawer";
        xml.doWrite("type", type);
        writexml = true;
    }

    auto device = xml.readString("dev");
    if (device.isEmpty()){
        device = "/dev/lp0";
        xml.doWrite("dev", device);
        writexml = true;
    }

    if ( writexml ) xml.save();

    if(drawer_type_map.contains(type))
         cashbox_type_combobox->setCurrentIndex(drawer_type_map[type]);
    else
        cashbox_type_combobox->setCurrentIndex(Drawer);

    if( device_map.contains(device) )
        cashbox_device_combobox->setCurrentIndex(device_map[device]);

    else{
        device_map[device] = (CashboxDevice) device_map.size();
        cashbox_device_combobox->insertItem(device_map[device], device);
        cashbox_device_combobox->setCurrentIndex(device_map[device]);
    }
}

void DevicesConfig::readPrinterConfig(){
    bool write_xml = false;
    bool kitchen_printer = false;
    QString kitchen_type,kitchen_device;
    int kitchen_char_length;

    XmlConfig xml(DCOPPRINTER_CONFIG);
    if ( !xml.isValid() ) return;

    if (!xml.setDomain("main")){
        xml.createElementSetDomain("main");
        xml.createElement("type","direct");
        xml.createElement("device","/dev/lp0");
        xml.createElement("length","42");
        write_xml = true;
    }

    auto printer_type = xml.readString("type");
    if (printer_type.isEmpty()){
        printer_type = "direct";
        xml.doWrite("type",printer_type);
        write_xml = true;
    }

    auto printer_device = xml.readString("device");
    if (printer_device.isEmpty()){
        printer_device = "/dev/lp0";
        xml.doWrite("device",printer_device);
        write_xml = true;
    }
    auto printer_char_length = xml.readString("length").toInt();
    if ( printer_char_length != CHAR_42 || printer_char_length != CHAR_40 )
        printer_char_length = CHAR_42;

    xml.releaseDomain("main");
    if (!xml.setDomain("kitchen")){
        kitchen_printer = false;
        kitchen_type = "direct";
        kitchen_device = "/dev/lp0";
        kitchen_char_length = CHAR_42;
    }
    else{
        kitchen_printer = true;
        kitchen_type = xml.readString("type");
        kitchen_device = xml.readString("device");

        if (kitchen_type.isEmpty()){
            kitchen_type = "direct";
            xml.doWrite("type",kitchen_type);
            write_xml = true;
        }

        kitchen_device = xml.readString("device");
        if (kitchen_device.isEmpty()){
            kitchen_device = "/dev/lp0";
            xml.doWrite("device",kitchen_device);
            write_xml = true;
        }

        printer_char_length = xml.readString("length").toInt();
        if ( printer_char_length != CHAR_42 || printer_char_length != CHAR_40 )
            printer_char_length = CHAR_42;
    }

    if (write_xml) xml.save();

    //main printer config
//    if(printer_type_map.contains(printer_type))
//        printer_type_combobox->setCurrentIndex(printer_type_map[printer_type]);
//        if ( printer_type == "direct" ){
//            printer_lineedit->hide();
//            printer_device_combobox->show();
//            if(device_map.contains(printer_device))
//                printer_device_combobox->setCurrentIndex(device_map[printer_device]);
//            else
//            {
//                device_map[device] = (CashboxDevice) device_map.size();
//                printer_device_combobox->insertItem(device_map[device], device);
//                printer_device_combobox->setCurrentIndex(device_map[device]);
//            }
//        }
//    else;

    if ( printer_type == "direct" ){
        printer_type_combobox->setCurrentIndex(0);
        printer_lineedit->hide();
        printer_device_combobox->show();

        if (printer_device == "/dev/lp0")
            printer_device_combobox->setCurrentIndex(0);
        else if (printer_device == "/dev/lp1")
            printer_device_combobox->setCurrentIndex(1);
        else if (printer_device == "/dev/ttyS0")
            printer_device_combobox->setCurrentIndex(2);
        else if (printer_device == "/dev/ttyS1")
            printer_device_combobox->setCurrentIndex(3);
        else if (printer_device == "/dev/ttyS2")
            printer_device_combobox->setCurrentIndex(4);
        else if (printer_device == "/dev/ttyS3")
            printer_device_combobox->setCurrentIndex(5);
        else if (printer_device == "/dev/ttyUSB0")
            printer_device_combobox->setCurrentIndex(6);
        else if (printer_device == "/dev/ttyUSB1")
            printer_device_combobox->setCurrentIndex(7);
        else if (printer_device == "/dev/ttyUSB2")
            printer_device_combobox->setCurrentIndex(8);
        else if (printer_device == "/dev/ttyUSB3")
            printer_device_combobox->setCurrentIndex(9);
        else{
            printer_device_combobox->insertItem(10, printer_device);
            printer_device_combobox->setCurrentIndex(10);
        }
        preferred_printer_device = printer_device_combobox->currentIndex();
    }
    else if ( printer_type == "ipp"){
        printer_type_combobox->setCurrentIndex(1);
        printer_lineedit->show();
        printer_device_combobox->hide();
        printer_lineedit->setText(printer_device);
        preferred_printer_ipp_device = printer_device;
    }
    else{
        printer_type_combobox->setCurrentIndex(0);
        printer_lineedit->hide();
        printer_device_combobox->show();
        printer_device_combobox->setCurrentIndex(0);
    }

    if ( printer_char_length == CHAR_42){
        printer_length_42_radiobutton->setChecked(true);
    }
    else if (printer_char_length == CHAR_42){
        printer_length_40_radiobutton->setChecked(true);
    }

    preferred_printer_device = printer_device_combobox->currentIndex();
    preferred_printer_ipp_device = printer_device;

    // kitchen printer config
    if (!kitchen_printer){
        kitchen_groupbox->setChecked(false);
        kitchen_lineedit->hide();
        kitchen_device_combobox->show();
        kitchen_device_combobox->setCurrentIndex(0);
    }
    else{
        if (kitchen_type == "direct"){
            kitchen_type_combobox->setCurrentIndex(0);
            kitchen_lineedit->hide();
            kitchen_device_combobox->show();

            if (kitchen_device == "/dev/lp0")
                kitchen_device_combobox->setCurrentIndex(0);
            else if (kitchen_device == "/dev/lp1")
                kitchen_device_combobox->setCurrentIndex(1);
            else if (kitchen_device == "/dev/ttyS0")
                kitchen_device_combobox->setCurrentIndex(2);
            else if (kitchen_device == "/dev/ttyS1")
                printer_device_combobox->setCurrentIndex(3);
            else if (kitchen_device == "/dev/ttyS2")
                printer_device_combobox->setCurrentIndex(4);
            else if (kitchen_device == "/dev/ttyS3")
                kitchen_device_combobox->setCurrentIndex(5);
            else if (kitchen_device == "/dev/ttyUSB0")
                kitchen_device_combobox->setCurrentIndex(6);
            else if (kitchen_device == "/dev/ttyUSB1")
                kitchen_device_combobox->setCurrentIndex(7);
            else if (kitchen_device == "/dev/ttyUSB2")
                kitchen_device_combobox->setCurrentIndex(8);
            else if (kitchen_device == "/dev/ttyUSB3")
                kitchen_device_combobox->setCurrentIndex(9);
            else{
                kitchen_device_combobox->insertItem(10, kitchen_device);
                kitchen_device_combobox->setCurrentIndex(10);
            }
        }
        else if ( kitchen_type == "ipp"){
            kitchen_type_combobox->setCurrentIndex(1);
            kitchen_lineedit->show();
            kitchen_device_combobox->hide();
            kitchen_lineedit->setText(printer_device);
        }
        else{
            kitchen_type_combobox->setCurrentIndex(0);
            kitchen_lineedit->hide();
            kitchen_device_combobox->show();
            kitchen_device_combobox->setCurrentIndex(0);
        }

        if (kitchen_char_length == CHAR_42){
            kitchen_length_42_radiobutton->setChecked(true);
        }
        else if (kitchen_char_length == CHAR_42){
            kitchen_length_40_radiobutton->setChecked(true);
        }

        preferred_kitchen_device = kitchen_device_combobox->currentIndex();
        preferred_kitchen_ipp_device = kitchen_device;
    }
}

void DevicesConfig::writeDevicesConfig(){
    writeCashboxConfig();
    writePrinterConfig();
}

void DevicesConfig::writePrinterConfig(){
    int index,i;
    bool has_kitchen_printer=false;
    QString printer_device,kitchen_device;
    QString printer_type,kitchen_type;
    QString printer_length,kitchen_length;
    XmlConfig *xml = 0;

    index = printer_type_combobox->currentIndex();
    switch ( index ){
    case 0:
        printer_type = "direct";
        i = printer_device_combobox->currentIndex();
        switch (i){
        case 0:
            printer_device = "/dev/lp0";
            break;
        case 1:
            printer_device = "/dev/lp1";
            break;
        case 2:
            printer_device = "/dev/ttyS0";
            break;
        case 3:
            printer_device = "/dev/ttyS1";
            break;
        case 4:
            printer_device = "/dev/ttyS2";
            break;
        case 5:
            printer_device = "/dev/ttyS3";
            break;
        case 6:
            printer_device = "/dev/ttyUSB0";
            break;
        case 7:
            printer_device = "/dev/ttyUSB1";
            break;
        case 8:
            printer_device = "/dev/ttyUSB2";
            break;
        case 9:
            printer_device = "/dev/ttyUSB3";
            break;
        default:
            printer_device = cashbox_device_combobox->currentText();
            break;
        }
        break;

    case 1:
        printer_type = "ipp";
        printer_device = printer_lineedit->text();
        break;
    }

    if (printer_length_42_radiobutton->isChecked())
        printer_length="42";
    else  if (printer_length_40_radiobutton->isChecked())
        printer_length="40";

    has_kitchen_printer = kitchen_groupbox->isChecked();

    index = kitchen_type_combobox->currentIndex();
    switch (index){
    case 0:
        kitchen_type = "direct";
        i = kitchen_device_combobox->currentIndex();
        switch (i){
        case 0:
            kitchen_device = "/dev/lp0";
            break;
        case 1:
            kitchen_device = "/dev/lp1";
            break;
        case 2:
            kitchen_device = "/dev/ttyS0";
            break;
        case 3:
            kitchen_device = "/dev/ttyS1";
            break;
        case 4:
            kitchen_device = "/dev/ttyS2";
            break;
        case 5:
            kitchen_device = "/dev/ttyS3";
            break;
        case 6:
            kitchen_device = "/dev/ttyUSB0";
            break;
        case 7:
            kitchen_device = "/dev/ttyUSB1";
            break;
        case 8:
            kitchen_device = "/dev/ttyUSB2";
            break;
        case 9:
            kitchen_device = "/dev/ttyUSB3";
            break;
        default:
            kitchen_device = cashbox_device_combobox->currentText();
            break;
        }
        break;
    case 1:
        kitchen_type = "ipp";
        kitchen_device = kitchen_lineedit->text();
        break;
    }

    if (kitchen_length_42_radiobutton->isChecked())
        kitchen_length="42";
    else if (kitchen_length_40_radiobutton->isChecked())
        kitchen_length="40";

    //write dcopprinter_config.xml configuration

    xml = new XmlConfig(DCOPPRINTER_CONFIG);
    if (!xml->isValid()){
        delete xml;
        return;
    }

    if (xml->howManyTags("main")){
        xml->deleteElement("main");
    }
    xml->createElementSetDomain("main");
    xml->createElement("type",printer_type);
    xml->createElement("device",printer_device);
    xml->createElement("length",printer_length);
    xml->releaseDomain("main");

    if (has_kitchen_printer){
        if (xml->howManyTags("kitchen")){
            xml->deleteElement("kitchen");
        }
        xml->createElementSetDomain("kitchen");
        xml->createElement("type",kitchen_type);
        xml->createElement("device",kitchen_device);
        xml->createElement("length",kitchen_length);
        xml->releaseDomain("kitchen");
    }
    else{
        xml->delDomain();
        xml->deleteElement("kitchen");
    }
    xml->save();

    prepareMainTickets(printer_length);
    prepareKitchenTickets(kitchen_length);
}

void DevicesConfig::prepareMainTickets(const QString& text){
    QString xml_file;
    QString xml_string;
    XmlConfig *xml = 0;
    QString company,cif,address,localidad,provincia,telephone,head,foot,bye;
    bool is_checked = true;

    if (text == "42")
        xml_file = QString(TEMPLATES_LOCATION) + "printerz.xml";
    else if (text == "40")
        xml_file = QString(TEMPLATES_LOCATION) + "printerz_40.xml";

    xml = new XmlConfig(xml_file);
    xml->save("/etc/ntpv/printerz.xml");
    delete xml;

    if (text == "42")
        xml_file = QString(TEMPLATES_LOCATION) + "printertickettotal.xml";
    else if (text == "40")
        xml_file = QString(TEMPLATES_LOCATION) + "printertickettotal_40.xml";

    xml = new XmlConfig(xml_file);
    xml->save("/etc/ntpv/printertickettotal.xml");
    delete xml;


    xml = new XmlConfig(SAVED_CONFIG);
    xml->setDomain("ticket_values");
    company = xml->readString("company");
    cif = xml->readString("cif");
    address = xml->readString("address");
    localidad = xml->readString("localidad");
    provincia = xml->readString("provincia");
    telephone = xml->readString("telephone");
    head = xml->readString("head");
    foot = xml->readString("foot");
    bye = xml->readString("bye");
    if (xml->readString("showtaxes") == "true")
        is_checked = true;
    else
        is_checked = false;
    delete xml;



    if (text == "42")
        xml_file = QString(TEMPLATES_LOCATION) + "template_ticket.xml";
    else if (text == "40")
        xml_file = QString(TEMPLATES_LOCATION) + "template_ticket_40.xml";

    xml = new XmlConfig(xml_file);
    xml_string = xml->toString();
    delete xml;

    xml_string.replace("%COMPANY%",company);
    xml_string.replace("%CIF%",cif);
    xml_string.replace("%ADDRESS%",address);
    xml_string.replace("%LOCALIDAD%",localidad);
    if (!provincia.isEmpty())
        xml_string.replace("%PROVINCIA%","("+provincia+")");
    else
        xml_string.replace("%PROVINCIA%","");
    xml_string.replace("%TELEPHONE%",telephone);
    xml_string.replace("%HEAD%",head);
    xml_string.replace("%FOOT%",foot);
    xml_string.replace("%BYE%",bye);
    if (is_checked){
        QString aux;
        aux="<b>"+tr("Base imponible: ")+"</b>";
        aux+="<normal>%price_no_tax%</normal><tr>";
        aux+="<td width=\"13\" values=\"2\"><b>IVA 4% </b><normal>%iva4%</normal></td>";
        aux+="<td width=\"13\" values=\"2\"><b>IVA 7% </b><normal>%iva7%</normal></td>";
        aux+="<td width=\"13\" values=\"2\"><b>IVA 16% </b><normal>%iva16%</normal></td>";
        aux+="</tr><br/>";
        xml_string.replace("%TAX%",aux);
    }
    else{
        QString aux;
        aux="<b>"+tr("Impuestos incluidos")+"</b>";
        xml_string.replace("%TAX%",aux);
    }

    xml = new XmlConfig();
    xml->readXmlFromString(xml_string);
    if (xml->isValid()){
        xml->save(MAIN_TICKET_FILE);
    }
    delete xml;


    if (text == "42")
        xml_file = QString(TEMPLATES_LOCATION) + "template_invoice.xml";
    else if (text == "40")
        xml_file = QString(TEMPLATES_LOCATION) + "template_invoice_40.xml";

    xml = new XmlConfig(xml_file);
    xml_string = xml->toString();
    delete xml;

    xml_string.replace("%COMPANY%",company);
    xml_string.replace("%CIF%",cif);
    xml_string.replace("%ADDRESS%",address);
    xml_string.replace("%LOCALIDAD%",localidad);
    if (!provincia.isEmpty())
        xml_string.replace("%PROVINCIA%","("+provincia+")");
    else
        xml_string.replace("%PROVINCIA%","");
    xml_string.replace("%TELEPHONE%",telephone);
    xml_string.replace("%HEAD%",head);
    xml_string.replace("%FOOT%",foot);
    xml_string.replace("%BYE%",bye);
    if (is_checked){
        QString aux;
        aux="<b>"+tr("Base imponible: ")+"</b>";
        aux+="<normal>%price_no_tax%</normal><tr>";
        aux+="<td width=\"13\" values=\"2\"><b>IVA 4% </b><normal>%iva4%</normal></td>";
        aux+="<td width=\"13\" values=\"2\"><b>IVA 7% </b><normal>%iva7%</normal></td>";
        aux+="<td width=\"13\" values=\"2\"><b>IVA 16% </b><normal>%iva16%</normal></td>";
        aux+="</tr><br/>";
        xml_string.replace("%TAX%",aux);
    }
    else{
        QString aux;
        aux="<b>"+tr("Impuestos incluidos")+"</b>";
        xml_string.replace("%TAX%",aux);
    }

    xml = new XmlConfig();
    xml->readXmlFromString(xml_string);
    if (xml->isValid()){
        xml->save(MAIN_INVOICE_FILE);
    }
    delete xml;

}

void DevicesConfig::prepareKitchenTickets(const QString& text){
    QString xml_file;
    QString xml_string;
    XmlConfig *xml = 0;

    if (text == "42")
        xml_file = QString(TEMPLATES_LOCATION) + "kitchen_printerhtml.xml";
    else if (text == "40")
        xml_file = QString(TEMPLATES_LOCATION) + "kitchen_printerhtml_40.xml";

    xml = new XmlConfig(xml_file);
    xml->save("/etc/ntpv/kitchen_printerhtml.xml");
    delete xml;
}

void DevicesConfig::writeCashboxConfig(){
    int index;
    QString type;
    QString device;
    XmlConfig *xml = 0;

    //get type
    index =cashbox_type_combobox->currentIndex();
    switch (index){
    case 0:
        type = "serial";
        break;
    case 1:
        type = "cash_drawer";
        break;
    default:
        type = "cash_drawer";
        break;
    }

    //get the device
    index = cashbox_device_combobox->currentIndex();
    switch (index){
    case 0:
        device = "/dev/ttyS0";
        break;
    case 1:
        device = "/dev/ttyS1";
        break;
    case 2:
        device = "/dev/ttyS2";
        break;
    case 3:
        device = "/dev/ttyS3";
        break;
    case 4:
        device = "/dev/lp0";
        break;
    case 5:
        device = "/dev/lp1";
        break;
    case 6:
        device = "/dev/ttyUSB0";
        break;
    case 7:
        device = "/dev/ttyUSB1";
        break;
    case 8:
        device = "/dev/ttyUSB2";
        break;
    case 9:
        device = "/dev/ttyUSB3";
        break;
    default:
        device = cashbox_device_combobox->currentText();
        break;
    }


    xml = new XmlConfig(DEVICES_CONFIG);
    if (!xml->isValid()){
        delete xml;
        return;
    }
    if (!xml->setDomain("cashbox")){
        xml->createElementSetDomain("cashbox");
    }
    xml->doWrite("type",type);
    xml->doWrite("dev",device);
    xml->save();
    delete xml;
}

void DevicesConfig::acceptSlot(){
    writeDevicesConfig();
    ok_button->setEnabled(false);

}

void DevicesConfig::printerTypeChanged(int index){
    //local printer
    if (index == 0){
        printer_device_combobox->show();
        printer_lineedit->hide();
        printer_device_combobox->setCurrentIndex(preferred_printer_device);
    }
    //ipp printer
    else if (index == 1){
        printer_device_combobox->hide();
        printer_lineedit->show();
        printer_lineedit->setText(preferred_printer_ipp_device);
    }
    ok_button->setEnabled(true);
}

void DevicesConfig::kitchenTypeChanged(int index){
    //local printer
    if (index == 0){
        kitchen_device_combobox->show();
        kitchen_lineedit->hide();
        kitchen_device_combobox->setCurrentIndex(preferred_kitchen_device);
    }
    //ipp printer
    else if (index == 1){
        kitchen_device_combobox->hide();
        kitchen_lineedit->show();
        kitchen_lineedit->setText(preferred_kitchen_ipp_device);
    }
    ok_button->setEnabled(true);
}

void DevicesConfig::printerDeviceChanged(int index){
    preferred_printer_device = index;
    ok_button->setEnabled(true);
}

void DevicesConfig::kitchenDeviceChanged(int index){
    preferred_kitchen_device=index;
    ok_button->setEnabled(true);
}

void DevicesConfig::printerIppDeviceChanged(const QString& text){
    preferred_printer_ipp_device = text;
    ok_button->setEnabled(true);
}

void DevicesConfig::kitchenIppDeviceChanged(const QString& text){
    preferred_kitchen_ipp_device = text;
    ok_button->setEnabled(true);
}

void DevicesConfig::somethingChanged(bool enable){
    ok_button->setEnabled(true);
}

void DevicesConfig::somethingChanged(int index){
    ok_button->setEnabled(true);
}

void DevicesConfig::showEvent(QShowEvent *e){
    ok_button->setEnabled(true);
    readDevicesConfig();
    QWidget::showEvent(e);
}
