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

const QString Files::ConfigDirectory{"/etc/ntpv"};
const QString Files::ImageDirectory{"/usr/share/ntpv/apps"};
