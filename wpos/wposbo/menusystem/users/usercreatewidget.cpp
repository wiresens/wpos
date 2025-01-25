/***************************************************************************
                          bsladduserswidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "usercreatewidget.h"

#include "menusystem/system/fx2000enrollwidget.h"
#include <libbslxml/xmlconfig.h>

#include <QMessageBox>
#include <QTimer>

UserCreationWidget::UserCreationWidget(
    UserModule* _user_mod,
    QWidget* parent, const QString& name)
    : UserEditionWidget(_user_mod, parent, name)
{
}

void UserCreationWidget::apply()
{
    // Implement the State design patern here
    switch (state) {
    case State::Creation: // create user phase

    {
        auto title = tr("Failed to insert the user");
        auto alt_msg = tr("User entered incorrectly");
        if (userId().isEmpty() || userFistName().isEmpty() || userLastName().isEmpty()) {
            auto msg = tr("To be able to insert a user you first have to\n introduce at least the three essential fields name, surname and ID of the employee\n\n");
            showMessageBox(title, msg, alt_msg);
            return;
        }

        // other check like the email or dni length
        if (!isValidEmail()) {
            QString msg = tr("The email field is not correct\n");
            showMessageBox(title, msg, alt_msg);
        }

        XmlConfig xml;
        retrieveUserInfo(xml);
        if (!model()->createUser(xml.toString())) {
            auto msg = tr("An error occurred while trying to insert named user :\n");
            msg += userFullName() + "\n";
            msg += tr("with DNI : ") + userId() + tr(" in the database");
            showMessageBox(title, msg, alt_msg);
            return;
        }

        showFeedBack(tr("User entered correctly"));
        expectNext();

        if (hasEnroller()) { // disable the users widget, and enable
            enablePage(m_fingerprints_page, true);
            enablePage(m_details_page, false);
            showPage(m_fingerprints_page);
            enroller()->setId(userId());
            state = State::Enrollment;
        } else
            clear(); // stays at the same phase
    } break;

    case State::Enrollment: // enrollment actions when the ok_button is pressed:
        enablePage(m_details_page, true);
        enablePage(m_fingerprints_page, false);
        showPage(m_details_page);
        clear();
        break;

    case State::AccesControl: // acl actions when the ok button is pressed
        break;
    default:
        break;
    }
}

void UserCreationWidget::cancel()
{
    clear();
    clearOutputLabel();
    enablePage(m_details_page, true);
    showPage(m_details_page);
    enablePage(m_fingerprints_page, false);
}

void UserCreationWidget::fingerprintAcquireSlot()
{

    // acquire fingerprint... the slot method of the user Module will sen us a signal to show the reults
    // of the acquire action.
    if (!canBiometricAuth())
        return;

    if (userId().isEmpty() || userFistName().isEmpty() || userLastName().isEmpty()) {
        QString text = tr("To be able to insert a user's fingerprint, you first have to\nintroduce at least the three essential fields:\nname, surname and employee id\n");
        QMessageBox::warning(this, tr("Failed to insert user"), text, QMessageBox::Ok);
        return;
    }

    auto msg = tr("An error occurred while trying to insert the fingerprint of user \n %1 with DNI %2 \nAn internal wsrpcd server error may have occurred or the communication with the server has been broken");
    auto caption = tr("failed to insert user fingerprint");

    if (!setReturnApplication()) {
        QString text = msg.arg(userFullName()).arg(userId());
        QMessageBox::warning(this, caption, text, QMessageBox::Ok);
        return;
    }

    setExpectingEnrollMent(true);
    QTimer::singleShot(ENROLMENT_TIMEOUT, this, &UserCreationWidget::flushExpectingData);
    if (!enroll()) {
        QString text = msg.arg(userFullName()).arg(userId());
        QMessageBox::warning(this, caption, text, QMessageBox::Ok);
        return;
    }
}

void UserCreationWidget::flushExpectingData()
{
    setExpectingEnrollMent(false);
}

void UserCreationWidget::fingerprintReadSlot(const QString& xml_str)
{
    if (!isExpectingEnrollMent())
        return;

    XmlConfig xml;
    xml.readXmlFromString(xml_str);
    auto id = xml.readString("identity");
    float quality = xml.readString("quality").toFloat();
    auto fingerprint = xml.readString("finger");
    auto fingerprint_path = xml.readString("fingerprint_path");
    // TODO : show the fingerprint and all the other things
}

void UserCreationWidget::doInitialize()
{
    clear();
}

void UserCreationWidget::doActualize()
{
    enablePage(m_fingerprints_page, false);
}

bool UserCreationWidget::setReturnApplication()
{
    // give the application name and also the interface to communicate with the results of the fingerprint
    // acquire process
    //     QByteArray data;
    //     QDataStream stream (&data, QIODevice::ReadWrite);
    //     stream << QString(kapp->name());
    //     stream << model()->getInterfaceName();
    //     stream << model()->getEnrollMethodName();
    //     DCOPClient *client = kapp->dcopClient();
    //     return client->send("dcopfx2000","FX2000Enroll","setReturnApplication(QString, QString,QString)", data);
    return false;
}

bool UserCreationWidget::enroll()
{
    //    QByteArray data;
    //    QDataStream stream(&data, QIODevice::ReadWrite);
    //    stream << name_edit->text();
    //    stream << QString::number(0); //it should be take from the fingerprint
    //    DCOPClient *client = kapp->dcopClient();
    //    return client->send("dcopfx2000", "FX2000Enroll", "asyncEnroll(QString,QString)", data);
    return false;
}
