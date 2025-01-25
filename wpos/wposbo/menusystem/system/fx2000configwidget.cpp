/***************************************************************************
                          fx2000configwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "fx2000configwidget.h"
#include "fx2000configwidget_adaptor.h"
#include "fx2000enrollwidget.h"

#include "usersmodule/usermodule.h"

#include <ntpvlibwidget/poslabel.h>

#include <QButtonGroup>
#include <QCheckBox>
#include <QDataStream>
#include <QFile>
#include <QFrame>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QLocale>
#include <QMessageBox>
#include <QPixmap>
#include <QProgressBar>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QStackedWidget>
#include <QString>
#include <QTabWidget>
#include <QTimer>
#include <QtDBus/QDBusConnection>

#include <iostream>
using namespace std;

static const QString& DEVICE_PICTURE { "/usr/share/ntpv_backoffice/apps/fx2000.jpg" };
static const QString& FX2000_INTERFACE_NAME { "Fx2000CommInterface" };
static const QString& FINGERPRINT_DCOP_MODULE { "dcopfx2000" };
static const QString& DCOPSTICK_MODULE { "dcopstick" };
static const int TIMER_SLOT = 5; // in milliseconds

const QString FX2000ConfigWidget::DBusObjectPath = QString { "/wpos/wposbo/FingerPrintReader" };

FX2000ConfigWidget::FX2000ConfigWidget(
    UserModule* model,
    QWidget* parent,
    const QString& name)
    : QWidget(parent)
    , model { model }
{
    setupUi(this);
    setObjectName(name);

    new FingerPrintReaderAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(FX2000ConfigWidget::DBusObjectPath, this);

    timer = new QTimer(this);

    QHBoxLayout* hlayout = new QHBoxLayout(device_frame);
    device_label = new PosLabel(DEVICE_PICTURE, device_frame, "device_photo");
    hlayout->addWidget(device_label);

    init_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/fx2000.jpg"));
    light_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/test.png"));
    test_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/ktip.png"));
    export_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/stick_48x48.png"));
    import_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/stick_48x48.png"));
    accept_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/button_ok_48.png"));

    device_label->insert("INTERNAL", 50, 120, 50, 45);
    device_label->insert("GREEN", 77, 77, 20, 20);
    device_label->insert("RED", 106, 100, 18, 18);
    connect(device_label, SIGNAL(clickAt(QString)), this, SLOT(deviceClickedAt(QString)));

    hlayout = new QHBoxLayout(enroll_frame);
    enroll_widget = new FX2000EnrollWidget(enroll_frame, "enroll_frame");
    hlayout->addWidget(enroll_widget);
    enroll_widget->setReturnApplication(qApp->applicationName(), model->getInterfaceName(), model->getEnrollMethodName());

    connect(id_line_edit, SIGNAL(textChanged(const QString&)), this, SLOT(idChanged(const QString&)));
    connect(model, SIGNAL(enrollDataAvailable(QString)), enroll_widget, SLOT(fingerprintReadSlot(QString)));

    connect(import_button, SIGNAL(clicked()), this, SLOT(importFromStickButton()));
    connect(export_button, SIGNAL(clicked()), this, SLOT(exportToStickButton()));
    connect(accept_button, SIGNAL(clicked()), this, SLOT(okButtonClicked()));

    connectDBusSignal();
}

void FX2000ConfigWidget::idChanged(const QString& id)
{
    enroll_widget->setId(id);
}

void FX2000ConfigWidget::showEvent(QShowEvent* e)
{

    stack->setCurrentWidget(main_page);
    if (!isFingerPrintModuleAvailable()) {
        setEnabled(false);
        return QWidget::showEvent(e);
    }

    if (!isMemstickModuleAvailable())
        stick_groupbox->setEnabled(false);
    else
        stick_groupbox->setEnabled(true);

    setEnabled(true);
    readStatus();
    tab->setCurrentWidget(config);
}

void FX2000ConfigWidget::readStatus()
{
    readGreen();
    readRed();
    readInternal();
    readPresence();
    readQuality();
    readSimilarity();
    readState();
}

void FX2000ConfigWidget::readRed()
{
    checkLeds("getRed()", red_button);
}

void FX2000ConfigWidget::readGreen()
{
    checkLeds("getGreen()", green_button);
}

void FX2000ConfigWidget::readInternal()
{
    checkLeds("getInternal()", green_button);
}

void FX2000ConfigWidget::readQuality()
{
    checkSensibility("getMinQuality()", quality_slider, quality_progressbar);
}

void FX2000ConfigWidget::readSimilarity()
{
    checkSensibility("getMinSimilarity()", similarity_slider, similarity_progressbar);
}

void FX2000ConfigWidget::readPresence()
{
    checkSensibility("getPresenceLimit()", presence_slider, presence_progressbar, 1);
}

void FX2000ConfigWidget::readState()
{
    DCOPClient* client = 0;
    QDataStream* out_stream = 0;
    QDataStream* in_stream = 0;
    QByteArray send;
    QByteArray receive;
    QString aux;
    QCString return_type;
    bool ret = false;

    client = kapp->dcopClient();
    if ((!client->isAttached()) || (!client->isApplicationRegistered(QCString(FINGERPRINT_DCOP_MODULE)))) {
        this->setEnabled(false);
        return;
    }

    out_stream = new QDataStream(send, IO_ReadWrite);
    in_stream = new QDataStream(receive, IO_ReadWrite);

    if (!client->call(QCString(FINGERPRINT_DCOP_MODULE), "FX2000Interface", "status()",
            send, return_type, receive)) {
        QString text = tr("se produjo un fallo al tratar de leer el\n\
                          estado del led rojo\n");
        QMessageBox::warning(this, tr("fallo al leer estado del dispositivo"), text, 0);
        delete in_stream;
        delete out_stream;
        return;
    }

    // check return_type
    *in_stream >> ret;
    disconnectAll();
    if (ret)
        resume_button->setChecked(true);
    else
        standby_button->setChecked(true);

    delete in_stream;
    delete out_stream;
    connectAll();
}

void FX2000ConfigWidget::redToggle(bool on)
{
    checkLeds("setRed(bool)");
    DCOPClient* client = 0;
    QDataStream* out_stream = 0;
    QByteArray send;

    client = kapp->dcopClient();
    if ((!client->isAttached()) || (!client->isApplicationRegistered(QCString(FINGERPRINT_DCOP_MODULE)))) {
        this->setEnabled(false);
        return;
    }

    out_stream = new QDataStream(send, IO_ReadWrite);
    *out_stream << on;

    if (!client->send(QCString(FINGERPRINT_DCOP_MODULE), "FX2000Interface", "setRed(bool)", send)) {
        QString text = tr("se produjo un fallo al tratar de escribir el\n\
                          estado del led rojo\n");
        QMessageBox::warning(this, tr("fallo al escribir estado en el dispositivo"), text, 0);
        delete out_stream;
        return;
    }

    // check return_type
    delete out_stream;
}

void FX2000ConfigWidget::greenToggle(bool on)
{
    DCOPClient* client = 0;
    QDataStream* out_stream = 0;
    QByteArray send;

    client = kapp->dcopClient();
    if ((!client->isAttached()) || (!client->isApplicationRegistered(QCString(FINGERPRINT_DCOP_MODULE)))) {
        this->setEnabled(false);
        return;
    }

    out_stream = new QDataStream(send, IO_ReadWrite);
    *out_stream << on;

    if (!client->send(QCString(FINGERPRINT_DCOP_MODULE), "FX2000Interface", "setGreen(bool)", send)) {
        QString text = tr("se produjo un fallo al tratar de escribir el\n\
                          estado del led verde\n");
        QMessageBox::warning(this, tr("fallo al escribir estado en el dispositivo"), text, 0);
        delete out_stream;
        return;
    }

    // check return_type
    delete out_stream;
}

void FX2000ConfigWidget::internalToggle(bool on)
{
    DCOPClient* client = 0;
    QDataStream* out_stream = 0;
    QByteArray send;

    client = kapp->dcopClient();
    if ((!client->isAttached()) || (!client->isApplicationRegistered(QCString(FINGERPRINT_DCOP_MODULE)))) {
        this->setEnabled(false);
        return;
    }

    out_stream = new QDataStream(send, IO_ReadWrite);
    *out_stream << on;

    if (!client->send(QCString(FINGERPRINT_DCOP_MODULE), "FX2000Interface", "setInternal(bool)", send)) {
        QString text = tr("se produjo un fallo al tratar de escribir el\n\
                          estado del led interno\n");
        QMessageBox::warning(this, tr("fallo al escribir estado en el dispositivo"), text, 0);
        delete out_stream;
        return;
    }

    // check return_type
    delete out_stream;
}

void FX2000ConfigWidget::stateToggle(int id)
{
    DCOPClient* client = 0;
    QDataStream* out_stream = 0;
    QByteArray send;
    QCString aux_method;

    client = kapp->dcopClient();
    if ((!client->isAttached()) || (!client->isApplicationRegistered(QCString(FINGERPRINT_DCOP_MODULE)))) {
        this->setEnabled(false);
        return;
    }

    if (resume_button->isChecked())
        aux_method = "resume()";
    else
        aux_method = "standby()";

    out_stream = new QDataStream(send, IO_ReadWrite);
    *out_stream << true;

    if (!client->send(QCString(FINGERPRINT_DCOP_MODULE), "FX2000Interface", aux_method, send)) {
        QString text = tr("se produjo un fallo al tratar de escribir el\n\
                          estado del led interno\n");
        QMessageBox::warning(this, tr("fallo al escribir estado en el dispositivo"), text, 0);
        delete out_stream;
        return;
    }

    // check return_type
    delete out_stream;
}

void FX2000ConfigWidget::qualityChanged(int quality)
{
    DCOPClient* client = 0;
    QDataStream* out_stream = 0;
    QByteArray send;
    float num;

    client = kapp->dcopClient();
    if ((!client->isAttached()) || (!client->isApplicationRegistered(QCString(FINGERPRINT_DCOP_MODULE)))) {
        this->setEnabled(false);
        return;
    }

    out_stream = new QDataStream(send, IO_ReadWrite);
    num = (float)quality / 100.0;
    *out_stream << num;

    if (!client->send(QCString(FINGERPRINT_DCOP_MODULE), "FX2000Interface", "setMinQuality(float)", send)) {
        QString text = tr("se produjo un fallo al tratar de escribir el\n\
                          estado del led interno\n");
        QMessageBox::warning(this, tr("fallo al escribir estado en el dispositivo"), text, 0);
        delete out_stream;
        return;
    }

    // check return_type
    delete out_stream;
}

void FX2000ConfigWidget::similarityChanged(int similarity)
{
    DCOPClient* client = 0;
    QDataStream* out_stream = 0;
    QByteArray send;
    float num;

    client = kapp->dcopClient();
    if ((!client->isAttached()) || (!client->isApplicationRegistered(QCString(FINGERPRINT_DCOP_MODULE)))) {
        this->setEnabled(false);
        return;
    }

    out_stream = new QDataStream(send, IO_ReadWrite);
    num = (float)similarity / 100.0;
    *out_stream << num;

    if (!client->send(QCString(FINGERPRINT_DCOP_MODULE), "FX2000Interface", "setMinSimilarity(float)", send)) {
        QString text = tr("se produjo un fallo al tratar de escribir el\n\
                          estado del led interno\n");
        QMessageBox::warning(this, tr("fallo al escribir estado en el dispositivo"), text, 0);
        delete out_stream;
        return;
    }

    // check return_type
    delete out_stream;
}

void FX2000ConfigWidget::presenceChanged(int presence)
{
    DCOPClient* client = 0;
    QDataStream* out_stream = 0;
    QByteArray send;

    client = kapp->dcopClient();
    if ((!client->isAttached()) || (!client->isApplicationRegistered(QCString(FINGERPRINT_DCOP_MODULE)))) {
        this->setEnabled(false);
        return;
    }

    out_stream = new QDataStream(send, IO_ReadWrite);
    *out_stream << presence;

    if (!client->send(QCString(FINGERPRINT_DCOP_MODULE), "FX2000Interface", "setMinPresenceLimit(int)", send)) {
        QString text = tr("se produjo un fallo al tratar de escribir el\n\
                          estado del led interno\n");
        QMessageBox::warning(this, tr("fallo al escribir estado en el dispositivo"), text, 0);
        delete out_stream;
        return;
    }

    // check return_type
    delete out_stream;
}

void FX2000ConfigWidget::connectAll()
{
    connect(red_button, SIGNAL(toggled(bool)), this, SLOT(redToggle(bool)));
    connect(green_button, SIGNAL(toggled(bool)), this, SLOT(greenToggle(bool)));
    connect(internal_button, SIGNAL(toggled(bool)), this, SLOT(internalToggle(bool)));
    connect(state_buttongroup, SIGNAL(clicked(int)), this, SLOT(stateToggle(int)));
    connect(quality_slider, SIGNAL(valueChanged(int)), this, SLOT(qualityChanged(int)));
    connect(similarity_slider, SIGNAL(valueChanged(int)), this, SLOT(similarityChanged(int)));
    connect(presence_slider, SIGNAL(valueChanged(int)), this, SLOT(presenceChanged(int)));
    connect(init_button, SIGNAL(released()), this, SLOT(initFX2000Slot()));
    connect(light_button, SIGNAL(released()), this, SLOT(lightAdjustSlot()));
    connect(test_button, SIGNAL(released()), this, SLOT(testSlot()));
}

void FX2000ConfigWidget::disconnectAll()
{
    disconnect(red_button, SIGNAL(toggled(bool)), this, SLOT(redToggle(bool)));
    disconnect(green_button, SIGNAL(toggled(bool)), this, SLOT(greenToggle(bool)));
    disconnect(internal_button, SIGNAL(toggled(bool)), this, SLOT(internalToggle(bool)));
    disconnect(state_buttongroup, SIGNAL(clicked(int)), this, SLOT(stateToggle(int)));
    disconnect(quality_slider, SIGNAL(valueChanged(int)), this, SLOT(qualityChanged(int)));
    disconnect(similarity_slider, SIGNAL(valueChanged(int)), this, SLOT(similarityChanged(int)));
    disconnect(presence_slider, SIGNAL(valueChanged(int)), this, SLOT(presenceChanged(int)));
    disconnect(init_button, SIGNAL(released()), this, SLOT(initFX2000Slot()));
    disconnect(light_button, SIGNAL(released()), this, SLOT(lightAdjustSlot()));
    disconnect(test_button, SIGNAL(released()), this, SLOT(testSlot()));
}

void FX2000ConfigWidget::initFX2000Slot()
{
    checkLeds("initFx2000()");
}

void FX2000ConfigWidget::lightAdjustSlot()
{
    checkLeds("lightAdjust()");
}

void FX2000ConfigWidget::testSlot()
{
    checkLeds("testLeds()");
}

void FX2000ConfigWidget::deviceClickedAt(QString name)
{
    if (name == "INTERNAL") {
        internal_button->setChecked(!internal_button->isChecked());
    } else if (name == "RED") {
        red_button->setChecked(!red_button->isChecked());
    } else if (name == "GREEN") {
        green_button->setChecked(!green_button->isChecked());
    }
}

void FX2000ConfigWidget::importFromStickButton()
{

    if (!checkStickModule())
        return;
    info_label->setText(tr("It is proceeding with the copy of models and fingerprints."));
    startTimer();
    getFingerPrintFromStick();
}

void FX2000ConfigWidget::exportToStickButton()
{

    if (!checkStickModule())
        return;
    info_label->setText(tr("It is proceeding with the copy of models and fingerprints."));
    startTimer();
    saveFingerPrintToStick();
}

void FX2000ConfigWidget::startTimer()
{
    stack->setCurrentWidget(progress_page);
    qApp->processEvents();
    progress_bar->setValue(0);
    timer->start(TIMER_SLOT);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    pos = 0;
}

void FX2000ConfigWidget::timerSlot()
{
    pos++;
    progress_bar->setValue(pos);
    qApp->processEvents();
}

void FX2000ConfigWidget::stopTimer()
{
    QObject::disconnect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    timer->stop();
}

void FX2000ConfigWidget::operationResultSlot(bool status)
{
    if (!isVisible())
        return;

    stopTimer();
    if (status)
        stack->setCurrentWidget(all_ok_page);
    else {
        cerr << "Problems when trying to perform operations with the stick module" << endl;
        auto text = tr("There were problems when trying to perform the operation on the memory stick .\nPerhaps the stick is not connected, is badly connected, or is not formatted\n");
        QMessageBox::information(this, tr("The operation with the stick has failed"), text);
        stack->setCurrentWidget(main_page);
    }
}

bool FX2000ConfigWidget::checkStickModule()
{
    //    DCOPClient *client= 0;
    //    client = kapp->dcopClient();
    //    if (!client->isAttached())
    //        client->attach();

    //    //if the dcopreceipt is not running  exit without saving
    //    return (client->isApplicationRegistered("dcopstick"));
    return false;
}

void FX2000ConfigWidget::okButtonClicked()
{
    stack->setCurrentWidget(main_page);
}

bool FX2000ConfigWidget::isFingerPrintModuleAvailable() const
{
    ////@benes FingerPrint Module not yet available.
    //    DCOPClient *client = kapp->dcopClient();
    //    return client->isAttached() && client->isApplicationRegistered(FINGERPRINT_DCOP_MODULE);
    return false;
}

bool FX2000ConfigWidget::isMemstickModuleAvailable() const
{
    ////@benes Memstick Module not yet available.
    //    DCOPClient *client = kapp->dcopClient();
    //    return client->isAttached() && client->isApplicationRegistered(DCOPSTICK_MODULE);
    return false;
}

void FX2000ConfigWidget::connectDBusSignal() const
{
    ///@benes   connectDCOPSignal(0,0,"stickOperationDone(bool)", "operationResultSlot(bool)",false);
}

bool FX2000ConfigWidget::checkFingerPrint(const QString& cmd)
{
    //    #include <QDBusConnection>
    //    #include <QDBusMessage>
    //    #include <iostream>

    //    auto dbus = QDBusConnection::sessionBus();
    //    if (!dbus.isConnected()){
    //        cerr << "Cannot connect to the D-Bus session bus.\n To start it, run:\n"
    //             << "qdbus com.wiresens.wpos.wposbo /wpos/wposbo/FingerPrintReader";
    //        return false;
    //    }

    //    auto message = QDBusMessage::createMethodCall(
    //         "com.wiresens.wpos.wposbo", "/wpos/wposbo/FingerPrintReader",
    //         "com.wiresens.wpos.wposbo.FingerPrintReader", "receiptChanged" );
    //    QList<QVariant> args;
    //    args.append(employee_id);
    //    args.append(start_time);
    //    message.setArguments(args);
    //    if(!dbus.send(message)) cerr << "Problems while trying to call wpos" << endl;

    //
    if (!isFingerPrintModuleAvailable()) {
        setEnabled(false);
        return false;
    }

    QByteArray send, receive;
    QDataStream ostream(&send, QIODevice::ReadWrite);
    QDataStream istream(&receive, QIODevice::ReadWrite);

    //@benes    client->call(QString(FINGERPRINT_DCOP_MODULE),"FX2000Interface", cmd, send, return_type, receive);
    return false;
}

void FX2000ConfigWidget::checkLeds(const QString& cmd, QCheckBox* button)
{

    if (!isFingerPrintModuleAvailable()) {
        setEnabled(false);
        return;
    }

    QByteArray send, receive;
    QDataStream ostream(&send, QIODevice::ReadWrite);
    QDataStream istream(&receive, QIODevice::ReadWrite);

    QString return_type;

    //@benes     if ( !client->call(QString(FINGERPRINT_DCOP_MODULE), "FX2000Interface", cmd, send, return_type, receive))
    {
        QString text = tr("There was an error trying to read the state of the red led");
        QMessageBox::information(this, tr("Failed to read device status"), text);
        return;
    }

    // check return_type

    if (button) {
        bool result = false;
        istream >> result;
        disconnectAll();
        button->setChecked(result);
        connectAll();
    }
}

void FX2000ConfigWidget::checkSensibility(const QString& cmd, QSlider* sld, QProgressBar* pb, uint coef)
{
    if (!isFingerPrintModuleAvailable()) {
        setEnabled(false);
        return;
    }

    QByteArray send, receive;
    QDataStream ostream(&send, QIODevice::ReadWrite);
    QDataStream istream(&receive, QIODevice::ReadWrite);

    QString return_type;

    //@benes    if ( !client->call(QString(FINGERPRINT_DCOP_MODULE), "FX2000Interface", cmd, send, return_type, receive))
    {
        QString text = tr("There was an error trying to read the state of the red led");
        QMessageBox::information(this, tr("Failed to read device status"), text);
        return;
    }

    // check return_type
    float result = 0.0;
    istream >> result;
    disconnectAll();
    sld->setValue((int)(result * coef));
    pb->setValue((int)(result * coef));
    connectAll();
}

//@benes
bool FX2000ConfigWidget::saveFingerPrintToStick()
{
    //    QByteArray data;
    //    QDataStream ostream(&data, QIODevice::ReadWrite);

    //    DCOPClient *client = kapp->dcopClient();
    //    if(! client->send("dcopstick", "dcopstick", "asyncSaveFingerprintsToStick()", data)){
    //        cerr << "problems while trying to call dcopstick module" << endl;
    //        auto text = tr("The call to the module to save to memory stick has failed .\n");
    //        QMessageBox::information(this,tr("The save operation on the stick has failed"),text);
    //        return false;
    //    }
    return false;
}

bool FX2000ConfigWidget::getFingerPrintFromStick()
{
    //    QByteArray data;
    //    QDataStream ostream(&data, QIODevice::ReadWrite);

    //    DCOPClient *client = kapp->dcopClient();
    //    if(!client->send("dcopstick", "dcopstick", "asyncGetFingerprintsFromStick", data)){
    //        cerr << "problems while trying to call dcopstick module" << endl;
    //        auto text = tr("The call to the module to save to memory stick has failed .\n");
    //        QMessageBox::information(this,tr("The save operation on the stick has failed"),text);
    //        return false;
    //    }
    return false;
}
