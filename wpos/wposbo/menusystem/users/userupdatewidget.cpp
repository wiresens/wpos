/***************************************************************************
                          bslupdateuser.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "userupdatewidget.h"

#include "menusystem/system/fx2000enrollwidget.h"
#include <libbslxml/xmlconfig.h>

static const QString& USERS_DTD { "dtddocs:users_user.dtd" };

UserUpdateWidget::UserUpdateWidget(
    UserModule* _user_mod,
    QWidget* parent, const QString& name)
    : UserEditionWidget(_user_mod, parent, name)
{

    setCaption(tr("Update User Information"));
    setCancelButtonVisible(false);
    setOkButtonVisible(false);
    setUserIdReadOnly(true);
}

void UserUpdateWidget::setUser(QString user_id)
{

    QString user_info;
    if (user_id.isEmpty() || (user_info = model()->getUserInfo(user_id)).isEmpty())
        return;

    XmlConfig xml;
    xml.readXmlFromString(user_info);
    if (!xml.wellFormed() || !xml.validateXmlWithDTD(USERS_DTD, true))
        return;
    showUserInfo(xml);
}

bool UserUpdateWidget::updateUser()
{

    auto title = tr("failed to update user");
    auto alt_msg = tr("User entered incorrectly");
    if (userId().isEmpty() || userFistName().isEmpty() || userLastName().isEmpty()) {
        auto msg = tr("To be able to update a user you first have to\n introduce at least the three essential fields\n name, surname and ID of the employee\n");
        showMessageBox(title, msg, alt_msg);
        return false;
    }

    // other check like the email or dni length
    if (!isValidEmail()) {
        auto msg = tr("The email field is not correct\n");
        showMessageBox(title, msg, alt_msg);
        return false;
    }

    XmlConfig xml;
    retrieveUserInfo(xml);

    if (!model()->changeUser(xml.toString())) {
        auto msg = tr("An error occurred while trying to update named user :\n");
        msg += userFullName() + "\n";
        msg += tr("with DNI : ") + userId() + tr(" in the database");
        showMessageBox(title, msg);
        return false;
    }

    clear();
    return true;
}

void UserUpdateWidget::doEnroll()
{
    enroller()->setId(userId());
}

// In oder to inherit privately from UserEditionWidget(a.k.a UserEditionForm)
// void UserUpdateWidget::doActualize(){
//     tab_widget->setTabEnabled(fingerprints, true);
// }

// void UserUpdateWidget::doFinalize(){
//     tab_widget->setTabEnabled(datos, true);
//     tab_widget->setCurrentWidget(datos);
// }

void UserUpdateWidget::doActualize()
{
    enablePage(m_fingerprints_page, true);
}

void UserUpdateWidget::doFinalize()
{
    enablePage(m_details_page, true);
    showPage(m_details_page);
}
