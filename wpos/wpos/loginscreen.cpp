/***************************************************************************
                          bslenterthematrix.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "loginscreen.h"
#include "loginscreen_adaptor.h"

#include "mainscreen.h"
#include "database/authcoredb.h"

#include <wposwidget/menupage.h>
#include <wposcore/genericsignalmanager.h>
#include <wposcore/config.h>
#include <libbslxml/xmlconfig.h>

#include <QDataStream>
#include <QPushButton>
#include <QLayout>
#include <QButtonGroup>
#include <QPixmap>
#include <QFile>
#include <QString>
#include <QLabel>
#include <QTimer>
#include <QPixmap>
#include <QStyleFactory>
#include <QStyle>
#include <QApplication>
#include <QFont>

extern "C"{
#include "unistd.h"
}

#include <iostream>
using namespace std;

const QString& BLACK_FINGERPRINT    = "controls:black_fingerprint.jpg";
const QString& BAD_FINGERPRINT      = "controls:bad_fingerprint.png";
const QString& LOGO_CORP            = "controls:napsis_logo.png";
const QString& DEFAULT_PICTURE      = "controls:fondo.png";
const QString& DEFAULT_TEXT         = "WireSens";

const int SLEEP_TIME                = 1500;
const int MAX_LAYOUT                = 5;
const int DEFAULT_FONT_SIZE         = 12;

const QString LoginScreen::DBusObject{"/wpos/wpos/Greeter"};

LoginScreen::LoginScreen(MenuPage* parent, const QString& name):
    QWidget(parent),
    db  {new AuthCoreDB("LoginScreen", Files::configFilePath("database"))}
{
    setupUi(this);
    setObjectName(name);
    parent->addWidget(this, objectName());

    new GreeterAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(LoginScreen::DBusObject , this);

    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    auto gsm = GenericSignalManager::instance();
    gsm->publishGenericDataSignal(GDATASIGNAL::CHANGE_USER, this);
    gsm->publishGenericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, this);
    gsm->subscribeToGenericSignal(GSIGNAL::AUTH_ENABLE_FINGERPRINT, this);
    gsm->subscribeToGenericSignal(GSIGNAL::AUTH_DISABLE_FINGERPRINT, this);
    gsm->subscribeToGenericSignal(GSIGNAL::LOAD_BUTTONS, this);

    loadPicture();
    createLoginButtons();
}

LoginScreen::~LoginScreen(){
    delete db;
}

void LoginScreen::loadPicture(){

    QString font_family;
    QString font_size_str;
    int font_size = 11;
    bool ok = false;
    int R,G,B;
    int BR,BG,BB;
    //    QStyle *st {};
    QFont font;
    QString pixmap_file, company_name, app_style;
    XmlConfig xml(Files::configFilePath("buttons")) ;

    if ( xml.setDomain("initscreen") ){
        pixmap_file = xml.readString("pixmap");
        company_name = xml.readString("company_text");

        app_style = xml.readString("style");
        if (!app_style.isEmpty()){
            QStyle * tmp_app_style = QStyleFactory::create(app_style);
            if (tmp_app_style) QApplication::setStyle(tmp_app_style);
        }

        if (xml.setDomain("font")){
            font_family = xml.readString("family");
            font_size_str = xml.readString("size");
            xml.releaseDomain("font");
            if (!font_family.isEmpty()){
                font_size = font_size_str.toInt(&ok);
                if (ok) font = QFont(font_family, font_size);
                else    font = QFont(font_family, DEFAULT_FONT_SIZE);
                QApplication::setFont(font);
            }
        }

        //There is a problem here. The xml.readInt(tag) internally calls
        //readString(tag).toInt() which may fail to convert the tag value
        //into an integer if tag was not a string convertible to and integer
        //and the resultant value will be unreliable! This is a potential bug

        //It may be better to set theses values as a single RGB string like #FFFAAB
        //in the xml file/
        if (xml.setDomain("backgroundcolor")){
            BR = xml.readInt("red");
            BG = xml.readInt("green");
            BB = xml.readInt("blue");
            xml.releaseDomain("backgroundcolor");
            auto bgpalette = QPalette(QPalette::Window);
            bgpalette.setColor(QPalette::Window, QColor(BR,BG,BB) );
            setPalette(bgpalette);
        }

        if (xml.setDomain("foregroundcolor")){
            R = xml.readInt("red");
            G = xml.readInt("green");
            B = xml.readInt("blue");
            xml.releaseDomain("foregroundcolor");
            auto fgpalette = QPalette(QPalette::WindowText);
            fgpalette.setColor(QPalette::WindowText, QColor(R,G,B) );
            setPalette(fgpalette);
        }

    }
    xml.releaseDomain("initscreen");

    QString  tmp_pixmap_file = DEFAULT_PICTURE;
    if (  !pixmap_file.isEmpty() && QFile(pixmap_file).exists() )
        tmp_pixmap_file = pixmap_file;

    login_pixmap_label->setPixmap(QPixmap(tmp_pixmap_file));

    if (!company_name.isEmpty())
        local_label->setText(company_name);
    else
        local_label->setText(DEFAULT_TEXT);

    fingerprint_label->setPixmap(QPixmap(BLACK_FINGERPRINT));

    //In the future we will dynamically hide or show the fingerprint
    //when this fonctionallity is available after checking availability of the device
    //Presently we just hide unconditionally
    fingerprint_label->hide();
}

void LoginScreen::showEvent(QShowEvent *e){
    (void) e ;
    //These have been disabled, we dont see the need. This is waste of computing ressources
    //    fingerprint_label->setPixmap(QPixmap(BLACK_FINGERPRINT));
    QWidget::showEvent(e);
}

void LoginScreen::createLoginButtons(){

    QGridLayout *layout{};
    UserData *user {};
    QPushButton *button{};
    HList<UserData> *ulist{} ;

    if ( !db->connect() ||
        !( ulist = db->getUsers() ) ||
        !db->disConnect() ) return;

    buttons.clear();
    if ( (layout = qobject_cast<QGridLayout*>(users_buttongroup->layout())) )
        delete layout;

    int cols  {MAX_LAYOUT};
    if ( ulist->count() < MAX_LAYOUT)
        cols = ulist->count();

    layout = new QGridLayout(users_buttongroup);
    layout->setSpacing(10);
    layout->setMargin(10);

    for( auto i = 0; i < ulist->count(); i++){
        user = ulist->at(i);
        auto user_name = user->name.trimmed().replace(" ","\n");
        button = new QPushButton( user_name , users_buttongroup);
        button->setObjectName(user->id);

        if ( i==0 )  button->setIcon(QPixmap(LOGO_CORP));

        button->setFixedSize(LOGIN_BUTTON_SIZE);
        button->setToolTip(QString("Connect to the system as %1").arg(user_name));

        buttons.append( button, button->objectName() );
        layout->addWidget(button, i / cols, i % cols);
        //layout->setAlignment(button , Qt::AlignBottom);

        connect(button, &QPushButton::pressed, this, &LoginScreen::handleButtonPressed);
        button->show();
    }
}

void LoginScreen::genericSignalSlot(const QString& signal_name){
    if (signal_name == GSIGNAL::AUTH_ENABLE_FINGERPRINT){
        use_auth = false;
    }
    else if (signal_name == GSIGNAL::AUTH_DISABLE_FINGERPRINT){
        use_auth = true;
    }
    else if (signal_name == GSIGNAL::LOAD_BUTTONS){
        loadPicture();
    }
}

void LoginScreen::handleButtonPressed(){

    QObject *sender = qobject_cast<QObject *>( QObject::sender() );
    if (!sender) return;

    QString id = sender->objectName();
    //used to set if the fingerpring auth must be used or not
    if ( !use_auth || id == "1" ){
        registerUser(id);
        return;
    }

    //    client = kapp->dcopClient();
    //    //only to use at the moment.
    //    //this code is going to be deleted from here

    //    if (!client->isAttached())
    //        client->attach();

    //    if (!client->isApplicationRegistered("dcopfx2000"))
    //        return;

    //    //prepare the return data...


    //    conf_stream << QString("ntpv");
    //    conf_stream << QString(INTERFACE_NAME);
    //    conf_stream << QString("getMatchResults(QString)");
    //    if (!client->send("dcopfx2000","FX2000Match",
    //                      "setReturnApplication(QString,QString,QString)",conf_data)){
    //        cerr << "problems while trying to call fx2000" << endl;
    //        return;
    //    }

    //    out_stream << id;


    //    if (!client->send("dcopfx2000","FX2000Match","asyncMatchIdentity(QString)",
    //                      data)){
    //        cerr << "problems while trying to call fx2000" << endl;
    //        return;
    //    }
}

void LoginScreen::registerUser(const QString& user_id){

    XmlConfig xml;
    xml.createElement("id", user_id);
    emit genericDataSignal(GDATASIGNAL::CHANGE_USER, &xml);
    xml.deleteElement("id");

    xml.createElement("name", MainScreen::SALES_SCREEN);
    emit genericDataSignal(GDATASIGNAL::MAINSTACK_SETPAGE, &xml);
}

void LoginScreen::getMatchResults(QString _xml_match_data){

    XmlConfig xml;
    if (!xml.readXmlFromString(_xml_match_data)) return;

    auto identity = xml.readString("identity");

    if (!identity.isEmpty()){       //has been detected
        //set the fingerprint at the screen
        registerUser(identity);
    }
    else{
        fingerprint_label->setPixmap(QPixmap(BAD_FINGERPRINT));
        QTimer::singleShot(SLEEP_TIME, this, &LoginScreen::flushFingerprintPixmap);
    }
}

void LoginScreen::flushFingerprintPixmap(){
    fingerprint_label->setPixmap(QPixmap(BLACK_FINGERPRINT));
    fingerprint_label->show();
}

//void BslEnterTheMatrix::keyPressEvent(QKeyEvent *e){
//        cout << "TECLA ASCII [ " << e->ascii()<<"] numero ["<< e->key()<<"] "<< endl;
//}
