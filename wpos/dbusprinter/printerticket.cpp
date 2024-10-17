/***************************************************************************
                          ticket.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Pablo Fernandez
modified by Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "printersamsungsrp350.h"

#include <libbslxml/xmlconfig.h>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QFile>

#include <iostream>
using namespace std;

#include "printerticket.h"

PrinterTicket::PrinterTicket(const QString& _type, const QString& _device,const QString& _user,const QString& _passwd):
    space_to_write(-1),
    values_to_write(-1),
    printersamsungsrp350(0)
{
    printersamsungsrp350 = new PrinterSamsungSRP350(_type, _device, _user, _passwd);
}

PrinterTicket::~PrinterTicket(){
    delete printersamsungsrp350;
}

void PrinterTicket::parse(const QString& path) {
    XmlConfig* xml = 0;
    xml = new XmlConfig(path);
    parse(xml);
    delete xml;
}

void PrinterTicket::parse(XmlConfig* xml) {
    QStringList list;
    QString subsection;
    int pos;
    int i, count;

    printersamsungsrp350->clean ();
    xml->pushDomain();
    xml->delDomain();
    if (!xml->setDomain("ticket.sections", true)) {
        xml->popDomain();
        return;
    }

    /* Find the printerRepresentation section */
    pos = findSection (xml, "printerRepresentation");
    if (pos < 0) {
        cerr << "Unable to find the printerRepresentation section" << endl;
        xml->popDomain();
        return;
    }

    /* We are in section printerRepresentation */
    xml->setDomain("section["+QString::number(pos)+"]");
    list = xml->findNode(QRegExp("*"), QString{}, false);
    count = list.count();

    for (i = 0; i < count; i++) {
        subsection = list[i];
        parseSubsection(xml, subsection);
    }
    xml->popDomain();
    printersamsungsrp350->print("/dev/lp0");
}

void PrinterTicket::parseSubsection(XmlConfig* xml, QString subsection) {
    int i, count;
    int j, attrcount;
    bool trproduct_processed = false;
    bool trpaytypes_processed = false;
    bool trcamareros_processed = false;
    bool trtotal_offers_processed = false;
    bool trtotal_options_processed = false;
    bool trzproducts_processed = false;
    QStringList list;
    QStringList attributes_names;
    QStringList attributes_values;
    QString value;
    
    /* Get in the subsection */
    xml->pushDomain();

    /* Get all tags within this subsection */
    list = xml->findNode(QRegExp("*"), subsection);
    list.push_front (subsection);
    count = list.count();
    for (i = 0; i < count; i++) {
        /* Clear the previous attributes */
        attributes_names.clear();
        attributes_values.clear();
        
        /* Get the attributes */
        attrcount = xml->howManyAttributes(list[i]);
        for (j = 0; j < attrcount; j++) {
            attributes_names << xml->readAttributeName(list[i], j);
            attributes_values << xml->readAttribute(list[i], attributes_names[j]);
        }
        /* Get the value */
        value = xml->readString(list[i]);
        replaceVariables (xml, subsection, &value);

        /* Do not process any child of trproduct */
        if (trproduct_processed && list[i].contains ("trproduct")) continue;
        if (trpaytypes_processed && list[i].contains ("trpaytypes")) continue;
        if (trcamareros_processed && list[i].contains ("trcamareros")) continue;
        if (trtotal_offers_processed && list[i].contains ("trtotaloffers")) continue;
        if (trtotal_options_processed && list[i].contains ("trtotaloptions")) continue;
        if (trzproducts_processed && list[i].contains ("trzproducts")) continue;

        if (list[i].contains("trproduct") )
            trproduct_processed = true;

        if (list[i].contains("trpaytypes") )
            trpaytypes_processed = true;

        if (list[i].contains("trcamareros") )
            trcamareros_processed = true;

        if (list[i].contains("trtotaloffers") )
            trtotal_offers_processed = true;

        if (list[i].contains("trtotaloptions") )
            trtotal_options_processed = true;

        if (list[i].contains("trzproducts") )
            trzproducts_processed = true;

        parseTag(xml, list[i], attributes_names, attributes_values, value);
    }
    xml->popDomain();
}

QString PrinterTicket::getMetaTag(const QString& tag){
    int pos;
    QString ret;

    pos = tag.lastIndexOf('.');
    if (pos < 0)
        ret = tag;
    else
        ret = tag.mid(pos+1);

    /* Remove the trailing (if exists) [] */
    if (ret[ret.length()-1] == ']') {
        pos = ret.lastIndexOf('[');
        ret.remove(pos, ret.length());
    }
    return ret;
}

void PrinterTicket::parseTag(XmlConfig* xml, QString tag, QStringList attributes_names, QStringList attributes_values, QString value) {
    QString mtag;

    mtag = getMetaTag(tag);

    if (mtag == "tr") parseTagTR(attributes_names, attributes_values);
    else if (mtag == "td") parseTagTD(attributes_names, attributes_values, value);
    else if (mtag == "trproduct") parseTagTRProduct(xml, tag, attributes_names, attributes_values);
    else if (mtag == "trpaytypes") parseTagTRPayTypes(xml, tag, attributes_names, attributes_values);
    else if (mtag == "trcamareros") parseTagTRCamareros(xml, tag, attributes_names, attributes_values);
    else if (mtag == "troffers") parseTagTROffers(xml, tag, attributes_names, attributes_values);
    else if (mtag == "troptions") parseTagTROptions(xml, tag, attributes_names, attributes_values);
    else if (mtag == "trtotaloffers") parseTagTRTotalOffers(xml, tag, attributes_names, attributes_values);
    else if (mtag == "trtotaloptions") parseTagTRTotalOptions(xml, tag, attributes_names, attributes_values);
    else if (mtag == "trzproducts") parseTagTRZProduct(xml, tag, attributes_names, attributes_values);
    else if (mtag == "th") parseTagTH(attributes_names, attributes_values);
    else if (mtag == "b") parseTagB(attributes_names, attributes_values, value);
    else if (mtag == "u") parseTagU(attributes_names, attributes_values, value);
    else if (mtag == "br") parseTagBR(attributes_names, attributes_values, value);
    else if (mtag == "hr") parseTagHR(attributes_names, attributes_values);
    else if (mtag == "normal") parseTagNormal(attributes_names, attributes_values, value);
    else if (mtag == "cut") parseTagCut(attributes_names, attributes_values, value);
    else if (mtag == "font") parseTagFont(attributes_names, attributes_values, value);
    else if (mtag == "trproptions") parseTagTRPROptions(xml,tag,attributes_names, attributes_values, value);
}

void PrinterTicket::parseTagDebug(XmlConfig* xml, QString tag, QStringList attributes_names, QStringList attributes_values, QString value) {
    int i, count;
    cout << "xml output" <<endl;
    cout << "--------------------------------------------------------------------" << endl;
    xml->debug();
    cout << "--------------------------------------------------------------------" << endl;
    cout << " TAG = "<< tag.toStdString().c_str() << "value " <<value.toStdString().c_str();
    cout << "lista de atributos"<<endl;
    count = attributes_names.count();
    for(i=0;i<count;i++){
        cout << "Atributo  ["<< (attributes_names[i]).toStdString().c_str() << "]  Valor ["<< (attributes_values[i]).toStdString().c_str() << "]" <<endl;
    }
}



QString PrinterTicket::getAttributeValue (QStringList attributes_names, QStringList attributes_values, QString attr) {
    int i, count;

    count = attributes_names.count();
    for (i = 0; i < count; i++) {
        if (attributes_names[i] == attr)
            return attributes_values[i];
    }
    return QString{};
}

void PrinterTicket::parseTagTR(QStringList attributes_names, QStringList attributes_values) {
    QStringList at_n = attributes_names;
    QStringList at_v = attributes_values;
    printersamsungsrp350->cmdInvertColors(0);
    printersamsungsrp350->cmdLineBreak();
}

void PrinterTicket::parseTagTRProduct(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values) {
    unsigned int i;
    int pos, count, j, attrcount, h;
    QStringList list;
    QString value;
    QString trproduct_domain;
    QStringList at_names, at_values;
    QStringList options_list;

    xml->pushDomain();
    /* Get in the domain to get a list of the td's */
    xml->setDomain(dom);
    trproduct_domain = xml->getDomain();
    list = xml->findNode (QRegExp("*"));
    xml->delDomain();

    /* Get in the domain where all products are */
    if (!xml->setDomain("ticket.sections", true)) {
        xml->popDomain();
        return;
    }

    /* Find the definitions section */
    pos = findSection (xml, "definitions");
    if (pos < 0) {
        cerr << "Unable to find the definitions section" << endl;
        xml->popDomain();
        return;
    }

    /* We are in section definitions.items */
    xml->setDomain("section["+QString::number(pos)+"].items");
    /* Find the command item */
    pos = findItem (xml, "command");

    if (pos < 0) {
        cerr << "Unable to find items of subsection command" << endl;
        xml->popDomain();
        return;
    }

    /* We are in the place were the <product> tags are */
    xml->setDomain("item["+QString::number(pos)+"]");
    count = xml->howManyTags("product");
    /* Look for the tag which contains information about $var */
    /* Go through all the products we have */
    for (h = 0; h < count; h++) {
        QString description, quantity, price, unitPrice, options, printed;

        /* Get information of this product */
        description = xml->readAttribute("product["+QString::number(h)+"]", "description");
        quantity = xml->readAttribute("product["+QString::number(h)+"]", "quantity");
        price = xml->readAttribute("product["+QString::number(h)+"]", "price");
        unitPrice = xml->readAttribute("product["+QString::number(h)+"]", "unitPrice");
        printed = xml->readAttribute("product["+QString::number(h)+"]", "printed");
        options = xml->readAttribute("product["+QString::number(h)+"]", "product_options");

        /* Send the TR we are emulating */
        parseTagTR (attributes_names, attributes_values);

        /* Go back to the domain where we have the "HTML" */
        xml->pushDomain ();
        xml->delDomain ();
        xml->setDomain(trproduct_domain, true);

        for (i = 0; i < list.count(); i++) {
            at_names.clear();
            at_values.clear();
            /* Get the attributes */
            attrcount = xml->howManyAttributes(list[i]);
            for (j = 0; j < attrcount; j++) {
                at_names << xml->readAttributeName(list[i], j);
                at_values << xml->readAttribute(list[i], at_names[j]);
            }
            /* Get the value */
            value = xml->readString(list[i]);
            if (value == "%description%")
                value = description;
            else if (value == "%quantity%")
                value = quantity;
            else if (value == "%unitPrice%")
                value = unitPrice;
            else if (value == "%price%")
                value = price;
            else if (value == "%printed%")
                value = printed;
            else if (getMetaTag(list[i]) == "trproptions")
                value = options;
            parseTag(xml, list[i], at_names, at_values, value);

        }
        xml->popDomain();
    }
    xml->popDomain();
}

void PrinterTicket::parseTagTRPROptions(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values,QString value){
    int i,count;
    QStringList pr_options;
    QString separator;
    QString init_space;
    QString width;
    QString prev_align;
    QStringList at_names;
    QStringList at_values;

    int aux;


    prev_align = alignment;

    width = getAttributeValue (attributes_names, attributes_values, "width");
    init_space = getAttributeValue (attributes_names, attributes_values, "init_space");

    aux = width.toInt() - init_space.toInt();
    width = QString::number(aux);

    separator = getAttributeValue (attributes_names, attributes_values, "separator");
    alignment = getAttributeValue (attributes_names, attributes_values, "align");

    //        pr_options = QStringList::split(',',value);
    pr_options = value.split(',');
    count = pr_options.count();

    parseTagTR (attributes_names, attributes_values);

    if (separator=="comma"){
        at_names.clear();
        at_values.clear();

        at_names << "width";
        at_values <<  init_space;

        at_names << "values";
        at_values << "1";

        at_names.clear();
        at_values.clear();

        this->parseTagTD(at_names,at_values," ");
        this->parseTagNormal(at_names,at_values,value);
    }
    else{
        for (i=0;i<count;i++){
            at_names.clear();
            at_values.clear();

            at_names << "width";
            at_values <<  init_space;

            at_names << "values";
            at_values << "1";

            this->parseTagTD(at_names,at_values," ");

            at_names.clear();
            at_values.clear();

            this->parseTagNormal(at_names,at_values," ");

            at_names.clear();
            at_values.clear();

            at_names << "width";
            at_values <<  width;

            at_names << "values";
            at_values << "1";

            this->parseTagTD(at_names,at_values,"");

            at_names.clear();
            at_values.clear();

            this->parseTagNormal(at_names,at_values,pr_options[i]);
        }
    }
    alignment = prev_align;
}


void PrinterTicket::parseTagTRPayTypes(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values) {
    unsigned int i;
    int pos, count, j, attrcount, h;
    QStringList list;
    QStringList at_names, at_values;
    QString value;
    QString trproduct_domain;

    xml->pushDomain();
    /* Get in the domain to get a list of the td's */
    xml->setDomain(dom);
    trproduct_domain = xml->getDomain();
    list = xml->findNode (QRegExp("*"));
    xml->delDomain();

    /* Get in the domain where all products are */
    if (!xml->setDomain("ticket.sections", true)) {
        xml->popDomain();
        return;
    }

    /* Find the definitions section */
    pos = findSection (xml, "definitions");

    if (pos < 0) {
        cerr << "Unable to find the definitions section" << endl;
        xml->popDomain();
        return;
    }

    /* We are in section definitions.items */
    xml->setDomain("section["+QString::number(pos)+"].items");

    /* Find the command item */
    pos = findItem (xml, "paytypes");

    if (pos < 0) {
        cerr << "Unable to find items of subsection paytypes" << endl;
        xml->popDomain();
        return;
    }

    /* We are in the place were the <product> tags are */
    xml->setDomain("item["+QString::number(pos)+"]");
    count = xml->howManyTags("paytype");
    for (h = 0; h < count; h++) {
        QString num_tickets, total_income, name;
        num_tickets = xml->readAttribute("paytype["+QString::number(h)+"]", "num_tickets");
        total_income = xml->readAttribute("paytype["+QString::number(h)+"]", "total_income");
        name = xml->readAttribute("paytype["+QString::number(h)+"]", "name");
        /* Send the TR we are emulating */
        parseTagTR (attributes_names, attributes_values);
        /* Go back to the domain where we have the "HTML" */
        xml->pushDomain ();
        xml->delDomain ();
        xml->setDomain(trproduct_domain, true);
        for (i = 0; i < list.count(); i++) {
            at_names.clear();
            at_values.clear();
            attrcount = xml->howManyAttributes(list[i]);
            for (j = 0; j < attrcount; j++) {
                at_names << xml->readAttributeName(list[i], j);
                at_values << xml->readAttribute(list[i], at_names[j]);
            }
            /* Get the value */
            value = xml->readString(list[i]);
            if (value == "%num_tickets%")
                value = num_tickets;
            else if (value == "%total_income%")
                value = total_income;
            else if (value == "%name%")
                value = name;
            parseTag(xml, list[i], at_names, at_values, value);
        }
        xml->popDomain();
    }
    xml->popDomain();
}

void PrinterTicket::parseTagTRCamareros(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values) {
    bool troffers_processed = false;
    bool troptions_processed = false;
    unsigned int i;
    int pos, count, j, attrcount, h;
    QStringList at_names, at_values;
    QString value;
    QString trproduct_domain;
    QStringList list;

    xml->pushDomain();
    /* Get in the domain to get a list of the td's */
    xml->setDomain(dom);
    trproduct_domain = xml->getDomain();
    list = xml->findNode (QRegExp("*"));
    xml->delDomain();

    /* Get in the domain where all products are */
    if (!xml->setDomain("ticket.sections", true)) {
        xml->popDomain();
        return;
    }

    /* Find the definitions section */
    pos = findSection (xml, "definitions");

    if (pos < 0) {
        cerr << "Unable to find the definitions section" << endl;
        xml->popDomain();
        return;
    }

    /* We are in section definitions.items */
    xml->setDomain("section["+QString::number(pos)+"].items");

    /* Find the command item */
    pos = findItem (xml, "employees");
    if (pos < 0) {
        cerr << "Unable to find items of subsection employees" << endl;
        xml->popDomain();
        return;
    }
    xml->setDomain("item["+QString::number(pos)+"]");
    count = xml->howManyTags("employee");
    for (h = 0; h < count; h++) {
        QString num_tickets, id, total_income, name,cashbox_open,total_invitations;
        /* Get information of this product */
        num_tickets = xml->readAttribute("employee["+QString::number(h)+"]", "num_tickets");
        total_income = xml->readAttribute("employee["+QString::number(h)+"]", "total_income");
        name = xml->readAttribute("employee["+QString::number(h)+"]", "name");
        id = xml->readAttribute("employee["+QString::number(h)+"]", "id");
        cashbox_open = xml->readAttribute("employee["+QString::number(h)+"]", "cashbox_open");
        total_invitations = xml->readAttribute("employee["+QString::number(h)+"]", "total_invitations");
        /* Send the TR we are emulating */
        parseTagTR (attributes_names, attributes_values);
        /* Go back to the domain where we have the "HTML" */
        xml->pushDomain ();
        xml->delDomain ();
        xml->setDomain(trproduct_domain, true);
        for (i = 0; i < list.count(); i++) {
            at_names.clear();
            at_values.clear();
            attrcount = xml->howManyAttributes(list[i]);
            for (j = 0; j < attrcount; j++) {
                at_names << xml->readAttributeName(list[i], j);
                at_values << xml->readAttribute(list[i], at_names[j]);
            }
            /* Try to process the offers */
            if (troffers_processed && list[i].contains ("troffers")) continue;
            if (list[i].contains("troffers") )
                troffers_processed = true;

            if (troptions_processed && list[i].contains ("troptions")) continue;
            if (list[i].contains("troptions") )
                troptions_processed = true;

            at_names << "employee";
            at_values << name;
            /* Get the value */
            value = xml->readString(list[i]);
            if (value == "%num_tickets%")
                value = num_tickets;
            else if (value == "%total_income%")
                value = total_income;
            else if (value == "%id%")
                value = id;
            else if (value == "%name%")
                value = name;
            else if (value == "%cashbox_open%")
                value = cashbox_open;
            else if (value == "%total_invitations%")
                value = total_invitations;
            parseTag(xml, list[i], at_names, at_values, value);
        }
        troffers_processed = false;
        troptions_processed = false;
        xml->popDomain();
    }
    xml->popDomain();
}

void PrinterTicket::parseTagTROffers(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values) {
    unsigned int i;
    int pos, count, j, attrcount, h;
    QString value;
    QString trproduct_domain;
    QString employee;
    QString employee_of_xml;
    QStringList list;
    QStringList at_names, at_values;

    pos = attributes_names.indexOf("employee");
    employee = attributes_values[pos];
    xml->pushDomain();

    /* Get in the domain to get a list of the td's */
    xml->setDomain(dom);
    trproduct_domain = xml->getDomain();
    list = xml->findNode (QRegExp("*"));
    xml->delDomain();

    /* Get in the domain where all products are */
    if (!xml->setDomain("ticket.sections", true)) {
        xml->popDomain();
        return;
    }

    /* Find the definitions section */
    pos = findSection (xml, "definitions");

    if (pos < 0) {
        cerr << "Unable to find the definitions section" << endl;
        xml->popDomain();
        return;
    }

    /* We are in section definitions.items */
    xml->setDomain("section["+QString::number(pos)+"].items");

    /* Find the offers item */
    pos = findItem (xml, "offers");

    if (pos < 0) {
        cerr << "Unable to find items of subsection offers" << endl;
        xml->popDomain();
        return;
    }

    /* We are in the place were the <product> tags are */
    xml->setDomain("item["+QString::number(pos)+"]");
    count = xml->howManyTags("offer");
    for (h = 0; h < count; h++) {
        QString type, name, count;
        /* Check if this offer belongs to the employee we are working on */
        employee_of_xml = xml->readAttribute("offer["+QString::number(h)+"]", "employee");
        if (employee_of_xml != employee)
            continue;
        /* Get information of this product */
        type = xml->readAttribute("offer["+QString::number(h)+"]", "type");
        name = xml->readAttribute("offer["+QString::number(h)+"]", "name");
        count = xml->readAttribute("offer["+QString::number(h)+"]", "count");
        /* Send the TR we are emulating */
        parseTagTR (attributes_names, attributes_values);
        /* Go back to the domain where we have the "HTML" */
        xml->pushDomain ();
        xml->delDomain ();
        xml->setDomain(trproduct_domain, true);
        for (i = 0; i < list.count(); i++) {
            at_names.clear();
            at_values.clear();
            /* Get the attributes */
            attrcount = xml->howManyAttributes(list[i]);
            for (j = 0; j < attrcount; j++) {
                at_names << xml->readAttributeName(list[i], j);
                at_values << xml->readAttribute(list[i], at_names[j]);
            }
            /* Get the value */
            value = xml->readString(list[i]);
            if (value == "%type%")
                value = type;
            else if (value == "%name%")
                value = name;
            else if (value == "%count%")
                value = count;
            parseTag(xml, list[i], at_names, at_values, value);
        }
        xml->popDomain();
    }
    xml->popDomain();
}


void PrinterTicket::parseTagTROptions(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values) {
    unsigned int i;
    int pos, count, j, attrcount, h;
    QString value;
    QString trproduct_domain;
    QString employee;
    QString employee_of_xml;
    QStringList list;
    QStringList at_names, at_values;


    pos = attributes_names.indexOf("employee");
    employee = attributes_values[pos];
    xml->pushDomain();

    /* Get in the domain to get a list of the td's */
    xml->setDomain(dom);
    trproduct_domain = xml->getDomain();
    list = xml->findNode (QRegExp("*"));
    xml->delDomain();

    /* Get in the domain where all products are */
    if (!xml->setDomain("ticket.sections", true)) {
        xml->popDomain();
        return;
    }

    /* Find the definitions section */
    pos = findSection (xml, "definitions");

    if (pos < 0) {
        cerr << "Unable to find the definitions section" << endl;
        xml->popDomain();
        return;
    }

    /* We are in section definitions.items */
    xml->setDomain("section["+QString::number(pos)+"].items");

    /* Find the offers item */
    pos = findItem (xml, "options");

    if (pos < 0) {
        cerr << "Unable to find items of subsection options" << endl;
        xml->popDomain();
        return;
    }

    /* We are in the place were the <product> tags are */
    xml->setDomain("item["+QString::number(pos)+"]");
    count = xml->howManyTags("option");
    for (h = 0; h < count; h++) {
        QString type, name, count;
        /* Check if this offer belongs to the employee we are working on */
        employee_of_xml = xml->readAttribute("option["+QString::number(h)+"]", "employee");
        if (employee_of_xml != employee)
            continue;
        /* Get information of this product */
        type = xml->readAttribute("option["+QString::number(h)+"]", "type");
        name = xml->readAttribute("option["+QString::number(h)+"]", "name");
        count = xml->readAttribute("option["+QString::number(h)+"]", "count");

        /* Send the TR we are emulating */
        parseTagTR (attributes_names, attributes_values);
        /* Go back to the domain where we have the "HTML" */
        xml->pushDomain ();
        xml->delDomain ();
        xml->setDomain(trproduct_domain, true);
        for (i = 0; i < list.count(); i++) {
            at_names.clear();
            at_values.clear();
            /* Get the attributes */
            attrcount = xml->howManyAttributes(list[i]);
            for (j = 0; j < attrcount; j++) {
                at_names << xml->readAttributeName(list[i], j);
                at_values << xml->readAttribute(list[i], at_names[j]);
            }
            value = xml->readString(list[i]);
            /* Get the value */
            if (value == "%type%")
                value = type;
            else if (value == "%name%")
                value = name;
            else if (value == "%count%")
                value = count;
            parseTag(xml, list[i], at_names, at_values, value);
        }
        xml->popDomain();
    }
    xml->popDomain();
}

//void PrinterTicket::parseTagTROptions(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values) {
//unsigned int i;
//int pos, count, j, attrcount, h;
//QString value;
//QString trproduct_domain;
//QString employee;
//QString employee_of_xml;
//QStringList list;
//QStringList at_names, at_values;
//
//        pos = attributes_names.indexOf("employee");
//        employee = attributes_values[pos];
//        xml->pushDomain();
//
//        /* Get in the domain to get a list of the td's */
//        xml->setDomain(dom);
//        trproduct_domain = xml->getDomain();
//        list = xml->findNode (QRegExp("*"));
//        xml->delDomain();
//
//        /* Get in the domain where all products are */
//        if (!xml->setDomain("ticket.sections", true)) {
//                xml->popDomain();
//                return;
//        }
//
//        /* Find the definitions section */
//        pos = findSection (xml, "definitions");
//
//        if (pos < 0) {
//                cerr << "Unable to find the definitions section" << endl;
//                xml->popDomain();
//                return;
//        }
//
//        /* We are in section definitions.items */
//        xml->setDomain("section["+QString::number(pos)+"].items");
//
//        /* Find the offers item */
//        pos = findItem (xml, "options");
//
//        if (pos < 0) {
//                cerr << "Unable to find items of subsection options" << endl;
//                xml->popDomain();
//                return;
//        }
//
//        /* We are in the place were the <product> tags are */
//        xml->setDomain("item["+QString::number(pos)+"]");
//        count = xml->howManyTags("option");
//        for (h = 0; h < count; h++) {
//                QString type, name, count;
//                /* Check if this offer belongs to the employee we are working on */
//                employee_of_xml = xml->readAttribute("option["+QString::number(h)+"]", "employee");
//                if (employee_of_xml != employee)
//                        continue;
//                /* Get information of this product */
//                type = xml->readAttribute("option["+QString::number(h)+"]", "type");
//                name = xml->readAttribute("option["+QString::number(h)+"]", "name");
//                count = xml->readAttribute("option["+QString::number(h)+"]", "amount");
//                /* Send the TR we are emulating */
//                parseTagTR(attributes_names, attributes_values);
//                /* Go back to the domain where we have the "HTML" */
//                xml->pushDomain ();
//                xml->delDomain ();
//                xml->setDomain(trproduct_domain, true);
//                for (i = 0; i < list.count(); i++) {
//                        at_names.clear();
//                        at_values.clear();
//                        /* Get the attributes */
//                        attrcount = xml->howManyAttributes(list[i]);
//                        for (j = 0; j < attrcount; j++) {
//                                at_names << xml->readAttributeName(list[i], j);
//                                at_values << xml->readAttribute(list[i], at_names[j]);
//                        }
//                        /* Get the value */
//                        value = xml->readString(list[i]);
//                        if (value == "%type%")
//                                value = type;
//                        else if (value == "%name%")
//                                value = name;
//                        else if (value == "%amount%")
//                                value = count;
//                        parseTag(xml, list[i], at_names, at_values, value);
//                }
//                xml->popDomain();
//        }
//        xml->popDomain();
//}


void PrinterTicket::parseTagTRTotalOffers(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values) {
    unsigned int i;
    int pos, count, j, attrcount, h;
    QString value;
    QString trproduct_domain;
    QStringList list;
    QStringList at_names, at_values;

    xml->pushDomain();
    xml->setDomain(dom);
    trproduct_domain = xml->getDomain();
    list = xml->findNode (QRegExp("*"));
    xml->delDomain();

    /* Get in the domain where all products are */
    if (!xml->setDomain("ticket.sections", true)) {
        xml->popDomain();
        return;
    }

    /* Find the definitions section */
    pos = findSection (xml, "definitions");

    if (pos < 0) {
        cerr << "Unable to find the definitions section" << endl;
        xml->popDomain();
        return;
    }

    /* We are in section definitions.items */
    xml->setDomain("section["+QString::number(pos)+"].items");

    /* Find the offers item */
    pos = findItem (xml, "total_offers");

    if (pos < 0) {
        cerr << "Unable to find items of subsection offers" << endl;
        xml->popDomain();
        return;
    }

    /* We are in the place were the <product> tags are */
    xml->setDomain("item["+QString::number(pos)+"]");
    count = xml->howManyTags("offer");
    for (h = 0; h < count; h++) {
        QString type, name, count;
        type = xml->readAttribute("offer["+QString::number(h)+"]", "type");
        name = xml->readAttribute("offer["+QString::number(h)+"]", "name");
        count = xml->readAttribute("offer["+QString::number(h)+"]", "count");
        /* Send the TR we are emulating */
        parseTagTR (attributes_names, attributes_values);
        xml->pushDomain ();
        xml->delDomain ();
        xml->setDomain(trproduct_domain, true);
        for (i = 0; i < list.count(); i++) {
            at_names.clear();
            at_values.clear();
            attrcount = xml->howManyAttributes(list[i]);
            for (j = 0; j < attrcount; j++) {
                at_names << xml->readAttributeName(list[i], j);
                at_values << xml->readAttribute(list[i], at_names[j]);
            }
            /* Get the value */
            value = xml->readString(list[i]);
            if (value == "%type%")
                value = type;
            else if (value == "%name%")
                value = name;
            else if (value == "%count%")
                value = count;
            parseTag(xml, list[i], at_names, at_values, value);
        }
        xml->popDomain();
    }
    xml->popDomain();
}

void PrinterTicket::parseTagTRTotalOptions(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values) {
    unsigned int i;
    int pos, count, j, attrcount, h;
    QString value;
    QString trproduct_domain;
    QStringList list;
    QStringList at_names, at_values;

    xml->pushDomain();
    xml->setDomain(dom);
    trproduct_domain = xml->getDomain();
    list = xml->findNode (QRegExp("*"));
    xml->delDomain();

    /* Get in the domain where all products are */
    if (!xml->setDomain("ticket.sections", true)) {
        xml->popDomain();
        return;
    }

    /* Find the definitions section */
    pos = findSection (xml, "definitions");

    if (pos < 0) {
        cerr << "Unable to find the definitions section" << endl;
        xml->popDomain();
        return;
    }

    /* We are in section definitions.items */
    xml->setDomain("section["+QString::number(pos)+"].items");

    /* Find the offers item */
    pos = findItem (xml, "total_options");

    if (pos < 0) {
        cerr << "Unable to find items of subsection offers" << endl;
        xml->popDomain();
        return;
    }

    /* We are in the place were the <product> tags are */
    xml->setDomain("item["+QString::number(pos)+"]");
    count = xml->howManyTags("option");
    for (h = 0; h < count; h++) {
        QString type, name, count;
        type = xml->readAttribute("option["+QString::number(h)+"]", "type");
        name = xml->readAttribute("option["+QString::number(h)+"]", "name");
        count = xml->readAttribute("option["+QString::number(h)+"]", "amount");
        /* Send the TR we are emulating */
        parseTagTR (attributes_names, attributes_values);
        xml->pushDomain ();
        xml->delDomain ();
        xml->setDomain(trproduct_domain, true);
        for (i = 0; i < list.count(); i++) {
            at_names.clear();
            at_values.clear();
            attrcount = xml->howManyAttributes(list[i]);
            for (j = 0; j < attrcount; j++) {
                at_names << xml->readAttributeName(list[i], j);
                at_values << xml->readAttribute(list[i], at_names[j]);
            }
            /* Get the value */
            value = xml->readString(list[i]);
            if (value == "%type%")
                value = type;
            else if (value == "%name%")
                value = name;
            else if (value == "%amount%")
                value = count;
            parseTag(xml, list[i], at_names, at_values, value);
        }
        xml->popDomain();
    }
    xml->popDomain();
}


void PrinterTicket::parseTagTRZProduct(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values) {
    unsigned int i;
    int pos, count, j, attrcount, h;
    QString value;
    QString trproduct_domain;
    QStringList list;
    QStringList at_names, at_values;

    xml->pushDomain();
    xml->setDomain(dom);
    trproduct_domain = xml->getDomain();
    list = xml->findNode (QRegExp("*"));
    xml->delDomain();

    if (!xml->setDomain("ticket.sections", true)) {
        xml->popDomain();
        return;
    }

    /* Find the definitions section */
    pos = findSection (xml, "definitions");

    if (pos < 0) {
        cerr << "Unable to find the definitions section" << endl;
        xml->popDomain();
        return;
    }

    /* We are in section definitions.items */
    xml->setDomain("section["+QString::number(pos)+"].items");

    /* Find the command item */
    pos = findItem (xml, "zproducts");

    if (pos < 0) {
        cerr << "Unable to find items of subsection zproducts" << endl;
        xml->popDomain();
        return;
    }

    xml->setDomain("item["+QString::number(pos)+"]");
    count = xml->howManyTags("zproduct");
    for (h = 0; h < count; h++) {
        QString units, total_income, name;
        units = xml->readAttribute("zproduct["+QString::number(h)+"]", "units");
        total_income = xml->readAttribute("zproduct["+QString::number(h)+"]", "total_income");
        name = xml->readAttribute("zproduct["+QString::number(h)+"]", "name");
        /* Send the TR we are emulating */
        parseTagTR (attributes_names, attributes_values);
        xml->pushDomain ();
        xml->delDomain ();
        xml->setDomain(trproduct_domain, true);
        for (i = 0; i < list.count(); i++) {
            at_names.clear();
            at_values.clear();
            /* Get the attributes */
            attrcount = xml->howManyAttributes(list[i]);
            for (j = 0; j < attrcount; j++) {
                at_names << xml->readAttributeName(list[i], j);
                at_values << xml->readAttribute(list[i], at_names[j]);
            }
            value = xml->readString(list[i]);
            if (value == "%units%")
                value = units;
            else if (value == "%total_income%")
                value = total_income;
            else if (value == "%name%")
                value = name;
            parseTag(xml, list[i], at_names, at_values, value);
        }
        xml->popDomain();
    }
    xml->popDomain();
}

void PrinterTicket::parseTagTD(QStringList attributes_names, QStringList attributes_values, QString value) {
    int s, v;
    QString space,val;

    val = value;

    space = getAttributeValue (attributes_names, attributes_values, "width");
    if (!space.isNull())
        s = space.toInt();
    else {
        s = 0xffff;
    }

    space = getAttributeValue (attributes_names, attributes_values, "values");
    if (!space.isNull())
        v = space.toInt();
    else
        v = -1;

    space_to_write = s;
    values_to_write = v;
    alignment = getAttributeValue (attributes_names, attributes_values, "align");
}

void PrinterTicket::parseTagTH(QStringList attributes_names, QStringList attributes_values) {
    printersamsungsrp350->cmdLineBreak();
    printersamsungsrp350->cmdInvertColors(1);
}

void PrinterTicket::parseTagB(QStringList attributes_names, QStringList attributes_values, QString value) {
    prePrint(&value);
    printersamsungsrp350->cmdBold();
    printersamsungsrp350->string(value);
}

void PrinterTicket::parseTagU(QStringList attributes_names, QStringList attributes_values, QString value) {
    prePrint(&value);
    printersamsungsrp350->cmdUnderline();
    printersamsungsrp350->string(value);
}

void PrinterTicket::parseTagBR(QStringList attributes_names, QStringList attributes_values, QString value) {
    printersamsungsrp350->cmdLineBreak();
    printersamsungsrp350->string(value);
}

void PrinterTicket::parseTagHR(QStringList attributes_names, QStringList attributes_values) {
    int i, w;
    QString width, type, nobr;

    width = getAttributeValue (attributes_names, attributes_values, "width");
    w = width.toInt();

    type = getAttributeValue (attributes_names, attributes_values, "type");
    nobr = getAttributeValue (attributes_names, attributes_values, "nobr");

    if (nobr.isNull())
        printersamsungsrp350->cmdLineBreak();

    for (i = 0; i < w; i++){
        if (type == "dashed") {
            printersamsungsrp350->string("_ ");
            i++;
        }
        else if (type == "dotted")
            printersamsungsrp350->string(".");
        else if (type == "vin")
            printersamsungsrp350->string("~");
        else
            printersamsungsrp350->string("_");
    }

    if (nobr.isNull())
        printersamsungsrp350->cmdLineBreak();
}

void PrinterTicket::parseTagNormal(QStringList attributes_names, QStringList attributes_values, QString value) {
    prePrint(&value);
    printersamsungsrp350->cmdNormal();
    printersamsungsrp350->string(value);
}

void PrinterTicket::parseTagCut(QStringList attributes_names, QStringList attributes_values, QString value) {
    printersamsungsrp350->cmdCut();
}

void PrinterTicket::parseTagFont(QStringList attributes_names, QStringList attributes_values, QString value) {
    QString width = getAttributeValue (attributes_names, attributes_values, "width");
    QString height = getAttributeValue (attributes_names, attributes_values, "height");
    printersamsungsrp350->cmdFontSize (width.toInt(), height.toInt());
    printersamsungsrp350->string (value);
}

void PrinterTicket::prePrint(QString *value) {
    /* If the length of the value should be controlled... */
    if (values_to_write > 0) {
        /* ... and the length of the string is longer than available space... */
        if (value->length () > space_to_write) {
            /* ... cut it. */
            value->truncate(space_to_write-2);
            value->append("..");
        }
        /* ... or if the length of the string doesn't fills the available space (and this is the last item to be written) ... */
        if (value->length () < space_to_write && values_to_write == 1) {
            while (space_to_write > value->length ())
                value->append(' ');
        }
        space_to_write -= value->length();
        values_to_write--;

        /* Check if we have to make any alignment */
        if (alignment == "right")
            alignRight (value);
    }
    else if (values_to_write == 0){
        /* Sorry, no space for this */
        value->truncate (0);
        values_to_write--;
    }

    /* If values_to_write is less than 0, then we don't have to control anything */
    /* If values_to_write is 0 then put it in -1 */
    if (values_to_write == 0)
        values_to_write = -1;
}

/* This function will count how many free spaces are to the left and to the right
 *  and put them all at the left.
 */
void PrinterTicket::alignRight(QString *value) {
    int i;
    int spaces = 0;

    /* Count spaces at left */
    for (i = 0; i < value->length(); i++) {
        if ((*value)[i] != ' ')
            break;
        spaces++;
    }

    value->remove(0, i);

    /* Count spaces at right */
    for (i = value->length()-1; i >= 0; i--) {
        if ((*value)[i] != ' ')
            break;
        spaces++;
    }

    value->truncate (i+1);

    /* Add spaces */
    for (i = 0; i < spaces; i++)
        value->prepend (' ');
}

int PrinterTicket::findSection(XmlConfig* xml, QString section) {
    int i, count;
    xml->pushDomain();
    xml->delDomain();

    if (!xml->setDomain("ticket.sections", true)) {
        xml->popDomain();
        return -1;
    }

    count = xml->howManyTags("section");
    for (i = 0; i < count; i++) {
        QString aux;
        aux = xml->readAttribute("section["+QString::number(i)+"]", "name");
        if (aux == section)
            break;
    }
    xml->popDomain();

    if (i == count)
        return -1;
    return i;
}

int PrinterTicket::findItem(XmlConfig* xml, QString item) {
    int i = -1, count;

    count = xml->howManyTags("item");
    for (i = 0; i < count; i++) {
        QString aux;
        aux = xml->readAttribute("item["+QString::number(i)+"]", "name");
        if (aux == item)
            break;
    }
    if (i == count)
        return -1;
    return i;
}

void PrinterTicket::replaceVariables (XmlConfig* xml, const QString& subsection, QString *value) {
    QString var;
    QString var_value;

    if (value->isNull())
        return;

    if (value->startsWith("%") && value->endsWith("%")) {
        var = value->mid(1, value->length()-2);
        var_value = getVariable (xml, subsection, var);
        if (!var_value.length()){
            if  ((*value) == "%ticketnumber%")
                var_value = "COMANDA";
            else
                var_value = QString ("No se parseo '")+(*value)+QString("'!");
        }
        value->replace (*value, var_value);
    }
}

QString PrinterTicket::getVariable (XmlConfig* xml, const QString& subsection, QString& var) {
    int i, pos, count;
    QStringList list;
    QString item;
    QString attr;

    xml->pushDomain();
    xml->delDomain();
    /* Get in the domain were we need to be */
    if (!xml->setDomain("ticket.sections", true)) {
        xml->popDomain();
        return QString ("");
    }

    /* Find the definitions section */
    pos = findSection (xml, "definitions");
    if (pos < 0) {
        cerr << "Unable to find the printerRepresentation section" << endl;
        xml->popDomain();
        return QString ("");
    }

    /* We are in section definitions.items */
    xml->setDomain("section["+QString::number(pos)+"].items");
    /* Find the $subsection item */
    pos = findItem (xml, subsection);
    if (pos < 0) {
        cerr << "Unable to find items of subsection " << subsection.toStdString().c_str() << endl;
        xml->popDomain();
        return QString ("");
    }

    /* We are in the place were the <data> tags are */
    xml->setDomain("item["+QString::number(pos)+"]");
    count = xml->howManyTags("data");
    for (i = 0; i < count; i++) {
        attr = xml->readAttribute("data["+QString::number(i)+"]", "name");
        if (attr == var) {
            item = xml->readString ("data["+QString::number(i)+"]");
            xml->popDomain();
            return item;
        }
    }
    xml->popDomain();
    return QString ("");
}
