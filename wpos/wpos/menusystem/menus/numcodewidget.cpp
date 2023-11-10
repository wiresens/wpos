/****************************************************************************
                          bslnumcodewidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "numcodewidget.h"
#include "numcodewidget_adaptor.h"
#include "mainscreen.h"
#include "salesscreen.h"
#include "auth/authcore.h"

#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>
#include <xmlconfig.h>

#include <QSignalMapper>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QFile>
#include <QPixmap>
#include <QProgressBar>
#include <QTimer>

#include <QDataStream>

extern "C"{
//        #define _XOPEN_SOURCE
#include <unistd.h>
}

#include <iostream>
using namespace std;

#define NUM_DIGITS_FOR_PASSWD 4
#define AUTH_TIME_SLEEP 750
#define nullptr_FINGERPRINT "controls:null_fingerprint.png"
#define BAD_FINGERPRINT "controls:bad_fingerprint.png"

extern AuthCore *auth;

const QString INTERFACE_NAME = "EnterAdminAuth";

const QString NumCodeWidget::DBusObject  = QString{"/wpos/wpos/Authenticator"};

NumCodeWidget::NumCodeWidget(QWidget *parent, const QString& name) :
    QWidget(parent)
{
    setupUi(this);
    setObjectName(name);

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::EVENTLOG, this);

    new AuthenticatorAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(NumCodeWidget::DBusObject, this);

    caption_label->setVisible(false);
    passwd_label->setText(passwd);
    cancel_button->setIcon(QPixmap("controls:button_cancel.png"));
    ok_button->setIcon(QPixmap("controls:button_ok_48.png"));
    shutdown_button->setIcon(QPixmap("controls:exit.png"));
    shutdown_button->setText(tr("Shutdown"));

    signalMapper = new QSignalMapper(this);

    signalMapper->setMapping(numkey_0,"0");
    signalMapper->setMapping(numkey_1,"1");
    signalMapper->setMapping(numkey_2,"2");
    signalMapper->setMapping(numkey_3,"3");
    signalMapper->setMapping(numkey_4,"4");
    signalMapper->setMapping(numkey_5,"5");
    signalMapper->setMapping(numkey_6,"6");
    signalMapper->setMapping(numkey_7,"7");
    signalMapper->setMapping(numkey_8,"8");
    signalMapper->setMapping(numkey_9,"9");

    //connect buttons with the QSignalMapper
    connect(numkey_0,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(numkey_1,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(numkey_2,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(numkey_3,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(numkey_4,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(numkey_5,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(numkey_6,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(numkey_7,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(numkey_8,SIGNAL(released()),signalMapper,SLOT(map()));
    connect(numkey_9,SIGNAL(released()),signalMapper,SLOT(map()));

    connect(signalMapper,SIGNAL(mapped(const QString &)),this,SLOT(mapperSlot(const QString &)));
    connect(aquire_button,SIGNAL(released()),this,SLOT(aquireButtonSlot()));
    connect(ce_button,SIGNAL(released()),this,SLOT(ceButtonSlot()));
    connect(ok_button,SIGNAL(released()),this,SLOT(accept()));
    connect(cancel_button,SIGNAL(released()),this,SLOT(cancel()));
    connect(shutdown_button,SIGNAL(released()),this,SLOT(shutdownSlot()));    
}

NumCodeWidget::~NumCodeWidget(){
    delete signalMapper;
}

//We disable finger print fonctionality because we don't yet have the dcopfx2000 module.
void NumCodeWidget::aquireButtonSlot(){
//    DCOPClient *client= 0;
//    QByteArray conf_data, data;
//    QDataStream out_stream(&data, QIODevice::ReadWrite);
//    QDataStream conf_stream(&conf_data, QIODevice::ReadWrite);

//    client = kapp->dcopClient();
//    if (!client->isAttached())
//        client->attach();

//    if (!client->isApplicationRegistered("dcopfx2000"))
//        return;

//    conf_stream << QString("ntpv");
//    conf_stream << QString(INTERFACE_NAME);
//    conf_stream << QString("getMatchResults(QString)");
//    if (!client->send("dcopfx2000","FX2000Match",
//                      "setReturnApplication(QString,QString,QString)",conf_data)){
//        cerr << "problems while trying to call fx2000" << endl;
//        return;
//    }

//    out_stream << auth->getUserId();

//    if ( !client->send("dcopfx2000","FX2000Match", "asyncMatchIdentity(QString)", data) ){
//        cerr << "problems while trying to call fx2000" << endl;
//        return;
//    }
}

void NumCodeWidget::getMatchResults(QString _xml_match_data){

    XmlConfig xml;
    if ( ! auth || !xml.readXmlFromString(_xml_match_data) )
        return;

    auto identity = xml.readString("identity");
    auto similarity = xml.readString("similarity");
    auto fingerprint_path = xml.readString("fingerprint_path");

    if ( identity == auth->getUserId() ){
        QTimer::singleShot(AUTH_TIME_SLEEP, this,  SLOT(authOk()) );
        similarity_progressbar->setValue( (int) ( similarity.toFloat()*100) );
        fingerprint_label->setPixmap(QPixmap(fingerprint_path));
    }
    else{
        authSlot(false);
        fingerprint_label->setPixmap(QPixmap(BAD_FINGERPRINT));
    }
}

void  NumCodeWidget::authOk(){
    authSlot(true);
}

void NumCodeWidget::mapperSlot(const QString& number){

    if ((passwd_label->text()).length() >= NUM_DIGITS_FOR_PASSWD)
        return;

    passwd_label->setText(passwd_label->text()+"*");
    passwd += number;
}

void NumCodeWidget::ceButtonSlot(){
    similarity_progressbar->setValue(0);
    fingerprint_label->setPixmap(QPixmap(nullptr_FINGERPRINT));
    passwd_label->setText("");
    passwd.clear();
}

void NumCodeWidget::accept(){

    if (passwd_label->text().length() !=  NUM_DIGITS_FOR_PASSWD){
        authSlot(false);
        return;
    }

    QFile p_file (Files::configFilePath("password"));
    p_file.open(QIODevice::ReadOnly);
    auto r_passwd = p_file.readLine(14);
    p_file.close();

    auto c_passwd = QString ( crypt(passwd.toLatin1(), "YY"));
    if( c_passwd == r_passwd )
        authSlot(true);
    else
        authSlot(false);
    ceButtonSlot();
}

void NumCodeWidget::cancel(){
    XmlConfig xml ;
    ceButtonSlot();

    if ( auth  && auth->getUserId() == "1" ){
        xml.createElement("name", MainScreen::LOGIN_SCREEN);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
        return;
    }

    xml.createElement("name", SalesScreen::PRODUCT_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
    xml.deleteElement("name");

    xml.createElement("enabled", "true");
    emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &xml);
}

void NumCodeWidget::authSlot(bool use_auth){
    XmlConfig xml;
    if (use_auth){
        xml.delDomain();
        xml.createElement("name", SalesScreen::ADMIN_MENU);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
        xml.deleteElement("name");
        xml.createElement("event_type", "admin_widget");
        emit genericDataSignal(GDATASIGNAL::EVENTLOG, &xml);
    }
}

//We disable finger print fonctionality because we don't yet have the dcopfx2000 module.
//All following lines relating to fingerprint screen displaying are commented.
//Presently, we just hide the fingerprint screen
void NumCodeWidget::showEvent(QShowEvent *e){
//    DCOPClient *client= 0;
//    //check if the user has administration privileges

    if (!auth) return;
//    client = kapp->dcopClient();
//    if (!client->isAttached())
//        client->attach();

//    if ( auth && auth->getAdmin() && client->isApplicationRegistered("dcopfx2000") ){
//        fingerprint_label->show();
//        similarity_progressbar->show();
//        aquire_button->show();
//    }
//    else{
        fingerprint_label->hide();
        similarity_progressbar->hide();
        aquire_button->hide();
//    }

    ceButtonSlot();
    QWidget::showEvent(e);
}

void NumCodeWidget::shutdownSlot(){
    XmlConfig xml ;
    xml.createElement("name", SalesScreen::EXIT_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
}
