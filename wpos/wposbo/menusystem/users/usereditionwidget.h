#ifndef USER_EDITION_WIDGET_H
#define USER_EDITION_WIDGET_H

#include "ui_usercreatewidgetbase.h"
#include "usersmodule/usermodule.h"

template<typename Key, typename Value>
class QMap;

class QString;
class FX2000EnrollWidget;
class XmlConfig;

class UserEditionWidget :
        public QWidget,
        private Ui::UserCreateWidgetBase {
    Q_OBJECT

public:

    static const QString& COMPANY_NAME;
    static const unsigned int ENROLMENT_TIMEOUT;
    static const unsigned int FEEDBACK_TIMEOUT;

    explicit UserEditionWidget( UserModule *_user_mod, QWidget *parent=0, const QString& name = QString());

    QString userId() const;

protected slots:
    virtual void apply(){}
    virtual void cancel(){}

    void setUserPicture();
    void clear();
    void clearOutputLabel();

protected:
    virtual void showEvent(QShowEvent *e) override final;
    virtual void doInitialize(){}
    virtual void doEnroll(){}
    virtual void doActualize() = 0;
    virtual void doFinalize(){}
    virtual bool canBiometricAuth() const;

    UserModule* model() const{ return user_mod; }
    FX2000EnrollWidget *enroller() const{ return enroll_widget; }
    bool hasEnroller() const{ return fx2000_available; }

    QString userFullName() const{ return userFistName() + " " + userLastName(); }

    QString userFistName() const;
    QString userLastName() const;    

    bool isValidEmail() const;
    void retrieveUserInfo(XmlConfig &xml);
    void showUserInfo(XmlConfig &xml);
    void showMessageBox(const QString& title, const QString& msg, const QString& feedBack = QString());
    void showFeedBack(const QString& feedBack);

    void expectNext(){ employee_id->setFocus(); }
    void setCancelButtonVisible( bool visible){ cancel_button->setVisible(visible); }
    void setOkButtonVisible( bool visible){ ok_button->setVisible(visible); }
    void setUserIdReadOnly( bool read_only){employee_id->setReadOnly(read_only);}
    void setCaption(const QString& caption){ title->setText(caption);}

    void setExpectingEnrollMent( bool expecting){ expecting_enrollment = expecting; }
    bool isExpectingEnrollMent() const { return expecting_enrollment ; }

    void showPage(const QString& name);
    void addPage(QWidget* page, const QString& name);
    void removePage(const QString& name);
    void enablePage(const QString& name, bool enabled);

private:
    void initializePages();

protected:
    //pages names
    const QString m_fingerprints_page{tr("&Fingerprints")};
    const QString m_details_page{tr("&Details")};
    const QString m_acls_page{tr("&ACL's")};

private:
    FX2000EnrollWidget *enroll_widget{};
    UserModule *user_mod{};
    QMap<QString, QWidget*> pages;
    QMap<QWidget*, QString> page_names;
    QString picture_path;

    bool expecting_enrollment{false};
    bool fx2000_available{false};
};

#endif // USER_EDITION_WIDGET_H
