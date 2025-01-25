/***************************************************************************
                          nprodandofferreportwidget.cpp  -  description
                             -------------------
    begin                : jue may 6 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "nprodandofferreportwidget.h"
#include "nprodandofferreportwidget_adaptor.h"

#include "usersmodule/usermodule.h"
#include <libbslxml/xmlconfig.h>

//@benes
// #include "viewer/kgv_view.h"
// #include "viewer/kgv_miniwidget.h"
// #include "viewer/kgvdocument.h"

#include <QButtonGroup>
#include <QFrame>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QListView>
#include <QPixmap>
#include <QProgressBar>
#include <QPushButton>
#include <QStackedWidget>
#include <QString>
#include <QTimer>

#include <QApplication>
#include <QDataStream>
#include <QDateTime>
#include <QLocale>
#include <QMessageBox>
#include <QStringList>

#define TIMER_SLOT 500

#define BUTTON_OFF QColor(238, 238, 230)
#define BUTTON_ON QColor(107, 181, 251)

#define USER_PIXMAP "/usr/share/ntpv_backoffice/apps/32x32/personal.png"

#define USERS_LIST_DTD "/etc/ntpv_backoffice/dtds/users_userslist.dtd"
#define USERS_DTD "/etc/ntpv_backoffice/dtds/users_user.dtd"

#define INTERFACE_NAME "ProductAndOffersReport"

#include <iostream>
namespace std {
}
using namespace std;

// const QString NProdAndOfferReportWidget::DBusObjectPath  = QString{"/wpos/wposbo/ProductAndOfferReport"};

NProdAndOfferReportWidget::NProdAndOfferReportWidget(
    UserModule* user_module,
    QWidget* parent, const QString& name)
    : QWidget(parent)
    , mod { user_module }
{
    setupUi(this);
    setObjectName(name);

    new ProductAndOfferReportAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(NProdAndOfferReportWidget::DBusObjectPath, this);

    init();
}

NProdAndOfferReportWidget::~NProdAndOfferReportWidget()
{
    //   delete part; //@benes
    delete timer;
}

void NProdAndOfferReportWidget::init()
{

    // unique
    productos_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/productos.png"));
    ofertas_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/ksplash.png"));
    opciones_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/ksplash.png"));
    prod_unitarios_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/producto.png"));
    ofertas_prod_unitarios_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/ksplash.png"));
    opciones_prod_unitarios_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/ksplash.png"));
    precios_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/modificar_productos.png"));

    ok_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/button_ok_48.png"));
    up_employee_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/up.png"));
    down_employee_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/down.png"));
    advanced_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/run.png"));
    start_gv_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/start.png"));
    prev_gv_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/2leftarrow.png"));
    down_gv_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/down.png"));
    zoomin_gv_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/viewmag+.png"));
    zoomout_gv_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/viewmag-.png"));
    up_gv_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/up.png"));
    next_gv_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/2rightarrow.png"));
    last_gv_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/32x32/finish.png"));
    save_report_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/stick_48x48.png"));
    exit_button->setIcon(QPixmap("/usr/share/ntpv_backoffice/apps/48x48/button_cancel.png"));

    employee_listview->sortByColumn(2);
    employee_listview->header()->hide();
    employee_listview->setSelectionMode(QListView::SingleSelection);
    employee_listview->setAllColumnsShowFocus(true);
    //    employee_listview->setColumnAlignment(1,Qt::AlignCenter); //@benes
    employee_listview->setColumnWidth(0, 50);
    employee_listview->setColumnWidth(1, 300);

    QVBoxLayout* lv = 0;
    QStringList list;
    if (!(lv = (QVBoxLayout*)kghostview_frame->layout()))
        lv = new QVBoxLayout(kghostview_frame);
    //  part = new KGVPart(kghostview_frame,"part",kghostview_frame,"part",list); //@benes
    //  lv->addWidget(part->widget());

    timer = new QTimer();

    //@benes
    //    connect(start_gv_button,SIGNAL(clicked()), part, SLOT(slotGotoStart()));
    //    connect(prev_gv_button,SIGNAL(clicked()), part, SLOT(slotPrevPage()));
    //    connect(up_gv_button,SIGNAL(clicked()), part, SLOT(slotScrollUp()));
    //    connect(zoomin_gv_button,SIGNAL(clicked()), part,SLOT(slotZoomIn()));
    //    connect(zoomout_gv_button,SIGNAL(clicked()), part,SLOT(slotZoomOut()));
    //    connect(down_gv_button,SIGNAL(clicked()), part,SLOT(slotScrollDown()));
    //    connect(next_gv_button,SIGNAL(clicked()), part,SLOT(slotNextPage()));
    //    connect(last_gv_button,SIGNAL(clicked()), part,SLOT(slotGotoEnd()));

    // unique
    button_group = new QButtonGroup(report_buttongroup);
    button_group->addButton(productos_button);
    button_group->addButton(ofertas_button);
    button_group->addButton(opciones_button);
    button_group->addButton(prod_unitarios_button);
    button_group->addButton(ofertas_prod_unitarios_button);
    button_group->addButton(opciones_prod_unitarios_button);
    button_group->addButton(precios_button);
    button_group->setExclusive(true);

    connect(button_group, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
        this, &NProdAndOfferReportWidget::toggleStates);

    connect(advanced_button, SIGNAL(toggled(bool)), this, SLOT(advancedButtonToggleSlot(bool)));
    connect(up_employee_button, SIGNAL(clicked()), this, SLOT(upUsersSlot()));
    connect(down_employee_button, SIGNAL(clicked()), this, SLOT(downUsersSlot()));
    connect(employee_groupbox, SIGNAL(toggled(bool)), this, SLOT(usersBoxCheckedSlot(bool)));
    connect(employee_listview, SIGNAL(itemSelectionChanged()), this, SLOT(selectedUserSlot()));

    connect(ok_button, SIGNAL(clicked()), this, SLOT(acceptSlot()));
    connect(exit_button, SIGNAL(clicked()), this, SLOT(exitFromViewSlot()));
    connect(save_report_button, SIGNAL(clicked()), this, SLOT(saveReportSlot()));

    //        connectDCOPSignal(0,0,"reportCreated(QString)","reportSlot(QString)",false);   //@benes
}

void NProdAndOfferReportWidget::showEvent(QShowEvent* e)
{
    if (!advanced_button->isDown())
        advanced_button->toggle();
    init_date_groupbox->hide();
    end_date_groupbox->hide();
    startShowing();
    QWidget::showEvent(e);
}

void NProdAndOfferReportWidget::hideEvent(QHideEvent* e)
{
    //    if (stack->visibleWidget()==view_page) part->closeURL(); //@benes
    QWidget::hideEvent(e);
}

void NProdAndOfferReportWidget::startShowing()
{
    clear();
    if (!checkReportModule())
        stack->setCurrentWidget(error_page);
    else
        stack->setCurrentWidget(main_page);
    getUsers();
}

void NProdAndOfferReportWidget::clear()
{
    employee_listview->clear();
    employee_groupbox->setChecked(false);
    //        if (!advanced_button->isDown())
    //                advanced_button->toggle();
    //        init_date_groupbox->hide();
    //        end_date_groupbox->hide();
    for (auto button : button_group->buttons()) {
        if (!button->isDown())
            button->toggle();
        button->setPalette(QPalette(QColor(BUTTON_OFF)));
    }
    //        init_datepicker->setDate(QDate::currentDate());
    //        end_datepicker->setDate(QDate::currentDate());
    ok_button->setEnabled(checkAllValues());
}

void NProdAndOfferReportWidget::getUsers()
{
    int count, i;
    QString employee_id, name, last_name;
    QString xml_string;
    XmlConfig* xml = 0;
    QListWidgetItem* item = 0;

    employee_listview->clear();
    xml_string = mod->usersList(true);
    xml = new XmlConfig();
    xml->readXmlFromString(xml_string);
    if (!xml->isValid()) {
        delete xml;
        return;
    }
    if (!xml->validateXmlWithDTD(USERS_LIST_DTD, true)) {
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        delete xml;
        return;
    }

    xml->delDomain();
    xml->setDomain("users");
    count = xml->howManyTags("user");
    for (i = 0; i < count; i++) {
        employee_id = xml->readString("user[" + QString::number(i) + "].employee_id");
        name = xml->readString("user[" + QString::number(i) + "].name");
        last_name = xml->readString("user[" + QString::number(i) + "].last_name");

        if (employee_id == "1") // do not show the system user.
            continue;

        item = new QListWidgetItem(employee_listview);
        item->setPixmap(0, QPixmap(USER_PIXMAP));
        item->setText(1, name + " " + last_name);
        item->setText(2, employee_id);
        employee_listview->insertItem(item);
    }
    employee_listview->clearSelection();
    delete xml;
    ok_button->setEnabled(checkAllValues());
}

void NProdAndOfferReportWidget::reportButtonClickedSlot()
{
    int i, count;
    QButton* button = 0;
    count = report_buttongroup->count();
    for (i = 0; i < count; i++) {
        button = report_buttongroup->find(i);
        if (button->isOn())
            button->setPaletteBackgroundColor(BUTTON_ON);
        else
            button->setPaletteBackgroundColor(BUTTON_OFF);

        if (doIsClosing()) {
            employee_groupbox->setChecked(false);
            employee_groupbox->setEnabled(false);
        } else
            employee_groupbox->setEnabled(true);
    }
    ok_button->setEnabled(checkAllValues());
}

bool NProdAndOfferReportWidget::doIsClosing() const
{
    return getActualReport() == "precios_button";
}

void NProdAndOfferReportWidget::toggleStates(QAbstractButton* button)
{
    button->setBackgroundRole(QPalette::Button);
    for (auto others : button_group->buttons())
        if (others != button)
            others->setPalette(button->palette());
    button->setPalette(QPalette(QColor(BUTTON_ON)));

    if (button->objectName() == "precios_button") {
        employee_groupbox->setChecked(false);
        employee_groupbox->setEnabled(false);
    } else
        employee_groupbox->setEnabled(true);
}

QString NProdAndOfferReportWidget::getActualReport()
{
    QString ret = "";
    QButton* button = 0;
    button = report_buttongroup->selected();
    if (button)
        ret = button->name();
    return ret;
}

void NProdAndOfferReportWidget::advancedButtonToggleSlot(bool on)
{
    if (advanced_button->isOn()) {
        init_date_groupbox->show();
        end_date_groupbox->show();
    } else {
        init_date_groupbox->hide();
        end_date_groupbox->hide();
    }
    ok_button->setEnabled(checkAllValues());
}

void NProdAndOfferReportWidget::usersBoxCheckedSlot(bool on)
{
    //        employee_listview->clear();
    //        if (employee_groupbox->isChecked()){
    //                getUsers();
    ok_button->setEnabled(checkAllValues());
    //        }
}

bool NProdAndOfferReportWidget::checkAllValues()
{
    QListWidgetItem* item = 0;
    if ((getActualReport()).isEmpty())
        return false;

    if (employee_groupbox->isChecked())
        if (!(item = employee_listview->selectedItem()))

            return false;

    return true;
}

void NProdAndOfferReportWidget::upUsersSlot()
{
    employee_listview->scrollBy(0, -70);
}

void NProdAndOfferReportWidget::downUsersSlot()
{
    employee_listview->scrollBy(0, 70);
}

bool NProdAndOfferReportWidget::checkReportModule()
{
    DCOPClient* client = 0;
    client = kapp->dcopClient();
    if (!client->isAttached())
        client->attach();

    // if the dcopreceipt is not running  exit without saving
    return (client->isApplicationRegistered("dcopreports"));
}

bool NProdAndOfferReportWidget::checkStickModule()
{
    DCOPClient* client = 0;
    client = kapp->dcopClient();
    if (!client->isAttached())
        client->attach();

    // if the dcopreceipt is not running  exit without saving
    return (client->isApplicationRegistered("dcopstick"));
}

void NProdAndOfferReportWidget::selectedUserSlot()
{
    ok_button->setEnabled(checkAllValues());
}

void NProdAndOfferReportWidget::acceptSlot()
{
    bool ret = false;
    QString informe = "";
    QString start_date = "";
    QString end_date = "";
    QString employee_id = "";
    QDate aux_date, date;
    QListWidgetItem* item = 0;
    DCOPClient* client = 0;
    QCString reply_type;
    QByteArray data, reply_data;
    QDataStream out_stream(data, IO_ReadWrite);
    QDataStream reply(reply_data, IO_ReadWrite);

    if (!checkReportModule()) {
        stack->raiseWidget(error_page);
        return;
    }

    informe = this->getActualReport();
    informe.replace("_button", "");

    if (employee_groupbox->isChecked()) {
        item = employee_listview->selectedItem();
        if (!item)
            return;
        employee_id = item->text(2);
    }

    if (advanced_button->isOn()) {
        if ((init_datepicker->date()) > (end_datepicker->date())) {
            QString text = tr("Las fechas introducidas no son correctas.\n\La fecha inicial es anterior a la fecha final\n");
            QMessageBox::warning(this, tr("No se puede obtener informe"), text, 0);
            return;
        }
        start_date = (init_datepicker->date()).toString("dd/MM/yyyy");
        end_date = (end_datepicker->date()).toString("dd/MM/yyyy");
    }
    // unique doSelectReport(informe, date, start_date, end_date);
    else {
        // year
        date = QDate::currentDate();
        aux_date.setYMD(date.year(), 1, 1);
        start_date = aux_date.toString("dd/MM/yyyy");
        aux_date.setYMD(date.year(), 12, 31);
        end_date = aux_date.toString("dd/MM/yyyy");

        // month
        //                        date = QDate::currentDate();
        //                        aux_date.setYMD(date.year(),date.month(),1);
        //                        start_date = aux_date.toString("dd/MM/yyyy");
        //                        aux_date.setYMD(date.year(),date.month(),date.daysInMonth());
        //                        end_date = aux_date.toString("dd/MM/yyyy");

        // week
        //                        int first_day,last_day;
        //                        date = QDate::currentDate();
        //                        first_day = date.day() - date.dayOfWeek();
        //                        last_day = date.day() + (7-date.dayOfWeek());
        //                        aux_date.setYMD(date.year(),date.month(),first_day);
        //                        start_date = aux_date.toString("dd/MM/yyyy");
        //                        aux_date.setYMD(date.year(),date.month(),last_day);
        //                        end_date = aux_date.toString("dd/MM/yyyy");

        // day
        //                        date = QDate::currentDate();
        //                        aux_date.setYMD(date.year(),date.month(),date.day());
        //                        start_date = aux_date.toString("dd/MM/yyyy");
        //                        aux_date.setYMD(date.year(),date.month(),date.day());
        //                        end_date = aux_date.toString("dd/MM/yyyy");
    }

    // unique doSetOutputStream(out_stream,informe,start_date,end_date,employee_id)
    client = kapp->dcopClient();
    out_stream << informe;
    if (informe == "precios") {
        out_stream << "";
        out_stream << "";
        out_stream << "";
    } else {
        //                out_stream << QString(start_date+" 00:00:00");
        //                out_stream << QString(end_date+" 23:59:59");
        out_stream << QString(start_date);
        out_stream << QString(end_date);
        out_stream << employee_id;
    }

    // idem
    if (!client->call("dcopreports", "DCOPReportReportman", "createReport(QString,QString,QString,QString)", data, reply_type, reply_data)) {
        cerr << "problems while trying to call dcopreports" << endl;
        QString text = tr("Ha fallado la llamada al modulo de informes.\n");
        QMessageBox::warning(this, tr("ha fallado la generacion de informes"), text, 0);
    }
    if (reply_type == "bool")
        reply >> ret;

    if (!ret) {
        QString text = tr("Ha fallado la llamada al modulo de informes.\n");
        QMessageBox::warning(this, tr("ha fallado la generacion de informes"), text, 0);
        startShowing();
        return;
    }

    last_report_name = informe;
    if (!start_date.isEmpty())
        last_report_name += "_" + start_date;
    if (!end_date.isEmpty())
        last_report_name += "_" + end_date;
    if (!employee_id.isEmpty())
        last_report_name += "_" + employee_id;

    last_report_name += ".pdf";
    last_report_name.replace('/', "-");

    stack->raiseWidget(progress_page);
    progress_bar->setTotalSteps(0);
    timer->start(5);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    pos = 0;
}

void NProdAndOfferReportWidget::doSelectReport(QString& start_date, QString& end_date, const QString& informe)
{
    (void)informe;
    QDate aux_date;
    auto date = QDate::currentDate();
    aux_date.setDate(date.year(), 1, 1);
    start_date = aux_date.toString("dd/MM/yyyy");
    aux_date.setDate(date.year(), 12, 31);
    end_date = aux_date.toString("dd/MM/yyyy");
}

void NProdAndOfferReportWidget::doSetOutputStream(
    QDataStream& out_stream, const QString& informe,
    const QString& start_date, const QString& end_date,
    const QString& employee_id)
{
    client = kapp->dcopClient();
    out_stream << informe;
    if (informe == "precios") {
        out_stream << "";
        out_stream << "";
        out_stream << "";
    } else {
        //   out_stream << QString(start_date+" 00:00:00");
        //   out_stream << QString(end_date+" 23:59:59");
        out_stream << QString(start_date);
        out_stream << QString(end_date);
        out_stream << employee_id;
    }
}

void NProdAndOfferReportWidget::timerSlot()
{
    pos++;
    progress_bar->setProgress(pos);
    qApp->processEvents();
}

void NProdAndOfferReportWidget::reportSlot(QString file)
{
    QObject::disconnect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    progress_bar->setTotalSteps(0);
    timer->stop();
    if (file.isEmpty()) {
        QString text = tr("Ha fallado la llamada al modulo de informes.\n");
        QMessageBox::warning(this, tr("ha fallado la generacion de informes"), text, 0);
        startShowing();
        return;
    }
    last_report_file = file;

    if (checkStickModule())
        save_report_button->show();
    else
        save_report_button->hide();

    stack->raiseWidget(view_page);
    part->openURL(KURL(file));
    part->slotZoomOut();
    part->slotZoomOut();
    part->slotZoomOut();
}

void NProdAndOfferReportWidget::exitFromViewSlot()
{
    part->closeURL();
    startShowing();
}

void NProdAndOfferReportWidget::saveReportSlot()
{
    bool ret = false;
    DCOPClient* client = 0;
    QCString reply_type;
    QByteArray data, reply_data;
    QDataStream out_stream(data, IO_ReadWrite);
    QDataStream reply(reply_data, IO_ReadWrite);

    client = kapp->dcopClient();
    out_stream << last_report_file;
    out_stream << last_report_name;

    if (!client->call("dcopstick", "dcopstick", "saveReportToStick(QString,QString)",
            data, reply_type, reply_data)) {
        cerr << "problems while trying to call dcopstick module" << endl;
        QString text = tr("Ha fallado la llamada al modulo de guardar a memory stick .\n");
        QMessageBox::warning(this, tr("ha fallado la operacion de guardar en el stick"), text, 0);
    }
    if (reply_type == "bool")
        reply >> ret;

    if (!ret) {
        QString text = tr("ha fallado la operacion de guardar en el stick.\n");
        text += tr("puede que este no este pinchado\n en una ranura USB o este ya montado");
        QMessageBox::warning(this, tr("ha fallado la operacion de guardar en el stick"), text, 0);
        return;
    }
}
