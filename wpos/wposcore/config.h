#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QMap>

struct Files{

    inline static const QString& configFile(const QString& file){
        return QString(ConfigDir).append(file);
    }

    inline static const QString& baseFile(const QString& key){
        return files[key];
    }

    // key =  "bar" | "database" | "buttons" | "invitations" | "button_state" ...
    inline static QString configFilePath(const QString& key){
        return ConfigDir + "/" + files[key];
    }

    static const QString BAR_XML;
    static const QString BAR_DATABASE_XML;
    static const QString NTPV_BUTTONS_XML;
    static const QString INVITATIONS_DESCRIPTION_XML;
    static const QString BUTTON_STATE_XML;
    static const QString PASSWD_XML;
    static const QString DEVICES_CONFIG_XML;
    static const QString ORDER_DESCRIPTION_XML;
    static const QString LOUNGE_XML;

    static const QString ConfigDir;
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
