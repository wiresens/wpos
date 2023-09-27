/***************************************************************************
                          cashbox.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright             : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef CASH_BOX_H
#define CASH_BOX_H

#include <QObject>

class XmlConfig;

class CashDevice : public QObject  {
    Q_OBJECT
public:
    explicit CashDevice(QObject *parent, const QString& name = QString());
    virtual ~CashDevice();

    virtual void initCashDevice();
    virtual bool open(const QString& cashbox_dev, const QString& cashbox_type);
    //     virtual bool isOpen();
    //     virtual void close();

public slots:
    void genericSignalSlot(const QString& signal_name);

signals:
    void genericDataSignal(const QString& signal_name, XmlConfig *xml);
};

#endif
