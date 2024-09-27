#include "config.h"

const QString Files::BAR_XML{"bar.xml"};
const QString Files::BAR_DATABASE_XML{"bar_database.xml"};
const QString Files::NTPV_BUTTONS_XML{"ntpv_buttons.xml"};
const QString Files::INVITATIONS_DESCRIPTION_XML {"invitations_description.xml"};
const QString Files::BUTTON_STATE_XML {"button_state.xml"};
const QString Files::PASSWD_XML {"passwd"};
const QString Files::DEVICES_CONFIG_XML {"devices_config.xml"};
const QString Files::ORDER_DESCRIPTION_XML {"order_description.xml"};
const QString Files::LOUNGE_XML {"lounges.xml"};

QMap<QString, QString> Files::files{
    {"bar",             Files::BAR_XML},
    {"database",        Files::BAR_DATABASE_XML},
    {"buttons",         Files::NTPV_BUTTONS_XML},
    {"invitations",     Files::INVITATIONS_DESCRIPTION_XML},
    {"button_state",    Files::BUTTON_STATE_XML},
    {"password",        Files::PASSWD_XML},
    {"devices",         Files::DEVICES_CONFIG_XML},
    {"orders",          Files::ORDER_DESCRIPTION_XML},
    {"lounges",         Files::LOUNGE_XML}
};

const QString Files::CONFIG_DIR     {"/etc/ntpv/"};
const QString Files::RessourceDir   {"/usr/share/wpos/"};
const QString Files::PicturesDir    {"/usr/share/wpos/wpos/pictures"};
const QString Files::ProductsDir    {"/usr/share/wpos/wpos/products/"};
const QString Files::ControlsDir    {"/usr/share/wpos/wpos/controls/"};
const QString Files::Controls32Dir  {"/usr/share/wpos/wposbo/32x32/"};
const QString Files::Controls48Dir  {"/usr/share/wpos/wposbo/48x48/"};
const QString Files::Controls64Dir  {"/usr/share/wpos/wposbo/64x64/"};
const QString Files::HandsDir       {"/usr/share/wpos/wposbo/hands/"};
const QString Files::PaymentsDir    {"/usr/share/wpos/wpos/payments/"};

const QString& Files::configFile(const QString& file){
    return QString(CONFIG_DIR).append(file);
}

const QString& Files::baseFile(const QString& key){
    return files[key];
}

// key =  "bar" | "database" | "buttons" | "invitations" | "button_state" ...
QString Files::configFilePath(const QString& key){
    return QString(Files::CONFIG_DIR).append(files[key]);
}
