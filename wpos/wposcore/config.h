#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QMap>

struct Files{

    inline static const QString& configFile(const QString& file){
        return QString(ConfigDirectory).append(file);
    }

    inline static const QString& baseFile(const QString& key){
        return files[key];
    }

    // key =  "bar" | "database" | "buttons" | "invitations" | "button_state" ...
    inline static QString configFilePath(const QString& key){
        return ConfigDirectory + "/" + files[key];
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

    static const QString ConfigDirectory;

private:
    static QMap<QString, QString> files;
    static const QString ImageDirectory;
};

#endif // CONFIG_H
