/***************************************************************************
                          printermanager.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Pablo Fernandez
modified by Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include <libbslxml/xmlconfig.h>
#include <wposcore/config.h>
#include <QFile>

#ifndef _WINDOWS
#include <cups/cups.h>
#endif

#include <iostream>
#include <QStringList>

#include "printermanager.h"
#include "printerticket.h"

using  std::string;

static const QString XML_TICKET_HTML_PATH       { cfg::xmlFileByKey(cfg::XMLKey::PrinterHtml) };
static const QString XML_INVOICE_HTML_PATH      { cfg::xmlFileByKey(cfg::XMLKey::Invoice) };
static const QString XML_KITCHEN_HTML_PATH      { cfg::xmlFileByKey(cfg::XMLKey::Kitchen) };
static const QString XML_Z_HTML_PATH            { cfg::xmlFileByKey(cfg::XMLKey::PrinterZeta) };
static const QString XML_TICKET_TOTAL_HTML_PATH { cfg::xmlFileByKey(cfg::XMLKey::PrinterTotal) };

static const QString TMP_PRINTER                 {"/tmp/printermanager_products.xml"};
static const QString TMP_PRINTER_PAYTYPES        {"/tmp/printermanager_paytypes.xml"};
static const QString TMP_PRINTER_CAMAREROS       {"/tmp/printermanager_camareros.xml"};
static const QString TMP_PRINTER_OFFERS          {"/tmp/printermanager_offers.xml"};
static const QString TMP_PRINTER_TOTAL_OFFERS    {"/tmp/printermanager_total_offers.xml"};
static const QString TMP_PRINTER_PROD            {"/tmp/print.xml"};
static const QString TMP_PRINTER_DATA            {"/tmp/printertmpvalue.xml"};
static const QString TMP_PRINTER_TEMPLATE        {"/tmp/printertmptemplate.xml"};
static const QString TMP_PRINTER_TMPFILE         {"/tmp/printertmp.xml"};

static const QString PL_SHIT                     {"/usr/share/dcopprinter/replace.pl"};

static const QString DEFAULT_TYPE                {"direct"};
static const QString DEFAULT_DEVICE              {"/dev/lp0"};

PrinterManager::PrinterManager(
    const QString& _type,
    const QString& _device,
    const QString& _user,
    const QString& _passwd)
    :type {DEFAULT_TYPE},
    device { DEFAULT_DEVICE},
    user {_user},
    passwd {_passwd}
{

    if (_type == "ipp") type = _type;    
    device = _device;
    if (_device.isEmpty()){
        if (type == "ipp")

#ifndef _WINDOWS
        device = QString(cupsGetDefault())
#endif
            ;
    }
}

PrinterManager::~PrinterManager() {}

void PrinterManager::setXmlPartHTML (XmlConfig* xml) {
    delete xml_html;
    xml_html = xml;
}

void PrinterManager::setXmlPartData (XmlConfig* xml) {
    delete xml_data;
    xml_data = xml;
}

void PrinterManager::printTicket (
    XmlConfig* xml,
    int times,
    bool print,
    bool dont_change_html)
{
    int i;
    bool finish = false;
    QString tag;
    QString path;
    QString value;

    action = "ticket";
    if ( !dont_change_html ) {
        setXmlPartHTML (new XmlConfig (XML_TICKET_HTML_PATH));
    }

    xml->pushDomain();
    xml->delDomain();

    delete xml_data;
    xml_data = new XmlConfig();

    for (i = 0; !finish; i++) {
        switch (i) {
        case 0:
            tag  = "employee";
            path = "employee.name";
            break;
        case 1:
            tag  = "lounge";
            path = "location.lounge";
            break;
        case 2:
            tag  = "table";
            path = "location.table";
            break;
        case 3:
            tag  = "timestamp";
            path = "timestamps.last";
            break;
        case 4:
            tag  = "ticketnumber";
            path = "ticketnumber";
            break;
        default:
            finish = true;
            break;
        }
        if (finish)
            break;
        value = getValueFromXml(xml, path);
        addDataTag ("tickets", tag, value);
    }
    /* Copy the products part */
    addProductsTags (xml);
    addIVADataTags("tickets");
    if (print){
        this->print(times);
    }
    action = "";
}

void PrinterManager::printInvoice(XmlConfig* xml, int times, bool print, bool dont_change_html) {
    int i;
    bool finish = false;
    QString tag;
    QString path;
    QString value;

    action = "ticket";
    if (!dont_change_html) {
        setXmlPartHTML (new XmlConfig (XML_INVOICE_HTML_PATH));
    }
    xml->pushDomain();
    xml->delDomain();

    delete xml_data;
    xml_data = new XmlConfig();

    for (i = 0; !finish; i++) {
        switch (i) {
        case 0:
            tag  = "employee";
            path = "employee.name";
            break;
        case 1:
            tag  = "lounge";
            path = "location.lounge";
            break;
        case 2:
            tag  = "table";
            path = "location.table";
            break;
        case 3:
            tag  = "timestamp";
            path = "timestamps.last";
            break;
        case 4:
            tag  = "ticketnumber";
            path = "ticketnumber";
            break;
        case 5:
            tag  = "invoicenumber";
            path = "invoicenumber";
            break;
        default:
            finish = true;
            break;
        }
        if (finish)
            break;
        value = getValueFromXml (xml, path);
        addDataTag("tickets", tag, value);
    }
    /* Copy the products part */
    addProductsTags(xml);
    addIVADataTags("tickets");
    if (print){
        this->print(times);
    }
    action = "";
}

void PrinterManager::printKitchenOrder(XmlConfig* xml, bool print, bool dont_change_html){
    int i;
    bool finish = false;
    QString tag;
    QString path;
    QString value;

    action = "ticket";
    if (!dont_change_html) {
        setXmlPartHTML (new XmlConfig (XML_KITCHEN_HTML_PATH));
    }
    xml->pushDomain();
    xml->delDomain();

    delete xml_data;
    xml_data = new XmlConfig();

    for (i = 0; !finish; i++) {
        switch (i) {
        case 0:
            tag  = "employee";
            path = "employee.name";
            break;
        case 1:
            tag  = "lounge";
            path = "location.lounge";
            break;
        case 2:
            tag  = "table";
            path = "location.table";
            break;
        case 3:
            tag  = "timestamp";
            path = "timestamps.last";
            break;
        case 4:
            tag  = "ticketnumber";
            path = "ticketnumber";
            break;
        case 5:
            tag  = "invoicenumber";
            path = "invoicenumber";
            break;
        default:
            finish = true;
            break;
        }
        if (finish)
            break;
        value = getValueFromXml (xml, path);
        addDataTag("tickets", tag, value);
    }
    /* Copy the products part */
    addProductsTags(xml,true);
    //        addIVADataTags("tickets");
    if (print){
        this->print();
    }
    action = "";

}

//void PrinterManager::printTicketSlot (XmlConfig* xml) {
//  printTicket (xml);
//}

void PrinterManager::printX (XmlConfig* xml, bool print, bool dont_change_html) {
    printXZ (xml, "X", print, dont_change_html);
}

void PrinterManager::printZ (XmlConfig* xml, bool print, bool dont_change_html) {
    printXZ (xml, "Z", print, dont_change_html);
}

void PrinterManager::printTicketTotal (XmlConfig* xml, bool print, bool dont_change_html) {
    int i;
    QString tag;
    QString path;
    QString value;
    bool finish = false;

    action = "ticket_total";
    if (!dont_change_html) {
        setXmlPartHTML (new XmlConfig (XML_TICKET_TOTAL_HTML_PATH));
    }

    xml->pushDomain();
    xml->delDomain();

    /* Check if xml_data exists */
    delete xml_data;
    xml_data = new XmlConfig();

    for (i = 0; !finish; i++) {
        switch (i) {
        case 0:
            tag  = "total_in_cash";
            path = "money_at_cashbox";
            break;
        default:
            finish = true;
        }
        if (finish)
            break;
        value = getValueFromXml (xml, path);
        addDataTag ("tickets", tag, value);
    }
    /* Copy the products part */
    if (print)
        this->print();
    action = "";
}

void PrinterManager::printXZ (XmlConfig* xml, const QString& type, bool print, bool dont_change_html) {
    int i;
    bool finish = false;
    QString tag;
    QString path;
    QString value;

    action = "Z";

    if (!dont_change_html) {
        setXmlPartHTML (new XmlConfig (XML_Z_HTML_PATH));
    }


    xml->pushDomain();
    xml->delDomain();

    /* Check if xml_data exists */
    delete xml_data;
    xml_data = 0;
    xml_data = new XmlConfig();

    for (i = 0; !finish; i++) {
        switch (i) {
        case 0:
            tag  = "timestamp_first";
            path = "timestamps.first";
            break;
        case 1:
            tag  = "timestamp_last";
            path = "timestamps.last";
            break;
        case 2:
            tag  = "total";
            path = "main.total";
            break;
        default:
            finish = true;
        }
        if (finish)
            break;
        value = getValueFromXml (xml, path);
        addDataTag ("tickets", tag, value);
    }
    addDataTag ("tickets", "type", type);
    /* Copy the products part */
    addZProductsTags(xml);
    addPayTypesTags(xml);
    addCamarerosTags(xml);
    addTotalOffersTags(xml);
    addTotalOptionsTags(xml);
    if (print)
        this->print();
    action = "";
}

void PrinterManager::addDataTag (const QString& subsection, const QString& name, const QString &value) {
    int subsection_nth;

    xml_data->pushDomain();
    xml_data->delDomain();
    xml_data->setDomain("items");
    /* Find the subsection where we need to work */
    subsection_nth = findSubsection(subsection, true);
    /* Go there */
    xml_data->setDomain("item["+QString::number(subsection_nth)+"]");
    /* Add the tag */
    xml_data->createElementSetDomain("data", value);
    xml_data->createAttributeHere("name", name);
    xml_data->createAttributeHere("type", "string");
    xml_data->releaseDomain("data",false);
    xml_data->popDomain();
}

void PrinterManager::addProductsTags(XmlConfig* xml,bool printed_option) {
    int i, count;
    int j,aux_count;
    float fprice;
    float aux_price;
    bool already_printed = false;
    int iva;
    QString name, price;
    QString siva;
    QString iva_percentage;
    QString options;
    ProductData *productdata = 0;

    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("products");
    count = xml->howManyTags("product");
    products_list.clear ();

    for (i = 0; i < count; i++) {
        xml->pushDomain();
        xml->setDomain("product["+QString::number(i)+"]");
        name = xml->readString("name");
        aux_price = (xml->readString("billing.price")).toFloat();
        price = QString::number(aux_price,'f',2);
        iva_percentage = xml->readString("billing.tax");
        siva = xml->readAttribute("billing.tax", "type");
        if (siva == "iva4")
            iva = 4;
        else if (siva == "iva7")
            iva = 7;
        else
            iva = 16;
        if (xml->howManyTags("printer")){
            already_printed = false;
            //FIXME   at the moment only the kitchen attribute is set at printer.
            aux_count = xml->howManyAttributes("printer");
            for (j=0;j<aux_count;j++){
                QString name;
                QString value;
                name = xml->readAttributeName("printer",j);
                value = xml->readAttribute("printer",name);
                if ((name == "printed") && (value == "true"))
                    already_printed = true;
            }
        }
        if (xml->howManyTags("options")){
            options = "";
            xml->setDomain("options");
            aux_count = xml->howManyTags("option");
            for(j=0;j<aux_count;j++){
                options+= xml->readString("option["+QString::number(j)+"].value");
                if (j!=(aux_count-1))
                    options+= ",";
            }
            xml->releaseDomain("options");
        }
        else
            options ="";

        if (!printed_option)
            products_list.addProduct(name, price, iva, iva_percentage.toFloat());
        else
            products_list.addProduct(name, price, iva, iva_percentage.toFloat (),options ,already_printed);

        xml->popDomain();
    }

    /* Now add the tags */
    delete products;
    products = new XmlConfig();
    count = products_list.length ();
    for (i = 0; i < count; i++) {
        productdata = products_list.productNth(i);
        fprice =(float) (productdata->quantity) * ((productdata->price).toFloat());
        products->createElementSetDomain("product");
        products->createAttributeHere("description", products_list.productNth(i)->name);
        products->createAttributeHere("unitPrice", products_list.productNth(i)->price);
        if (productdata->price.toFloat() < 0)
            products->createAttributeHere("quantity", QString::number((-1)*(products_list.productNth(i)->quantity)));
        else
            products->createAttributeHere("quantity", QString::number(products_list.productNth(i)->quantity));
        products->createAttributeHere("price", QString::number (fprice,'f',2));
        products->createAttributeHere("product_options", products_list.productNth(i)->options);
        if (products_list.productNth(i)->already_printed)
            products->createAttributeHere("printed","***");
        else
            products->createAttributeHere("printed","   ");

        products->releaseDomain("product");
    }
    xml->popDomain();
}

void PrinterManager::addPayTypesTags(XmlConfig* xml) {
    int i, count;
    QStringList name, num_tickets, total_income;

    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("main");

    count = xml->howManyTags("paytype");
    for (i = 0; i < count; i++) {
        xml->pushDomain();
        xml->setDomain("paytype["+QString::number(i)+"]");
        name << xml->readString("name");
        num_tickets << xml->readString("num_tickets");
        total_income << xml->readString("total_income");
        xml->popDomain();
    }

    count = name.count ();

    delete paytypes;
    paytypes = new XmlConfig();

    for (i = 0; i < count; i++) {
        paytypes->createElementSetDomain("paytype");
        paytypes->createAttributeHere("name", name[i]);
        paytypes->createAttributeHere("num_tickets", num_tickets[i]);
        paytypes->createAttributeHere("total_income", total_income[i]);
        paytypes->releaseDomain("paytype");
    }
    xml->popDomain();
}

void PrinterManager::addCamarerosTags(XmlConfig* xml) {
    int i, count;
    int total_invitations;
    int nc;
    QStringList name, id, num_tickets, total_income,cashbox_open, total_invitations_list;
    QString employee_name,aux;

    /* Delete the offers */
    delete offers;
    offers = new XmlConfig();

    delete options;
    options = new XmlConfig();

    xml->pushDomain();
    xml->delDomain();

    xml->setDomain("employees");
    count = xml->howManyTags("employee");

    /* Get the info */
    for (i = 0; i < count; i++) {
        xml->pushDomain();
        xml->setDomain("employee["+QString::number(i)+"]");
        employee_name = xml->readString("name");
        name.append(employee_name);
        aux =xml->readString("id");
        id.append(aux);
        aux = xml->readString("num_tickets");
        num_tickets.append(aux);
        aux = xml->readString("total_income");
        total_income.append(aux);
        aux = xml->readString("event_count");
        cashbox_open.append(aux);

        /* Get the invitations */
        xml->pushDomain ();
        xml->setDomain ("offers");

        total_invitations = 0;
        nc = xml->howManyTags ("offer");
        for (int m = 0; m < nc; m++)
            total_invitations += xml->readInt ("offer["+QString::number (m)+"].count");
        xml->popDomain ();
        total_invitations_list << QString::number (total_invitations);
        addOffersTags(xml, employee_name);
        addOptionsTags(xml, employee_name);
        xml->popDomain();
    }

    /* Now add the tags */
    delete camareros;
    camareros = new XmlConfig();
    count = name.count ();
    for (i = 0; i < count; i++) {
        camareros->createElementSetDomain("employee");
        camareros->createAttributeHere("name", name[i]);
        camareros->createAttributeHere("id", id[i]);
        camareros->createAttributeHere("num_tickets", num_tickets[i]);
        camareros->createAttributeHere("total_income", total_income[i]);
        camareros->createAttributeHere("total_invitations", total_invitations_list[i]);
        camareros->createAttributeHere("cashbox_open", cashbox_open[i]);
        camareros->releaseDomain("employee");
    }
    xml->popDomain();
}

/**
*       the addOfferTag Xml is created at addCamarerosTags method.
*/

void PrinterManager::addOffersTags (XmlConfig* xml, const QString& employee_name) {
    int i, c;
    int offers_tags;
    QString type, name, count;

    xml->pushDomain ();
    xml->setDomain ("offers");
    offers_tags = xml->howManyTags ("offer");
    c = offers->howManyTags ("offer");
    for (i = 0; i < offers_tags; i++) {
        type = xml->readString ("offer["+QString::number (i)+"].type");
        name = xml->readString ("offer["+QString::number (i)+"].name");
        count = xml->readString ("offer["+QString::number (i)+"].count");
        offers->createElementSetDomain("offer");
        offers->createAttributeHere("employee", employee_name);
        offers->createAttributeHere("type", type);
        offers->createAttributeHere("name", name);
        offers->createAttributeHere("count", count);
        offers->releaseDomain("offer");
    }
    xml->popDomain ();
}

/**
*       the addOptionTag Xml is created at addCamarerosTags method.
*/

void PrinterManager::addOptionsTags (XmlConfig* xml, const QString& employee_name) {
    int i, c;
    int options_tags;
    QString type, name, count;

    xml->pushDomain ();
    xml->setDomain ("options");
    options_tags = xml->howManyTags ("option");
    c = options->howManyTags ("option");
    for (i = 0; i < options_tags; i++) {
        type = xml->readString ("option["+QString::number (i)+"].type");
        name = xml->readString ("option["+QString::number (i)+"].name");
        count = xml->readString ("option["+QString::number (i)+"].amount");
        options->createElementSetDomain ("option");
        options->createAttributeHere("employee", employee_name);
        options->createAttributeHere("type", type);
        options->createAttributeHere("name", name);
        options->createAttributeHere("count", count);
        options->releaseDomain("option");
    }
    xml->popDomain ();
}

void PrinterManager::addTotalOffersTags(XmlConfig* xml) {
    int i, c;
    QStringList type, name, count;

    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("offers");
    c = xml->howManyTags("offer");
    for (i = 0; i < c; i++) {
        xml->pushDomain();
        xml->setDomain("offer["+QString::number(i)+"]");
        type << xml->readString("type");
        name << xml->readString("name");
        count << xml->readString("count");
        xml->popDomain();
    }

    c = name.count ();

    delete total_offers;
    total_offers = new XmlConfig();

    for (i = 0; i < c; i++) {
        total_offers->createElementSetDomain("offer");
        total_offers->createAttributeHere("type", type[i]);
        total_offers->createAttributeHere("name", name[i]);
        total_offers->createAttributeHere("count", count[i]);
        total_offers->releaseDomain("offer");
    }
    xml->popDomain();
}

void PrinterManager::addTotalOptionsTags(XmlConfig* xml) {
    int i, c;
    QStringList type, name, count;

    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("options");
    c = xml->howManyTags("option");
    for (i = 0; i < c; i++) {
        xml->pushDomain();
        xml->setDomain("option["+QString::number(i)+"]");
        type << xml->readString("type");
        name << xml->readString("name");
        count << xml->readString("amount");
        xml->popDomain();
    }

    c = name.count ();

    delete total_options;
    total_options = new XmlConfig();

    for (i = 0; i < c; i++) {
        total_options->createElementSetDomain("option");
        total_options->createAttributeHere("type", type[i]);
        total_options->createAttributeHere("name", name[i]);
        total_options->createAttributeHere("amount", count[i]);
        total_options->releaseDomain("option");
    }
    xml->popDomain();
}

void PrinterManager::addZProductsTags(XmlConfig* xml) {
    int i, count;
    QStringList name, units, total_income;

    xml->pushDomain();
    xml->delDomain();
    xml->setDomain("products");
    count = xml->howManyTags("product");
    for (i = 0; i < count; i++) {
        xml->pushDomain();
        xml->setDomain("product["+QString::number(i)+"]");
        name << xml->readString("name");
        units << xml->readString("units");
        total_income << xml->readString("total_income");
        xml->popDomain();
    }

    /* Now add the tags */
    delete products;
    products = new XmlConfig();
    count = name.count ();
    for (i = 0; i < count; i++) {
        products->createElementSetDomain("zproduct");
        products->createAttributeHere("name", name[i]);
        products->createAttributeHere("units", units[i]);
        products->createAttributeHere("total_income", total_income[i]);
        products->releaseDomain("zproduct");
    }
    xml->popDomain();
}

void PrinterManager::addPriceDataTag (const QString& subsection) {
    float total_price = 0;
    int subsection_nth;
    int i, count;
    float price;
    float quantity;
    float iprice;

    count = products_list.length ();
    for (i = 0; i < count; i++) {
        price = products_list.productNth(i)->price.toFloat();
        quantity = products_list.productNth(i)->quantity;
        iprice = (float) (price * quantity);
        total_price += iprice;
    }

    xml_data->pushDomain();
    xml_data->delDomain();
    xml_data->setDomain("items");

    /* Find the subsection where we need to work */
    subsection_nth = findSubsection(subsection, true);

    /* Go there */
    xml_data->setDomain("item["+QString::number(subsection_nth)+"]");

    /* Add the tag */
    xml_data->createElementSetDomain("data", QString::number(total_price));
    xml_data->createAttributeHere("name", "total_price");
    xml_data->createAttribute("type", "string");
    xml_data->releaseDomain("data",false);
    xml_data->popDomain();
}

void PrinterManager::addIVADataTags (const QString& subsection) {
    int subsection_nth;
    int i, count;
    float price;
    float quantity;
    float iva4 = 0;
    float iva7 = 0;
    float iva16 = 0;
    float total_price = 0;
    float price_no_tax = 0;
    float pq;
    ProductData *product = 0;

    count = products_list.length ();
    for (i = 0; i < count; i++) {
        product = products_list.productNth(i);
        price = product->price.toFloat();
        quantity = product->quantity;
        pq = (float) (price * quantity);
        total_price += pq;
        switch (product->iva) {
        case 4:
            iva4 += (float) pq * product->iva_percentage;
            break;
        case 7:
            iva7 += (float) pq * product->iva_percentage;
            break;
        default:
        case 16:
            iva16 += (float) pq * product->iva_percentage;
            break;
        }
    }

    price_no_tax = total_price - (iva4+iva7+iva16);
    xml_data->pushDomain();
    xml_data->delDomain();
    xml_data->setDomain("items");

    /* Find the subsection where we need to work */
    subsection_nth = findSubsection(subsection, true);

    /* Go there */
    xml_data->setDomain("item["+QString::number(subsection_nth)+"]");
    /* Add the tags */

    xml_data->createElementSetDomain("data", QString::number(iva4,'f',2));
    xml_data->createAttributeHere("name", "iva4");
    xml_data->createAttributeHere("type", "float");
    xml_data->releaseDomain("data",false);

    xml_data->createElementSetDomain("data", QString::number(iva7,'f',2));
    xml_data->createAttributeHere("name", "iva7");
    xml_data->createAttributeHere("type", "float");
    xml_data->releaseDomain("data",false);

    xml_data->createElementSetDomain("data", QString::number(iva16,'f',2));
    xml_data->createAttributeHere("name", "iva16");
    xml_data->createAttributeHere("type", "float");
    xml_data->releaseDomain("data",false);

    xml_data->createElementSetDomain("data", QString::number(price_no_tax,'f',2));
    xml_data->createAttributeHere("name", "price_no_tax");
    xml_data->createAttributeHere("type", "float");
    xml_data->releaseDomain("data",false);

    xml_data->createElementSetDomain("data", QString::number(total_price,'f',2));
    xml_data->createAttributeHere("name", "total_price");
    xml_data->createAttributeHere("type", "float");
    xml_data->releaseDomain("data",false);

    xml_data->popDomain();
}

int PrinterManager::findSubsection(const QString& subsection, bool create) {
    int i, count;

    count = xml_data->howManyTags("item");
    for (i = 0; i < count; i++) {
        if (xml_data->readAttribute("item["+QString::number(i)+"]", "name") == subsection)
            return i;
    }

    if (create) {
        xml_data->createElement("item");
        i = xml_data->howManyTags("item");
        if (i)
            i--;
        xml_data->createAttribute("item["+QString::number(i)+"]", "name", subsection);
        return i;
    }

    return -1;
}

QString PrinterManager::getValueFromXml (XmlConfig* xml, const QString& path) {
    QString value;

    xml->pushDomain();
    xml->delDomain();
    value = xml->readString(path);
    xml->popDomain();
    return value;
}

void PrinterManager::debug () {
    std::cout << "--[ HTML ]--------------------\n";
    if (xml_html) xml_html->debug();

    std::cout << "--[ DATA ]--------------------\n";
    if (xml_data) xml_data->debug();

    std::cout << "--[ PRODUCTS ]--------------------\n";
    if (products)        products->debug();
}

void PrinterManager::print(int copies) {
    XmlConfig* xml_aux = 0;
    xml_aux = preparePrinting();
    if (xml_aux){
        realPrint(xml_aux, copies);
        delete xml_aux;
    }
}

XmlConfig* PrinterManager::preparePrinting(){
    XmlConfig *all = 0;

    all = new XmlConfig ();
    all->createElement("ticket");
    all->createElement("ticket.sections");
    all->createElement("ticket.sections.section");
    all->createAttribute("ticket.sections.section", "name", "printerRepresentation");
    all->createElement("ticket.sections.section.tickets");

    all->createElement("ticket.sections.section");
    all->createAttribute("ticket.sections.section[1]", "name", "definitions");
    all->createElement("ticket.sections.section[1].items");

    if (action == "Z") {
        all->createElement("ticket.sections.section[1].items.item");
        all->createAttribute("ticket.sections.section[1].items.item", "name", "zproducts");
        all->createElement("ticket.sections.section[1].items.item[1]");
        all->createAttribute("ticket.sections.section[1].items.item[1]", "name", "paytypes");
        all->createElement("ticket.sections.section[1].items.item[2]");
        all->createAttribute("ticket.sections.section[1].items.item[2]", "name", "employees");
        all->createElement("ticket.sections.section[1].items.item[3]");
        all->createAttribute("ticket.sections.section[1].items.item[3]", "name", "offers");
        all->createElement("ticket.sections.section[1].items.item[4]");
        all->createAttribute("ticket.sections.section[1].items.item[4]", "name", "options");
        all->createElement("ticket.sections.section[1].items.item[5]");
        all->createAttribute("ticket.sections.section[1].items.item[5]", "name", "total_offers");
        all->createElement("ticket.sections.section[1].items.item[6]");
        all->createAttribute("ticket.sections.section[1].items.item[6]", "name", "total_options");
    }
    else if (action == "ticket") {
        all->createElement("ticket.sections.section[1].items.item");
        all->createAttribute("ticket.sections.section[1].items.item", "name", "command");
    }

    copy(xml_html, &all, "ticket.sections.section.tickets.");
    copy(xml_data, &all, "ticket.sections.section[1].items.");
    if (products)
        copy(products, &all, "ticket.sections.section[1].items.item[0].");

    if (action == "Z") {
        copy(paytypes, &all, "ticket.sections.section[1].items.item[1].");
        copy(camareros, &all, "ticket.sections.section[1].items.item[2].");
        copy(offers, &all, "ticket.sections.section[1].items.item[3].");
        copy(options, &all, "ticket.sections.section[1].items.item[4].");
        copy(total_offers, &all, "ticket.sections.section[1].items.item[5].");
        copy(total_options, &all, "ticket.sections.section[1].items.item[6].");
    }
    //        all->save(TMP_PRINTER_PROD);
    //        pt.parse(TMP_PRINTER_PROD);
    //        pt.parse(all);
    //        delete all;

    delete camareros;
    camareros = 0;
    delete products;
    products = 0;
    delete offers;
    offers = 0;
    delete options;
    options = 0;
    delete total_offers;
    total_offers = 0;
    delete total_options;
    total_options = 0;
    delete paytypes;
    paytypes =0;
    return all;
}

void PrinterManager::realPrint(XmlConfig* xml, int copies){

    PrinterTicket pt(type, device, user, passwd);
    for (auto i=0; i< copies; i++)
        pt.parse(xml);
}

/**
 * Copy everything from xml in dest in the domain specified by domain.
 * Pointer to pointer because I need to return a new XmlConfig.
 **/

void PrinterManager::copy (XmlConfig* xml, XmlConfig **dest, QString domain) {
    QFile *file = new QFile(TMP_PRINTER_DATA);
    if (file->exists())
        file->remove();
    delete file;

    file = new QFile(TMP_PRINTER_TEMPLATE);
    if (file->exists())
        file->remove();
    delete file;

    xml->save (TMP_PRINTER_DATA);
    (*dest)->createElement (domain+QString("REPLACE"));
    (*dest)->save (TMP_PRINTER_TEMPLATE);

    //@benes system (PL_SHIT" " TMP_PRINTER_TEMPLATE" " TMP_PRINTER_DATA " > " TMP_PRINTER_TMPFILE);
    delete (*dest);

    (*dest) = new XmlConfig (TMP_PRINTER_TMPFILE);
}

/************************
 **** LIST MANAGEMENT ***
 ************************/

void ProductDataList::addProduct (
    const QString& name,
    const QString& price,
    int iva,
    float iva_percentage,
    const QString& options,
    bool _already_printed)
{
    int i, count;
    bool ok;
    QString p = price;
    ProductData *data;

    if (!p.toFloat (&ok) || !ok)
        p = "0";

    count = list.count();

    for (i = 0; i < count; i++) {
        data = list.at(i);
        if ((data->name == name) &&
            (data->price == p) &&
            (data->options == options) &&
            (_already_printed == data->already_printed))
        {
            data->quantity++;
            return;
        }
    }

    data = new ProductData;
    data->name = name;
    data->price = p;
    data->quantity = 1;
    data->iva = iva;
    data->iva_percentage = iva_percentage;
    data->already_printed = _already_printed;
    data->options = options;
    list.append (data);
}

void ProductDataList::addProduct (const QString& name,
                                 const QString& price,
                                 int iva,
                                 float iva_percentage)
{
    int i, count;
    bool ok;
    QString p = price;
    ProductData *data;

    if (!p.toFloat (&ok) || !ok)
        p = "0";

    count = list.count();

    for (i = 0; i < count; i++) {
        data = list.at(i);
        if ((data->name == name) && (data->price == p)){
            data->quantity++;
            return;
        }
    }

    data = new ProductData;
    data->name = name;
    data->price = p;
    data->quantity = 1;
    data->iva = iva;
    data->iva_percentage = iva_percentage;
    data->already_printed = false;
    data->options = "";
    list.append (data);
}

ProductData* ProductDataList::productNth(int i) {
    return list.at(i);
}

int ProductDataList::length() {
    return list.count();
}

void ProductDataList::clear() {
    list.clear ();
}
