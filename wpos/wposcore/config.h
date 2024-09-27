#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QMap>

struct Files{

    static const QString& configFile(const QString& file);
    static const QString& baseFile(const QString& key);
    // key =  "bar" | "database" | "buttons" | "invitations" | "button_state" ...
    static QString configFilePath(const QString& key);

    static const QString BAR_XML;
    static const QString BAR_DATABASE_XML;
    static const QString NTPV_BUTTONS_XML;
    static const QString INVITATIONS_DESCRIPTION_XML;
    static const QString BUTTON_STATE_XML;
    static const QString PASSWD_XML;
    static const QString DEVICES_CONFIG_XML;
    static const QString ORDER_DESCRIPTION_XML;
    static const QString LOUNGE_XML;

    static const QString CONFIG_DIR;
    static const QString RessourceDir;
    static const QString PicturesDir;
    static const QString ProductsDir;
    static const QString ControlsDir;
    static const QString Controls32Dir;
    static const QString Controls48Dir;
    static const QString Controls64Dir;
    static const QString HandsDir;
    static const QString PaymentsDir;

private:
    static QMap<QString, QString> files;
};

#endif // CONFIG_H
