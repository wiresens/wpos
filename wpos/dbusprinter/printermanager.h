/***************************************************************************
                          printermanager.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Pablo Fernandez
modified by Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PRINTERMANAGER_H
#define PRINTERMANAGER_H

#define XML_TICKET_HTML_PATH "/etc/ntpv/printerhtml.xml"
#define XML_INVOICE_HTML_PATH "/etc/ntpv/invoice.xml"
#define XML_KITCHEN_HTML_PATH "/etc/ntpv/kitchen_printerhtml.xml"
#define XML_Z_HTML_PATH "/etc/ntpv/printerz.xml"
#define XML_TICKET_TOTAL_HTML_PATH "/etc/ntpv/printertickettotal.xml"

#include <QList>

class QString;
class XmlConfig;

struct ProductData {
    QString name;
    QString price;
    QString options;
    int iva;
    float iva_percentage;
    int quantity;
    bool already_printed;
};

class ProductDataList {
public:
    void addProduct(const QString& name,
                    const QString& price,
                    int iva,
                    float iva_percentage);

    void addProduct(const QString& name,
                    const QString& price,
                    int iva,
                    float iva_percentage,
                    const QString& options,
                    bool already_printed=false);

    ProductData* productNth(int i);
    int length();
    void clear();

protected:
    QList<ProductData*> list;
};

class PrinterManager {
public:
    PrinterManager(const QString& _type, const QString& _device,const QString& _user="",const QString& _passwd="");
    ~PrinterManager();

    void setXmlPartHTML(XmlConfig* xml);
    void setXmlPartData(XmlConfig* xml);

    void printTicket(XmlConfig* xml, int times=1, bool print=true, bool dont_change_html=false);
    void printInvoice(XmlConfig* xml, int times=1, bool print=true, bool dont_change_html=false);
    void printX(XmlConfig* xml, bool print=true, bool dont_change_html=false);
    void printZ(XmlConfig* xml, bool print=true, bool dont_change_html=false);
    void printTicketTotal(XmlConfig* xml, bool print=true, bool dont_change_html=false);
    void printKitchenOrder(XmlConfig* xml, bool print=true, bool dont_change_html=false);

    void addDataTag(const QString& subsection, const QString& name, const QString &value);
    void addPriceDataTag(const QString& subsection);
    void addIVADataTags(const QString& subsection);
    void addProductsTags(XmlConfig* xml,bool printed_option=false);
    void addPayTypesTags(XmlConfig* xml);
    void addCamarerosTags(XmlConfig* xml);
    void addOffersTags(XmlConfig* xml, const QString& employee_name);
    void addOptionsTags(XmlConfig* xml, const QString& employee_name);
    void addTotalOffersTags(XmlConfig* xml);
    void addTotalOptionsTags(XmlConfig* xml);
    void addZProductsTags(XmlConfig* xml);
    QString getValueFromXml(XmlConfig* xml, const QString& path);
    int findSubsection(const QString& subsection, bool create=false);
    void copy (XmlConfig* xml, XmlConfig **dest, QString domain);

    void print(int copies=1);
    void debug();

    //public slots:
    //  void printTicketSlot (XmlConfig* xml);

protected:
    void printXZ (XmlConfig* xml, const QString& type, bool print=true, bool dont_change_html=false);
    //prepare the printing, get an xml with all the things that are going to be printed by PrinterTicket
    XmlConfig* preparePrinting();
    void realPrint(XmlConfig* xml, int copies=1);

    XmlConfig* xml_html;
    XmlConfig* xml_data;
    XmlConfig *products;
    XmlConfig *paytypes;
    XmlConfig *camareros;
    XmlConfig *offers;
    XmlConfig *options;
    XmlConfig *total_offers;
    XmlConfig *total_options;

    QString action; /* Variable so we can know what we are doing (Z, ticket, etc.) */
    ProductDataList products_list;

    /* The XML we will give to print will be like this:
   * <ticket>
   *  <sections>
   *    <section name="printerRepresentation">
   *      %xml_html%
   *    </section>
   *    <section name="definitions">
   *      %xml_data%
   *      <item name="command">
   *        %products%
   *      </item>
   *      <item name="totalPrice">
   *        <data type="string" name="totalPrice">$totalPrice</data>
   *      </item>
   *    </section>
   *  </sections>
   * </ticket>
   */
    QString device;
    QString type;
    QString user;
    QString passwd;
};

#endif
