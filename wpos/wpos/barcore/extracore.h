/***************************************************************************
                          extracore.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef EXTRACORE_H
#define EXTRACORE_H

#include <QObject>
#include <wposcore/hlist.h>

class XmlConfig;
class QString;
class ProductExtraInfo;

class ProductPriceData;
class OfferData;

class ExtraCore : public QObject  {
    Q_OBJECT
public:
    explicit ExtraCore(QObject *parent=nullptr, const QString& name = QString{});

    QStringList getOptionsNames();
    QString getOptionValue(const QString& option_name);
    QStringList getFixedOptionsNames();
    QString getFixedOptionValue(const QString& option_name);
    bool clearOptionList(bool fixed=false);

    ProductPriceData* getPrice();
    bool setPrice(const ProductPriceData *price_data);
    bool clearPrice();

    bool getPriceMode();
    void setPriceMode(bool state);
    double getModifiedPrice(double price);

    OfferData *getOffer();
    bool setOffer(const OfferData *offer);
    bool clearOffer();
    bool setDefaultOffer(const QString& offer_type,const QString& offer_name);
    void delDefaultOffer();

    void clearExtras();


public slots:
    void genericSignalSlot(const QString& signal_name);
    void genericDataSignalSlot(const QString& signal_name,XmlConfig *xml);

    void readPriceFromXml(XmlConfig *xml);
    void readOptionFromXml(XmlConfig *xml);
    void addProductOption(XmlConfig *xml);
    void readOfferFromXml(XmlConfig *xml);


signals:
    void genericDataSignal(const QString& signal,XmlConfig *xml);

private:
    HList<ProductExtraInfo> options;
    HList<ProductExtraInfo> fixed_options;
    ProductPriceData *custom_price{};
    OfferData *custom_offer{};
    OfferData *default_offer{};
    bool price_state;
};

#endif
