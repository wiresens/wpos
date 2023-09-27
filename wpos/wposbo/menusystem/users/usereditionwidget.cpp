#include "usereditionwidget.h"

#include <libbslxml/xmlconfig.h>
#include "menusystem/system/fx2000enrollwidget.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>

static const QString&  FINGERPRINT_DCOP_MODULE {"dcopfx2000"};

// this should be selected from the database.
const QString& UserEditionWidget::COMPANY_NAME{"1"};
const unsigned int UserEditionWidget::ENROLMENT_TIMEOUT {1000};
const unsigned int UserEditionWidget::FEEDBACK_TIMEOUT{ 2*UserEditionWidget::ENROLMENT_TIMEOUT};

UserEditionWidget::UserEditionWidget(
        UserModule* _user_mod,
        QWidget* parent, const QString& name):
    QWidget(parent), user_mod{_user_mod}
{
    //We will provide a better alternative later
    struct NullPtrUserModuleException{};
    if( !user_mod ) throw NullPtrUserModuleException{};
    setupUi(this);
    setObjectName(name);    

    //ACL's are currently not implemented, so the tab page is not shown
    // will not work until the ACLS are implemented
    // tab_widget->setTabEnabled(acls,false);
//    tab_widget->removeTab(tab_widget->indexOf(acls));
    initializePages();
//    removePage(m_acls_page);
    picture_button->hide(); //We don't use User Photo now

    QHBoxLayout *hlayout = new QHBoxLayout(enroll_frame);
    enroll_widget = new FX2000EnrollWidget(enroll_frame);
    hlayout->addWidget(enroll_widget);

    // Set pixmaps for this widget to avoid ugly black effect from QT when inserting pixmap in ui's
    generic_data_pixmap->setPixmap(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/basic_data_user.png"));
    location_data_pixmap->setPixmap(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/location_data_user.png"));
    info_data_pixmap->setPixmap(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/info.png"));

    cancel_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/button_cancel.png"));
    ok_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/button_ok_48.png"));

    connect(picture_button, &QPushButton::pressed, this, &UserEditionWidget::setUserPicture);
    connect(ok_button, &QPushButton::pressed, this, &UserEditionWidget::apply);
    connect(cancel_button, &QPushButton::pressed, this, &UserEditionWidget::cancel);
}

void UserEditionWidget::setUserPicture(){
    QString filename = QFileDialog::getOpenFileName(this,
                    tr("Choose a photo for the user"), "/usr/share/ntpv",
                    tr("Images (*.png *.xpm *.jpg)"));
    picture_path = filename;
    picture_button->setIcon(QPixmap(picture_path));

}

void UserEditionWidget::clear(){
    name_edit->clear();
    last_name->clear();
    employee_id->clear();
    address->clear();
    cellular->clear();
    phone->clear();
    email->clear();
    nss->clear();
    picture_path.clear();
    clearOutputLabel();
}

void UserEditionWidget::clearOutputLabel(){
    output_label->clear();
}

void UserEditionWidget::showEvent(QShowEvent *e){
    //Template Method design pattern
    doInitialize(); //hook method1

    disconnect(user_mod, &UserModule::enrollDataAvailable, enroll_widget, &FX2000EnrollWidget::fingerprintReadSlot);

    if ( !canBiometricAuth()){
        removePage(m_fingerprints_page);
        fx2000_available = false;
    }
    else{
        fx2000_available = true;
        connect(user_mod, &UserModule::enrollDataAvailable, enroll_widget, &FX2000EnrollWidget::fingerprintReadSlot);
        enroll_widget->setReturnApplication(qApp->applicationName(), user_mod->getInterfaceName(), user_mod->getEnrollMethodName());

        doEnroll(); //hook method2
        addPage(fingerprints, m_fingerprints_page);
        doActualize(); //hook method3
    }
    doFinalize(); //hook method4
    QWidget::showEvent(e);
}

bool UserEditionWidget::canBiometricAuth() const{
//    DCOPClient *client = kapp->dcopClient();
//    return client->isAttached() && client->isApplicationRegistered(FINGERPRINT_DCOP_MODULE);
    //Normaly should be false until the Biometric capability is avaiable.
    // Then the result will depend on the detection of the dcopfx2000 process
    //We et this to true just to see how it is displayed for test purpose.
    //dont forget to switch it back to false when test is finished
    return true;
}

void UserEditionWidget::retrieveUserInfo(XmlConfig& xml){
    xml.createElementSetDomain("user");
    xml.createElement("employee_id", employee_id->text());
    xml.createElement("name", name_edit->text());
    xml.createElement("last_name", last_name->text());
    xml.createElement("address", address->text());
    xml.createElement("nss", nss->text());
    xml.createElement("phone", phone->text());
    xml.createElement("cellular", cellular->text());
    xml.createElement("email", email->text());
    xml.createElement("company_id", COMPANY_NAME);
    xml.createElement("picture", picture_path);

    if (gerente_checkbox->isChecked())
        xml.createElement("gerente", "true");
    else
        xml.createElement("gerente", "false");
    xml.delDomain();
}

void UserEditionWidget::showUserInfo(XmlConfig &xml){
    xml.setDomain("user");
    enroll_widget->setId(xml.readString("employee_id"));
    employee_id->setText(xml.readString("employee_id"));
    name_edit->setText(xml.readString("name"));
    last_name->setText(xml.readString("last_name"));
    address->setText(xml.readString("address"));
    nss->setText(xml.readString("nss"));
    phone->setText(xml.readString("phone"));
    cellular->setText(xml.readString("cellular"));
    email->setText(xml.readString("email"));
    picture_path = xml.readString("picture_path");
    gerente_checkbox->setChecked( xml.readString("gerente") == "true" );

    if (QFile(picture_path).exists()) picture_button->setIcon(QPixmap(picture_path));
}

bool UserEditionWidget::isValidEmail() const{
    return !email->text().isEmpty() && email->text().contains('@');
}

void UserEditionWidget::showMessageBox(const QString& title, const QString& msg, const QString& feedBack){
    QMessageBox::warning(this, title, msg, QMessageBox::Ok);
    if( !feedBack.isEmpty()) showFeedBack(feedBack);
}

void UserEditionWidget::showFeedBack(const QString& feedBack){
    output_label->setText(feedBack);
    QTimer::singleShot(FEEDBACK_TIMEOUT, this, SLOT(clearOutputLabel()));
}

QString UserEditionWidget::userId() const{
    return employee_id->text();
}

QString UserEditionWidget::userFistName() const{
    return name_edit->text();
}

QString UserEditionWidget::userLastName() const{
    return last_name->text();
}

void UserEditionWidget::showPage(const QString& name){
    if( pages.contains(name) )
        tab_widget->setCurrentWidget(pages[name]);
}

void UserEditionWidget::enablePage(const QString& name, bool enabled){
    if( pages.contains(name) )
        tab_widget->setTabEnabled(tab_widget->indexOf(pages[name]), enabled);
}

void UserEditionWidget::addPage(QWidget* page, const QString& name){    
    //Normally should throw exception if already there.
    //For now  we silently ignore
    if( page && !pages.contains(name)){
        tab_widget->addTab(page, name);
        pages[name] = page;
    }
}

void UserEditionWidget::removePage(const QString& name){
    if( pages.contains(name)){
        auto page = pages[name];
        tab_widget->removeTab(tab_widget->indexOf(page));
        pages.remove(name);
    }
}

void UserEditionWidget::initializePages(){
    pages =
    {
        { m_details_page, datos},
        { m_fingerprints_page, fingerprints},
        { m_acls_page, acls}
    };

    page_names =
    {
        { datos, m_details_page},
        { fingerprints, m_fingerprints_page},
        {acls,  m_acls_page}
    };

    for(auto i = 0; i < tab_widget->count(); i++)
        tab_widget->setTabText(i, page_names[tab_widget->widget(i)]);
}

//void UserEditionWidget::removeDuplicates(){

//    //Make sure there is no duplicate in tabs names
//    QMap<QString, QWidget*> tabs;
//    for(auto i = 0; i < tab_widget->count(); i++)
//        tabs[tab_widget->tabText(i)] = tab_widget->widget(i);
//    tab_widget->clear();

//    for( auto name : tabs.keys())
//        tab_widget->addTab(pages[name], name);
//}
