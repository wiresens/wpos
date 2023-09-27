/***************************************************************************
                          ticket.h  -  description
                             -------------------
    begin                 : Mon Jun 2 2003
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
    PrinterTicket(const QString& _type, const QString& _device, const QString& _user="", const QString& _passwd="");
    ~PrinterTicket();

    void parse(const QString& path);
    void parse(XmlConfig* xml);
    int findSection(XmlConfig* xml, QString section);
    int findItem(XmlConfig* xml, QString item);
    QString getVariable (XmlConfig* xml, const QString& subsection, QString& var);

protected:
    void parseSubsection(XmlConfig* xml, QString subsection);
    void parseTag(XmlConfig* xml, QString tag, QStringList attributes_names, QStringList attributes_values, QString value);
    void parseTagDebug(XmlConfig* xml, QString tag, QStringList attributes_names, QStringList attributes_values, QString value);
    void replaceVariables (XmlConfig* xml, const QString& subsection, QString* value);

    QString getAttributeValue (QStringList attributes_names, QStringList attributes_values, QString attr);

    void parseTagTR(QStringList attributes_names, QStringList attributes_values);
    void parseTagTRProduct(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values);
    void parseTagTRPROptions(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values,QString value);
    void parseTagTRCamareros(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values);
    void parseTagTROffers(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values);
    void parseTagTROptions(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values);
    void parseTagTRTotalOffers(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values);
    void parseTagTRTotalOptions(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values);
    void parseTagTRPayTypes(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values);
    void parseTagTRZProduct(XmlConfig* xml, QString dom, QStringList attributes_names, QStringList attributes_values);
    void parseTagTD(QStringList attributes_names, QStringList attributes_values, QString value);
    void parseTagTH(QStringList attributes_names, QStringList attributes_values);
    void parseTagB(QStringList attributes_names, QStringList attributes_values, QString value);
    void parseTagU(QStringList attributes_names, QStringList attributes_values, QString value);
    void parseTagBR(QStringList attributes_names, QStringList attributes_values, QString value);
    void parseTagHR(QStringList attributes_names, QStringList attributes_values);
    void parseTagNormal(QStringList attributes_names, QStringList attributes_values, QString value);
    void parseTagCut(QStringList attributes_names, QStringList attributes_values, QString value);
    void parseTagFont(QStringList attributes_names, QStringList attributes_values, QString value);

    void prePrint(QString* value);
    void alignRight(QString* value);

    QString getMetaTag(const QString& tag);

    int space_to_write;
    int values_to_write;
    QString alignment;

    PrinterSamsungSRP350* printersamsungsrp350;
};

#endif
