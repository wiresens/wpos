/***************************************************************************
                          bslnumcodewidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PINCODEWIDGET_H
#define PINCODEWIDGET_H

#include "greeterinterface.h"

#include <QWidget>

namespace Ui {
class PinCodeWidget;
} // namespace Ui

class XmlConfig;
class QSignalMapper;
class QString;

class PinCodeWidget :
    public QWidget,
    public GreeterInterface
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wpos.Authenticator")

public: 
    static const QString DBusObject ;

    explicit PinCodeWidget(QWidget* parent=nullptr, const QString& name = QString{});
    ~PinCodeWidget();

signals:
    void genericDataSignal(const QString& signal, XmlConfig* xml);
    
public slots:
    Q_SCRIPTABLE void getMatchResults(QString _xml_match_data);

protected:
    void showEvent(QShowEvent* event);

private slots:
    void echoPin(const QString& number);
    void clear();
    void accept();
    void cancel();
    void succeeded();
    void aquire();
    void shutdown();

private:
    void setGranted(bool granted);

private:
    QSignalMapper* signalMapper{};
    Ui::PinCodeWidget* ui{};
    QString pinCode;
};

#endif
