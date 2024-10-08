#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QMap>

struct cfg{
    enum class XMLKey : unsigned char{
        Bar             = 0,
        Database        = 1,
        Buttons         = 2,
        Invitations     = 3,
        ButtonState     = 4,
        Password        = 5,
        Devices         = 6,
        Orders          = 7,
        AdvancedOrders  = 8,
        Lounges         = 9,

        Receipt         = 10,

        Printer         = 11,
        PrinterHtml     = 12,
        Invoice         = 13,
        Kitchen         = 12,
        PrinterZeta     = 14,
        PrinterTotal    = 15,




        Defautl         = 255
    };

    enum class DTDKey : unsigned char{
        Printer         = 0,
        Receipt         = 1,


        Defautl         = 255
    };

    static QString xmlFileByName(const QString& file);
    static QString baseFile(XMLKey key);
    static QString baseFile(DTDKey key);

    static QString xmlFileByKey(const QString& key);
    static QString xmlFileByKey(XMLKey key);
    static QString dtdFileByKey(DTDKey key);

    static const QString BAR_XML;
    static const QString DATABASE_XML;
    static const QString BUTTONS_XML;
    static const QString INVITATIONS_XML;
    static const QString BUTTONS_STATE_XML;
    static const QString PASSWD;
    static const QString DEVICES_XML;
    static const QString ORDERS_XML;
    static const QString ADVANCED_ORDERS_XML;
    static const QString LOUNGE_XML;

    // dbusreceipt
    static const QString RECEIPT_XML;
    static const QString RECEIPT_DTD;

    //dbusprinter
    static const QString PRINTER_XML;
    static const QString PRINTER_DTD;

    static const QString PRINTER_HTML_XML;
    static const QString INVOICE_XML;
    static const QString KITCHEN_HTML_XML;
    static const QString PRINTER_ZETA_XML;
    static const QString PRINTER_TOTAL_XML;

    static const QString CFG_XML_DIR;
    static const QString CFG_DTD_DIR;
    static const QString RESOURCE_DIR;
    static const QString PICTURE_DIR;
    static const QString PRODUCT_DIR;
    static const QString CONTROLS_DIR;
    static const QString CONTROLS_32_DIR;
    static const QString CONTROLS_48_DIR;
    static const QString CONTROLS_64_DIR;
    static const QString HANDS_DIR;
    static const QString PAYMENT_DIR;

private:
    static QMap<DTDKey, QString> m_dtd_key_map;
    static QMap<XMLKey, QString> m_xml_key_map;
};

#endif // CONFIG_H
