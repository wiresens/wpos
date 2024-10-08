#include "config.h"

const QString cfg::BAR_XML              {"bar.xml"};
const QString cfg::DATABASE_XML         {"bar_database.xml"};
const QString cfg::BUTTONS_XML          {"ntpv_buttons.xml"};
const QString cfg::INVITATIONS_XML      {"invitations_description.xml"};
const QString cfg::BUTTONS_STATE_XML    {"button_state.xml"};
const QString cfg::PASSWD               {"passwd"};
const QString cfg::DEVICES_XML          {"devices_config.xml"};
const QString cfg::ORDERS_XML           {"order_description.xml"};
const QString cfg::ADVANCED_ORDERS_XML  {"advanced_order_description.xml"};
const QString cfg::LOUNGE_XML           {"lounges.xml"};

// dbusreceipt
const QString cfg::RECEIPT_XML           {"dcopreceipt_config.xml"};
const QString cfg::RECEIPT_DTD           {"dcopreceipt_config.dtd"};

// dbusprinter
const QString cfg::PRINTER_XML           {"dcopprinter_config.xml"};
const QString cfg::PRINTER_DTD           {"dcopprinter_config.dtd"};

const QString cfg::PRINTER_HTML_XML      {"printerhtml.xml"};
const QString cfg::INVOICE_XML           {"invoice.xml"};
const QString cfg::KITCHEN_HTML_XML      {"kitchen_printerhtml.xml"};
const QString cfg::PRINTER_ZETA_XML      {"printerz.xml"};
const QString cfg::PRINTER_TOTAL_XML     {"printertickettotal.xml"};


// key =  "printer_dtd" | "receipt_dtd" | "dbmodule_dtd" ..
QMap<cfg::DTDKey, QString>   cfg::m_dtd_key_map{
    {DTDKey::Printer,        cfg::PRINTER_DTD},
    {DTDKey::Receipt,        cfg::RECEIPT_DTD}
};

QMap<cfg::XMLKey, QString>   cfg::m_xml_key_map{
    {XMLKey::Bar,            cfg::BAR_XML},
    {XMLKey::Database,       cfg::DATABASE_XML},
    {XMLKey::Buttons,        cfg::BUTTONS_XML},
    {XMLKey::Invitations,    cfg::INVITATIONS_XML},
    {XMLKey::ButtonState,    cfg::BUTTONS_STATE_XML},
    {XMLKey::Password,       cfg::PASSWD},
    {XMLKey::Devices,        cfg::DEVICES_XML},
    {XMLKey::Orders,         cfg::ORDERS_XML},
    {XMLKey::AdvancedOrders, cfg::ADVANCED_ORDERS_XML},
    {XMLKey::Lounges,        cfg::LOUNGE_XML},
    {XMLKey::Receipt,        cfg::RECEIPT_XML},
    {XMLKey::Printer,        cfg::PRINTER_XML},
    {XMLKey::PrinterHtml,    cfg::PRINTER_HTML_XML},
    {XMLKey::Invoice,        cfg::INVOICE_XML},
    {XMLKey::Kitchen,        cfg::KITCHEN_HTML_XML},
    {XMLKey::PrinterZeta,    cfg::PRINTER_ZETA_XML},
    {XMLKey::PrinterTotal,   cfg::PRINTER_TOTAL_XML}
};

const QString cfg::CFG_XML_DIR   {"xmldocs:/etc/wpos/wpos/"};
const QString cfg::CFG_DTD_DIR   {"dtddocs:/etc/wpos/wpos/dtds/"};

const QString cfg::PRODUCT_DIR    {"share/wpos/wpos/products/"};
const QString cfg::CONTROLS_DIR   {"share/wpos/wpos/controls/"};
const QString cfg::PAYMENT_DIR    {"share/wpos/wpos/payments/"};

const QString cfg::RESOURCE_DIR   {"/usr/share/wpos/"};
const QString cfg::PICTURE_DIR    {"/usr/share/wpos/wpos/pictures/"};
const QString cfg::CONTROLS_32_DIR{"/usr/share/wpos/wposbo/32x32/"};
const QString cfg::CONTROLS_48_DIR{"/usr/share/wpos/wposbo/48x48/"};
const QString cfg::CONTROLS_64_DIR{"/usr/share/wpos/wposbo/64x64/"};
const QString cfg::HANDS_DIR      {"/usr/share/wpos/wposbo/hands/"};


QString cfg::xmlFileByName(const QString& file){
    return QString(CFG_XML_DIR).append(file);
}

QString cfg::baseFile(XMLKey key){
    return m_xml_key_map[key];
}

QString cfg::baseFile(DTDKey key){
    return m_dtd_key_map[key];
}

QString cfg::xmlFileByKey(XMLKey key){
    return  QString(cfg::CFG_XML_DIR).append(m_xml_key_map[key]);
}

QString cfg::dtdFileByKey(DTDKey key){
    return QString(cfg::CFG_DTD_DIR).append(m_dtd_key_map[key]);
}
