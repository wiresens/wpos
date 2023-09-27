/***************************************************************************
                          nticketdesignwidget.cpp  -  description
                             -------------------
    begin                : lun abr 19 2004
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

#include "nticketdesignwidget.h"

#include <libbslxml/xmlconfig.h>

#include <QRadioButton>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QMessageBox>

static const QString& SAVED_CONFIG{ "/etc/ntpv/company_ticket_data.xml"};
static const QString& DTD {"/etc/ntpv_backoffice/dtds/company_ticket.dtd"};
static const QString& TEMPLATES_LOCATION {"/etc/ntpv/templates/"};
static const QString& MAIN_TICKET_FILE {"/etc/ntpv/printerhtml.xml"};
static const QString& MAIN_INVOICE_FILE {"/etc/ntpv/invoice.xml"};
static const QString& DCOPPRINTER_FILE {"/etc/ntpv/dcopprinter_config.xml"};
static const QString& ICON_PATH {"/usr/share/ntpv_backoffice/apps/"};

NTicketDesignWidget::NTicketDesignWidget(QWidget *parent, const QString& name ):
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
    clear();
    ok_button->setIcon(QPixmap(ICON_PATH + "48x48/button_ok_48.png"));
    pix_label1->setPixmap(QPixmap(ICON_PATH + "32x32/galeon.png"));
    pix_label2->setPixmap(QPixmap(ICON_PATH + "32x32/kmessedwords.png"));
    pix_label3->setPixmap(QPixmap(ICON_PATH + "32x32/package_editors.png"));

    connect(ok_button,SIGNAL(clicked()),this,SLOT(acceptSlot()));

    connect(company_label,SIGNAL(textChanged(const QString&)),this,SLOT(somethingChanged(const QString&)));
    connect(cif_label,SIGNAL(textChanged(const QString&)),this,SLOT(somethingChanged(const QString&)));
    connect(address_label,SIGNAL(textChanged(const QString&)),this,SLOT(somethingChanged(const QString&)));
    connect(provincia_label,SIGNAL(textChanged(const QString&)),this,SLOT(somethingChanged(const QString&)));
    connect(tlf_label,SIGNAL(textChanged(const QString&)),this,SLOT(somethingChanged(const QString&)));
    connect(head_label,SIGNAL(textChanged(const QString&)),this,SLOT(somethingChanged(const QString&)));
    connect(foot_label,SIGNAL(textChanged(const QString&)),this,SLOT(somethingChanged(const QString&)));
    connect(bye_label,SIGNAL(textChanged(const QString&)),this,SLOT(somethingChanged(const QString&)));
}

void NTicketDesignWidget::acceptSlot(){
    if  (  company_label->text().isEmpty()  ||
           cif_label->text().isEmpty()  ||
           address_label->text().isEmpty()  ||
           provincia_label->text().isEmpty()  ||
           tlf_label->text().isEmpty())
        //                ((tlf_label->text()).isEmpty())  ||
        //                ((head_label->text()).isEmpty())  ||
        //                ((foot_label->text()).isEmpty())  ||
        //                ((bye_label->text()).isEmpty())  )
    {
        QString text = tr("Please complete all fields\n");
        QMessageBox::warning(this,tr("Failed to save ticket"),text, QMessageBox::NoButton);
        return;
    }

    writeXml();
    ok_button->setEnabled(false);
}

void NTicketDesignWidget::clear(){
    company_label->clear();
    cif_label->clear();
    address_label->clear();
    localidad_label->clear();
    provincia_label->clear();
    tlf_label->clear();
    head_label->clear();
    foot_label->clear();
    bye_label->clear();
    no_iva_radiobutton->setChecked(false);
}

bool NTicketDesignWidget::readXml(){


    XmlConfig xml(SAVED_CONFIG);
    if ( !xml.isValid() || !xml.validateXmlWithDTD(DTD,true) )
        return false;

    xml.setDomain("ticket_values");
    company_label->setText(xml.readString("company"));
    cif_label->setText(xml.readString("cif"));
    address_label->setText(xml.readString("address"));
    localidad_label->setText(xml.readString("localidad"));
    provincia_label->setText(xml.readString("provincia"));
    tlf_label->setText(xml.readString("telephone"));
    head_label->setText(xml.readString("head"));
    foot_label->setText(xml.readString("foot"));
    bye_label->setText(xml.readString("bye"));

    if (xml.readString("showtaxes") == "false")
        no_iva_radiobutton->setChecked(true);
    return true;
}

void NTicketDesignWidget::writeXml(){
    QString xml_string;
    QString xml_file;
    int char_length=42;

    {
        QFile file(SAVED_CONFIG);
        if (file.exists()) file.remove();
    }

    {
        XmlConfig xml(DCOPPRINTER_FILE);
        if ( !xml.isValid() )
            char_length = 42;

        else{

            xml_string = xml.readString("main.length");
            char_length = xml_string.toInt();

            if ( char_length !=42 || char_length!=40 )
                char_length = 42;
        }
    }

    {
        XmlConfig xml(SAVED_CONFIG);
        xml.createElementSetDomain("ticket_values");
        xml.createElement("company",company_label->text());
        xml.createElement("cif",cif_label->text());
        xml.createElement("address",address_label->text());
        xml.createElement("localidad",localidad_label->text());
        xml.createElement("provincia",provincia_label->text());
        xml.createElement("telephone",tlf_label->text());
        xml.createElement("head",head_label->text());
        xml.createElement("foot",foot_label->text());
        xml.createElement("bye",bye_label->text());
        if (ok_iva_radiobutton->isChecked())
            xml.createElement("showtaxes","true");
        else
            xml.createElement("showtaxes","false");
        xml.save();
    }


    if (char_length == 42)
        xml_file = QString(TEMPLATES_LOCATION) + "template_ticket.xml";
    else if (char_length == 40)
        xml_file = QString(TEMPLATES_LOCATION) + "template_ticket_40.xml";

    {
        XmlConfig xml (xml_file);
        xml_string = xml.toString();
    }


    xml_string.replace("%COMPANY%",company_label->text());
    xml_string.replace("%CIF%",cif_label->text());
    xml_string.replace("%ADDRESS%",address_label->text());
    xml_string.replace("%LOCALIDAD%",localidad_label->text());
    if ( !provincia_label->text().isEmpty() )
        xml_string.replace("%PROVINCIA%", "("+provincia_label->text()+")");
    else
        xml_string.replace("%PROVINCIA%","");

    xml_string.replace("%TELEPHONE%",tlf_label->text());
    xml_string.replace("%HEAD%",head_label->text());
    xml_string.replace("%FOOT%",foot_label->text());
    xml_string.replace("%BYE%",bye_label->text());

    if ( ok_iva_radiobutton->isChecked()){
        auto aux="<b>"+tr("Base imponible: ")+"</b>";
        aux+="<normal>%price_no_tax%</normal><tr>";
        aux+="<td width=\"13\" values=\"2\"><b>IVA 4% </b><normal>%iva4%</normal></td>";
        aux+="<td width=\"13\" values=\"2\"><b>IVA 7% </b><normal>%iva7%</normal></td>";
        aux+="<td width=\"13\" values=\"2\"><b>IVA 16% </b><normal>%iva16%</normal></td>";
        aux+="</tr><br/>";
        xml_string.replace("%TAX%",aux);
    }
    else{
        auto aux="<b>"+tr("Impuestos incluidos")+"</b>";
        xml_string.replace("%TAX%", aux);
    }

    {
        XmlConfig xml;
        xml.readXmlFromString(xml_string);
        if (xml.isValid())
            xml.save(MAIN_TICKET_FILE);
    }

    if (char_length == 42)
        xml_file = QString(TEMPLATES_LOCATION) + "template_invoice.xml";
    else if (char_length == 40)
        xml_file = QString(TEMPLATES_LOCATION) + "template_ticket_40.xml";

    {
        XmlConfig xml (xml_file);
        xml_string = xml.toString();
    }

    xml_string.replace("%COMPANY%",company_label->text());
    xml_string.replace("%CIF%",cif_label->text());
    xml_string.replace("%ADDRESS%",address_label->text());
    xml_string.replace("%LOCALIDAD%",localidad_label->text());

    if ( !provincia_label->text().isEmpty())
        xml_string.replace("%PROVINCIA%","("+provincia_label->text()+")");
    else
        xml_string.replace("%PROVINCIA%","");

    xml_string.replace("%TELEPHONE%",tlf_label->text());
    xml_string.replace("%HEAD%",head_label->text());
    xml_string.replace("%FOOT%",foot_label->text());
    xml_string.replace("%BYE%",bye_label->text());

    if (ok_iva_radiobutton->isChecked()){
        auto tmp_str="<b>"+tr("Base imponible: ")+"</b>";
        tmp_str+="<normal>%price_no_tax%</normal><tr>";
        tmp_str+="<td width=\"13\" values=\"2\"><b>IVA 4% </b><normal>%iva4%</normal></td>";
        tmp_str+="<td width=\"13\" values=\"2\"><b>IVA 7% </b><normal>%iva7%</normal></td>";
        tmp_str+="<td width=\"13\" values=\"2\"><b>IVA 16% </b><normal>%iva16%</normal></td>";
        tmp_str+="</tr><br/>";
        xml_string.replace("%TAX%",tmp_str);
    }
    else{
        auto tmp_str="<b>"+tr("Impuestos incluidos")+"</b>";
        xml_string.replace("%TAX%",tmp_str);
    }

    {
        XmlConfig xml;
        xml.readXmlFromString(xml_string);
        if (xml.isValid())
            xml.save(MAIN_INVOICE_FILE);
    }
}

void NTicketDesignWidget::showEvent(QShowEvent *e){
    ok_button->setEnabled(true);
    clear();
    readXml();
    QWidget::showEvent(e);
}

void NTicketDesignWidget::somethingChanged(const QString& text){
    ok_button->setEnabled(true);
}
