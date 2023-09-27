/***************************************************************************
                          ticket.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Pablo Fernandez
modified by Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRINTERTICKET_H
#define PRINTERTICKET_H

/**
  *@author
  */

class XmlConfig;
class QDomNode;
class QString;
class QFile;

#include "printersamsungsrp350.h"

class PrinterTicket {
public: 
    PrinterTicket();
    ~PrinterTicket();

    void parse(const QString& path);
    void parse(XmlConfig* xml);
    int findSection(XmlConfig* xml, QString section);
    int findItem(XmlConfig* xml, QString item);
    QString getVariable (XmlConfig* xml, const QString& subsection, QString& var);

protected:
    void parseSubsection(XmlConfig* xml, QString subsection);
    void parseTag(XmlConfig* xml, QString tag, QStringList attr_names, QStringList attr_values, QString value);
    void replaceVariables (XmlConfig* xml, const QString& subsection, QString *value);

    QString getAttributeValue (QStringList attributes_names, QStringList attributes_values, QString attr);

    void parseTagTR();
    void parseTagTRProduct(XmlConfig* xml, QString dom);
    void parseTagTRCamareros(XmlConfig* xml, QString dom);
    void parseTagTROffers(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values);
    void parseTagTRTotalOffers(XmlConfig* xml, QString dom);
    void parseTagTRPayTypes(XmlConfig* xml, QString dom);
    void parseTagTRZProduct(XmlConfig* xml, QString dom);
    void parseTagTD(QStringList attributes_names, QStringList attributes_values);
    void parseTagTH();
    void parseTagB(QString value);
    void parseTagU(QString value);
    void parseTagBR(QString value);
    void parseTagHR(QStringList attributes_names, QStringList attributes_values);
    void parseTagNormal(QString value);
    void parseTagCut();
    void parseTagFont(QStringList attributes_names, QStringList attributes_values, QString value);

    void prePrint(QString* value);
    void alignRight(QString* value);

    PrinterSamsungSRP350* printer;
    int space_to_write;
    int values_to_write;
    QString alignment;

};

#endif
