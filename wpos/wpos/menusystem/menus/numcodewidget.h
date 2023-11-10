/***************************************************************************
                          bslnumcodewidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef NUMCODEWIDGET_H
#define NUMCODEWIDGET_H

#include "ui_numcodewidget.h"
#include "loginscreeninterface.h"

#include <QWidget>

class QFile;
class XmlConfig;
class QSignalMapper;
class QString;

class NumCodeWidget :
        public QWidget,
        public LoginScreenInterface ,
        private Ui::NumCodeWidget
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wpos.Authenticator")

public: 
    static const QString DBusObject ;

    explicit NumCodeWidget(QWidget *parent=0, const QString& name = QString());
    ~NumCodeWidget();

signals:
    void genericDataSignal(const QString& signal_name, XmlConfig *xml);
    
public slots:
    void mapperSlot(const QString& number);
    void ceButtonSlot();
    void accept();
    void cancel();
    void authOk();
    void aquireButtonSlot();
    void authSlot(bool use_auth);

    Q_SCRIPTABLE void getMatchResults(QString _xml_match_data);

protected slots:
    void shutdownSlot();

protected:
    void showEvent(QShowEvent *e);

    QSignalMapper *signalMapper;
    QString passwd;
};

#endif
