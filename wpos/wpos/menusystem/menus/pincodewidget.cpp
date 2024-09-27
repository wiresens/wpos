/****************************************************************************
                          pincodewidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

 ***************************************************************************/

#include "ui_pincodewidget.h"
#include "pincodewidget.h"
#include "pincodewidget_adaptor.h"
#include "mainscreen.h"
#include "salesscreen.h"
#include "auth/authcore.h"

#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>
#include <wposcore/authentication/crypto_hasher.h>
#include <xmlconfig.h>

extern AuthCore *authCore;

static const uint PIN_CODE_LENGTH {4};
static const uint AUTH_TIME_SLEEP {750};

static const QString NULL_FINGERPRINT {"controls:null_fingerprint.png"};
static const QString BAD_FINGERPRINT {"controls:bad_fingerprint.png"};
static const QString ROOT_ID {"1"};

const QString PinCodeWidget::DBusObject{"/wpos/wpos/Authenticator"};

PinCodeWidget::PinCodeWidget(
    QWidget *parent,
    const QString& name):
    QWidget(parent)
{
    ui = new Ui::PinCodeWidget();
    ui->setupUi(this);
    setObjectName(name);

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::EVENTLOG, this);

    new AuthenticatorAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(PinCodeWidget::DBusObject, this);

    ui->caption_label->setVisible(false);
    ui->passwd_label->clear();
    ui->cancel_button->setIcon(QPixmap("controls:button_cancel.png"));
    ui->ok_button->setIcon(QPixmap("controls:button_ok_48.png"));
    ui->shutdown_button->setIcon(QPixmap("controls:exit.png"));
    ui->shutdown_button->setText(tr("Shutdown"));

    signalMapper = new QSignalMapper(this);

    signalMapper->setMapping(ui->numkey_0, "0");
    signalMapper->setMapping(ui->numkey_1, "1");
    signalMapper->setMapping(ui->numkey_2, "2");
    signalMapper->setMapping(ui->numkey_3, "3");
    signalMapper->setMapping(ui->numkey_4, "4");
    signalMapper->setMapping(ui->numkey_5, "5");
    signalMapper->setMapping(ui->numkey_6, "6");
    signalMapper->setMapping(ui->numkey_7, "7");
    signalMapper->setMapping(ui->numkey_8, "8");
    signalMapper->setMapping(ui->numkey_9, "9");

    //connect buttons with the QSignalMapper
    connect(ui->numkey_0, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->numkey_1, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->numkey_2, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->numkey_3, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->numkey_4, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->numkey_5, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->numkey_6, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->numkey_7, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->numkey_8, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));
    connect(ui->numkey_9, &QPushButton::released, signalMapper, QOverload<>::of(&QSignalMapper::map));

    connect(ui->aquire_button, &QPushButton::released, this, &PinCodeWidget::aquire);
    connect(ui->ce_button, &QPushButton::released, this, &PinCodeWidget::clear);
    connect(ui->ok_button, &QPushButton::released, this, &PinCodeWidget::accept);
    connect(ui->cancel_button, &QPushButton::released, this, &PinCodeWidget::cancel);
    connect(ui->shutdown_button, &QPushButton::released, this, &PinCodeWidget::shutdown);

    //prepare the map signal
    connect(signalMapper, &QSignalMapper::mappedString, this, &PinCodeWidget::echoPin);
}

PinCodeWidget::~PinCodeWidget(){
    signalMapper->deleteLater();
    delete ui;
}

// We disable finger print fonctionality because
// we don't yet have the dcopfx2000 module.
void PinCodeWidget::aquire(){
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

void PinCodeWidget::getMatchResults(QString _xml_match_data){

    XmlConfig xml;
    if ( !authCore || !xml.readXmlFromString(_xml_match_data) )  return;

    auto identity = xml.readString("identity");
    auto similarity = xml.readString("similarity");
    auto fingerprint_path = xml.readString("fingerprint_path");

    if ( identity == authCore->userId() ){
        QTimer::singleShot(AUTH_TIME_SLEEP, this,  &PinCodeWidget::succeeded);
        ui->similarity_progressbar->setValue( (int) ( similarity.toFloat()*100) );
        ui->fingerprint_label->setPixmap(QPixmap(fingerprint_path));
    }
    else{
        setGranted(false);
        ui->fingerprint_label->setPixmap(QPixmap(BAD_FINGERPRINT));
    }
}

void PinCodeWidget::echoPin(const QString& number){

    if ( ui->passwd_label->text().length() >= PIN_CODE_LENGTH) return;

    ui->passwd_label->setText(ui->passwd_label->text()+"*");
    pinCode += number;
}

void PinCodeWidget::clear(){
    ui->similarity_progressbar->setValue(0);
    ui->fingerprint_label->setPixmap(QPixmap(NULL_FINGERPRINT));
    ui->passwd_label->setText("");
    pinCode.clear();
}

void PinCodeWidget::accept(){

    if (ui->passwd_label->text().length() !=  PIN_CODE_LENGTH){
        setGranted(false);
        return;
    }

    QFile password_file (Files::configFilePath("password"));
    password_file.open(QIODevice::ReadOnly);
    auto encrypted_passwd = password_file.readLine(33);
    password_file.close();

    MD5CryptoHasher hasher;
    if( hasher.hash(pinCode) == encrypted_passwd) setGranted(true);
    else setGranted(false);
    clear();
}

void PinCodeWidget::cancel(){
    clear();

    XmlConfig xml ;
    if ( authCore  && authCore->userId() == ROOT_ID ){   // go to loginscreen if root user
        xml.createElement("name", MainScreen::LOGIN_SCREEN);
        emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
        return;
    }

    // Not root user, return to salesscreen
    xml.createElement("name", SalesScreen::PRODUCT_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
    xml.deleteElement("name");

    xml.createElement("enabled", "true");
    emit genericDataSignal(GDATASIGNAL::MAINWIDGET_SETENABLED, &xml);
}

void  PinCodeWidget::succeeded(){
    setGranted(true);
}

void PinCodeWidget::setGranted(bool granted){
    XmlConfig xml;
    if (granted){
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
void PinCodeWidget::showEvent(QShowEvent *event){
//    DCOPClient *client= 0;
//    //check if the user has administration privileges

    if (!authCore) return;
//    client = kapp->dcopClient();
//    if (!client->isAttached())
//        client->attach();

//    if ( auth && auth->getAdmin() && client->isApplicationRegistered("dcopfx2000") ){
//        fingerprint_label->show();
//        similarity_progressbar->show();
//        aquire_button->show();
//    }
//    else{
        // ui->fingerprint_label->hide();
        // ui->similarity_progressbar->hide();
        // ui->aquire_button->hide();
//    }

    clear();
    QWidget::showEvent(event);
}

void PinCodeWidget::shutdown(){
    XmlConfig xml ;
    xml.createElement("name", SalesScreen::EXIT_MENU);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
}
