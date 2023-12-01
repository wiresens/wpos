/***************************************************************************
                          barcore.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%


        this class is the core of the ntpv application so it should be documented like an API
        most of the methods will be slots, and it will emit signals to admit that changes have been taken
        The core depends on an XmlConfig class to contain all the data. It will also send and recieve data from
        other XmlConfig objects, but it must follow the scheme located at the end of this file.
        Think in this class as the glue that keep the rest of parts of the program interacting between themselves.
 ***************************************************************************/

#ifndef BARCORE_H
#define BARCORE_H

#include <QObject>

class ExtraCore;
class QString;
class BarCoreDB;
class DBusReceiptQuerier;
class XmlConfig;

class BarCore : public QObject  {
    Q_OBJECT
public:
    explicit BarCore(QObject *parent=nullptr, const QString& name = QString());
    ~BarCore();
    /**
     * prepare a new XML and set on it the timestamp, employee, and locationInfo
     */

    void reInitialise(bool flush_last = true);
    double getBillingSum();
    bool hasProducts();
    bool hasProducts(XmlConfig *_xml);

    bool exitAndSaveReceipt();

public slots:
    void genericDataSignalSlot(const QString& signal_name, XmlConfig *_xml);
    void genericSignalSlot(const QString& signal_name);

    void receiveProduct(XmlConfig *_xml);
    void receiveProduct(XmlConfig *_xml, int number);

    void processCore(const QString& pay_type);
    void initExtras();
    void setLastReceipt();
    void setDescription(const QString& description);

    void debug();
    void dataChangedSlot(XmlConfig *_xml);

signals:
    void dataChanged(XmlConfig *_xml);
    void ticket(XmlConfig *_xml);
    void genericSignal(const QString& signal_name);

protected:
    //module info reception
    void setProductPrinterSection(const QString& product_code);
    void setEmployeeInfo();
    void setLocationInfo();
    void setTimeStamp(const QString& name);
    bool setProduct(XmlConfig *product);
    QString getProductCode(XmlConfig *xml);

    void sendCurrentOrderToKitchen();

    //product reception
    void setProductName(XmlConfig *_xml, const QString& product_code);
    void setPrice(XmlConfig *_xml, const QString& product_code);
    bool setOptions(XmlConfig *_xml, const QString& product_code);
    bool checkOffers(XmlConfig *_xml, const QString& product_code);

    void saveLastReceipt();

private:
    void resetCore();

private:
    XmlConfig *xml{};
    ExtraCore *extra_core{};
    DBusReceiptQuerier *receipt_com{};

    QString last_employee_id;
    QString last_start_time;
    static BarCoreDB db;
};

#endif
