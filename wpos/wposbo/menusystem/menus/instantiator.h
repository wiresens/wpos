/***************************************************************************
                          bslinstantiator.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef INSTANTIATOR_H
#define INSTANTIATOR_H

#include <wposcore/hlist.h>
#include <QObject>
#include <QWidget>

//class XmlConfig;

class UserModule;
class DatabaseModule;
class ProductModule;

class Instantiator : public QObject{
    Q_OBJECT
public:
    explicit Instantiator(QWidget* parent=0, const QString & name= QString());
    ~Instantiator() = default;

    bool hasMenu(const QString& menu_name);

public slots:
    QWidget *getMenu(QWidget *parent, const QString &menu_name); //@benes , maybe getSubMenu
    void showMenu(const QString& page_name);

signals:
    void showPageMenu(const QString& page_name);
    void splashRequested(const QString &str, int flags, const QColor &color);

protected:
    void initDcopModules();
    void initSysConfMenu();
    void initUsersMenu();
    void initProductsMenu();
    void initDesignMenu();
    void initOffersAndOptionsMenu();
    void initHistoricMenu();
    void initReportMenu();
    void initSupportMenu();
    void initStickMenu();
    
    //   XmlConfig *xml_description;
    HList<QWidget> *menus{};
    QWidget *parent{};
    UserModule *user_mod{};
    DatabaseModule *db_mod{};
    ProductModule *product_mod{};

    Qt::Alignment splash_align {Qt::AlignBottom | Qt::AlignRight};
    Qt::GlobalColor splash_color = Qt::darkBlue;
};

#endif
