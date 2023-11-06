/***************************************************************************
                          nprofitreportwidget.cpp  -  description
                             -------------------
    begin                : lun abr 19 2004
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

All the viewer repor has been taken from KGhostview so it's under GPL2 license and
the code hasn't been modified.

%LICENCIA%
 ***************************************************************************/

#include "nprofitreportwidget.h"
#include "nprofitreportwidget_adaptor.h"

#include <wposwidget/pdfviewer/pdfviewer.h>
#include <libbslxml/xmlconfig.h>
#include "usersmodule/usermodule.h"

#include <QPushButton>
#include <QFrame>
#include <QLayout>
#include <QGroupBox>
#include <QListView>
#include <QPixmap>
#include <QStackedWidget>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>
#include <QHeaderView>
#include <QButtonGroup>
#include <QString>

#include <QStringList>
#include <qdatetime.h>
#include <QDataStream>
#include <QMessageBox>
#include <QApplication>
#include <QStatusBar>
#include <QMenuBar>
#include <QTabBar>
#include <QLocale>

#define TIMER_SLOT 500

#define BUTTON_OFF QColor(238, 238, 230)
#define BUTTON_ON QColor(107, 181, 251)

#define USER_PIXMAP "controls32:personal.png"

#define USERS_LIST_DTD  "/etc/ntpv_backoffice/dtds/users_userslist.dtd"
#define USERS_DTD  "/etc/ntpv_backoffice/dtds/users_user.dtd"

#define INTERFACE_NAME "ProfitReport"

#include <iostream>
namespace std{}
using namespace std;

//const QString NProfitReportWidget::DBusObjectPath  = QString{"/wpos/wposbo/ProfitReport"};

NProfitReportWidget::NProfitReportWidget(
    UserModule *user_module,
    QWidget *parent, const QString& name):
    QWidget(parent),
    mod{user_module}
{
    setupUi(this);
    setObjectName(name);

    new ProfitReportAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject(NProfitReportWidget::DBusObjectPath, this);

    init();
}

NProfitReportWidget::~NProfitReportWidget(){
    delete timer;
}

void NProfitReportWidget::init(){

    //unique
    interanual_button->setIcon(QPixmap("controls32:package_network.png"));
    anual_button->setIcon(QPixmap("controls32:galeon.png"));
    mensual_button->setIcon(QPixmap("controls32:today.png"));
    semanal_button->setIcon(QPixmap("controls32:date.png"));
    diario_button->setIcon(QPixmap("controls32:clock.png"));
    cierres_button->setIcon(QPixmap("controls32:xcalc.png"));

    ok_button->setIcon(QPixmap("controls48:button_ok_48.png"));
    up_employee_button->setIcon(QPixmap("controls32:up.png"));
    down_employee_button->setIcon(QPixmap("controls32:down.png"));
    advanced_button->setIcon(QPixmap("controls32:run.png"));

    save_report_button->setIcon(QPixmap("controls48:stick_48x48.png"));
    exit_button->setIcon(QPixmap("controls48:button_cancel.png"));

    employee_listview->sortByColumn(2);
    employee_listview->header()->hide();
    employee_listview->setSelectionMode(QListView::SingleSelection);
    employee_listview->setAllColumnsShowFocus(true);
    //    employee_listview->setColumnAlignment(1,Qt::AlignCenter); //0benes
    employee_listview->setColumnWidth(0,50);
    employee_listview->setColumnWidth(1,300);

    QVBoxLayout *vlayout = 0;
    if (!(vlayout = (QVBoxLayout *) kghostview_frame->layout()))
        vlayout = new QVBoxLayout(kghostview_frame);

    viewer = new PdfViewer(kghostview_frame); //@benes
    viewer->setWindowFlags(Qt::Widget);
    viewer->statusBar()->hide();
    viewer->menuBar()->hide();
    vlayout->addWidget(viewer);

    timer = new QTimer();

     //unique
    button_group = new QButtonGroup(report_buttongroup);
    button_group->addButton(interanual_button);
    button_group->addButton(anual_button);
    button_group->addButton(mensual_button);
    button_group->addButton(semanal_button);
    button_group->addButton(diario_button);
    button_group->addButton(cierres_button);
    button_group->setExclusive(true);

    connect(button_group, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked),
            this, &NProfitReportWidget::toggleStates);

    connect(advanced_button,SIGNAL(toggled(bool)),this,SLOT(advancedButtonToggleSlot(bool)));
    connect(up_employee_button,SIGNAL(clicked()),this,SLOT(upUsersSlot()));
    connect(down_employee_button,SIGNAL(clicked()),this,SLOT(downUsersSlot()));
    connect(employee_groupbox,SIGNAL(toggled(bool)),this,SLOT(usersBoxCheckedSlot(bool)));
    connect(employee_listview,SIGNAL(itemSelectionChanged()),this,SLOT(selectedUserSlot()));

    connect(ok_button,SIGNAL(clicked()), this, SLOT(acceptSlot()));
    connect(exit_button,SIGNAL(clicked()),this,SLOT(exitFromViewSlot()));
    connect(save_report_button,SIGNAL(clicked()), this, SLOT(saveReportSlot()));

    //    connectDCOPSignal(0,0,"reportCreated(QString)","reportSlot(QString)", false); //@benes
    connect( this, &NProfitReportWidget::reportCreated, &NProfitReportWidget::reportSlot); //@benes
}

void NProfitReportWidget::showEvent(QShowEvent *e){
    if ( ! advanced_button->isDown()) advanced_button->toggle();
    init_date_groupbox->hide();
    end_date_groupbox->hide();
    startShowing();
    QWidget::showEvent(e);
}

void NProfitReportWidget::hideEvent(QHideEvent *e){
    //    if (stack->visibleWidget()==view_page) part->closeURL(); //@benes
    QWidget::hideEvent(e);
}

void NProfitReportWidget::startShowing(){
    clear();
    if ( !checkReportModule() ) stack->setCurrentWidget(error_page);
    else stack->setCurrentWidget(main_page);
    getUsers();
}

void NProfitReportWidget::clear(){
    employee_listview->clear();
    employee_groupbox->setChecked(false);
    for ( auto button : button_group->buttons()){
        if ( !button->isDown() ) button->toggle();
        button->setPalette(QPalette(QColor(BUTTON_OFF)));
    }
    ok_button->setEnabled(checkAllValues());
}

void NProfitReportWidget::getUsers(){

    employee_listview->clear();
    QString xml_string = mod->usersList(true);
    XmlConfig xml;
    xml.readXmlFromString(xml_string);
    if (!xml.isValid() || !xml.validateXmlWithDTD(USERS_LIST_DTD, true)){
         cerr << "XML parse error or XML does not validate against dtd" << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        return;
    }

    xml.delDomain();
    xml.setDomain("users");
    for ( auto i=0; i < xml.howManyTags("user"); i++){
        auto employee_id = xml.readString("user["+QString::number(i)+"].employee_id");
        auto name = xml.readString("user["+QString::number(i)+"].name");
        auto last_name = xml.readString("user["+QString::number(i)+"].last_name");

        if (employee_id=="1") continue; //do not show the system user.

        auto item = new QTreeWidgetItem(employee_listview);
        item->setIcon(0,QPixmap(USER_PIXMAP));
        item->setText(1,name+" "+last_name);
        item->setText(2,employee_id);
    }
    employee_listview->clearSelection();
    ok_button->setEnabled(checkAllValues());
}

void NProfitReportWidget::toggleStates(QAbstractButton* button){
    button->setBackgroundRole(QPalette::Button);
    for( auto others : button_group->buttons())
        if(others != button) others->setPalette(button->palette());
    button->setPalette(QPalette(QColor(BUTTON_ON)));

    if ( doIsClosing()){
        employee_groupbox->setChecked(false);
        employee_groupbox->setEnabled(false);
    }
    else
        employee_groupbox->setEnabled(true);
}

QString NProfitReportWidget::getActualReport(){
    auto button  = button_group->checkedButton();
    if (button)  return button->objectName();
    return QString();
}

bool NProfitReportWidget::doIsClosing(){
    return getActualReport() == "cierres_button";
}

void NProfitReportWidget::advancedButtonToggleSlot(bool on){
    if ( !advanced_button->isDown()){
        init_date_groupbox->show();
        end_date_groupbox->show();
    }
    else{
        init_date_groupbox->hide();
        end_date_groupbox->hide();
    }
    ok_button->setEnabled(checkAllValues());
}

void NProfitReportWidget::usersBoxCheckedSlot(bool on){
    //        employee_listview->clear();
    //        if (employee_groupbox->isChecked()){
    //                getUsers();
    ok_button->setEnabled(checkAllValues());
}

bool NProfitReportWidget::checkAllValues(){

    return  !getActualReport().isEmpty()
           && employee_groupbox->isChecked()
           && !employee_listview->selectedItems().isEmpty();
}

void NProfitReportWidget::upUsersSlot(){
    //    employee_listview->scroll(0,-70);
}

void NProfitReportWidget::downUsersSlot(){
    //    employee_listview->scroll(0,70);
}

bool NProfitReportWidget::checkReportModule(){
    //    DCOPClient *client = kapp->dcopClient();
    //    if ( !client->isAttached() ) client->attach();

    //    //if the dbusreceipt is not running  exit without saving
    //    return client->isApplicationRegistered("dcopreports");
    return true;
}

bool NProfitReportWidget::checkStickModule(){
    //    DCOPClient *client = kapp->dcopClient();
    //    if (!client->isAttached()) client->attach();

    //    //if the dbusreceipt is not running  exit without saving
    //    return client->isApplicationRegistered("dcopstick");
    return false;
}

void NProfitReportWidget::selectedUserSlot(){
    ok_button->setEnabled(checkAllValues());
}

void NProfitReportWidget::acceptSlot(){
    if (!checkReportModule()){
        stack->setCurrentWidget(error_page);
        return;
    }

    QString informe = getActualReport();
    informe.replace("_button","");

    QString employee_id;
    if (employee_groupbox->isChecked()){
        if (employee_listview->selectedItems().isEmpty())
            return;
        auto item = employee_listview->selectedItems().first();
        employee_id = item->text(2);
    }

    QString start_date, end_date;
    if (!advanced_button->isDown()){
        if (init_datepicker->date() > end_datepicker->date()){
            QString text = tr("The dates entered are not correct.\n\The start date is before the end date\n");
            QMessageBox::warning(this,tr("Can't get report"), text);
            return;
        }
        start_date = init_datepicker->date().toString("dd/MM/yyyy");
        end_date = end_datepicker->date().toString("dd/MM/yyyy");
    }

    else
        doSelectReport(informe, start_date, end_date);

    QByteArray data, reply_data;
    QDataStream out_stream(&data, QIODevice::ReadWrite);
    QDataStream reply(&reply_data, QIODevice::ReadWrite);

    doSetOutputStream(out_stream, informe, start_date, end_date, employee_id);

    QString reply_type;
    //idem
    //    if (!client->call("dcopreports","DCOPReportReportman","createReport(QString,QString,QString,QString)",data, reply_type,reply_data))
    //    {
    //        cerr << "problems while trying to call dcopreports" << endl;
    //        QString text = tr("Ha fallado la llamada al modulo de informes.\n");
    //        QMessageBox::warning(this,tr("ha fallado la generacion de informes"),text);
    //    }


    bool ret = false;
    if (reply_type == "bool") reply >> ret;

//    if (!ret){
//        QString text = tr("The call to the report module has failed.\n");
//        QMessageBox::warning(this,tr("Report Generation Failiure"),text);
//        startShowing();
//        return;
//    }

    last_report_name = informe ;

    if (!start_date.isEmpty())  last_report_name += "_" + start_date;
    if (!end_date.isEmpty())    last_report_name += "_" + end_date;
    if (!employee_id.isEmpty()) last_report_name += "_" + employee_id;

    last_report_name += ".pdf";
    last_report_name.replace('/',"-");

    stack->setCurrentWidget(progress_page);
    progress_bar->setValue(0);
    timer->start(5);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    pos = 0;
    emit reportCreated(last_report_name);  //@benes
}

void NProfitReportWidget::doSelectReport(
    QString& start_date, QString& end_date,
    const QString& informe)
{
    QDate aux_date, date;

    if ( informe == tr("interanual" )){
        date.setDate( 1970, 1, 1);
        start_date = date.toString("dd/MM/yyyy");
        date = QDate::currentDate();
        end_date = date.toString("dd/MM/yyyy");
    }
    else if ( informe == tr("anual" )){
        date = QDate::currentDate();
        aux_date.setDate(date.year(), 1, 1);
        start_date = aux_date.toString("dd/MM/yyyy");
        aux_date.setDate(date.year(), 12, 31);
        end_date = aux_date.toString("dd/MM/yyyy");
    }
    else if ( informe == tr("mensual" )){
        date = QDate::currentDate();
        aux_date.setDate(date.year(), date.month(), 1);
        start_date = aux_date.toString("dd/MM/yyyy");
        aux_date.setDate(date.year(), date.month(), date.daysInMonth());
        end_date = aux_date.toString("dd/MM/yyyy");
    }
    else if ( informe == tr("semanal" )){
        int first_day , last_day;
        date = QDate::currentDate();
        first_day = date.day() - date.dayOfWeek()+ 1;
        last_day = date.day() + ( 7 - date.dayOfWeek()) + 1;

        aux_date.setDate( date.year(), date.month(), first_day);
        start_date = aux_date.toString("dd/MM/yyyy");
        aux_date.setDate(date.year(), date.month(), last_day);
        end_date = aux_date.toString("dd/MM/yyyy");
    }
    else if ( informe == tr("diario") ){
        date = QDate::currentDate();
        aux_date.setDate(date.year(), date.month(), date.day());
        start_date = aux_date.toString("dd/MM/yyyy");
        aux_date.setDate(date.year(), date.month(), date.day());
        end_date = aux_date.toString("dd/MM/yyyy");
    }
    else if ( informe == tr("cierres") ){
        date = QDate::currentDate();
        aux_date.setDate(date.year(), 1, 1);
        start_date = aux_date.toString("dd/MM/yyyy");
        aux_date.setDate(date.year(), 12, 31);
        end_date = aux_date.toString("dd/MM/yyyy");
    }
}

void NProfitReportWidget::doSetOutputStream(QDataStream& out_stream, const QString& informe, const QString& start_date, const QString& end_date, const QString& employee_id){
    out_stream << informe;
    out_stream << QString(start_date);
    out_stream << QString(end_date);
    out_stream << employee_id;
}

void NProfitReportWidget::timerSlot(){
    pos++;
    progress_bar->setValue(pos);
    qApp->processEvents();
}

void NProfitReportWidget::reportSlot(QString file){
    file = "/home/benes/Workspace/study/wpos/gitops.pdf";
    QObject::disconnect(timer,SIGNAL(timeout()),this,SLOT(timerSlot()));
    progress_bar->setValue(0);
    timer->stop();

    if (file.isEmpty()){
        QString text = tr("The call to the report module has failed.\n");
        QMessageBox::warning(this, tr("Report Generation Failiure"), text);
        startShowing();
        return;
    }
    last_report_file = file;

    if (checkStickModule()) save_report_button->show();
    else save_report_button->hide();

    stack->setCurrentWidget(view_page);
    viewer->open(QUrl::fromLocalFile(file));
}

void NProfitReportWidget::exitFromViewSlot(){
    viewer->close();
    startShowing();
}

void NProfitReportWidget::saveReportSlot(){
    //will be implemented by the pdfviewer
    //    bool ret = false;
    //    DCOPClient *client= 0;
    //    QCString reply_type;
    //    QByteArray data, reply_data;
    //    QDataStream out_stream(data,IO_ReadWrite);
    //    QDataStream reply(reply_data, IO_ReadWrite);

    //    client = kapp->dcopClient();
    //    out_stream << last_report_file;
    //    out_stream << last_report_name;

    //    if (!client->call("dcopstick", "dcopstick","saveReportToStick(QString, QString)",
    //                      data,reply_type,reply_data)){
    //        cerr << "problems while trying to call dcopstick module" << endl;
    //        QString text = tr("Ha fallado la llamada al modulo de guardar a memory stick .\n");
    //        QMessageBox::warning(this,tr("ha fallado la operacion de guardar en el stick"),text,0);
    //    }
    //    if (reply_type == "bool")
    //        reply >> ret;

    //    if (!ret){
    //        QString text = tr("ha fallado la operacion de guardar en el stick.\n\
    //                          puede que este no este pinchado\n en una ranura USB o este ya montado");
    //                          QMessageBox::warning(this,tr("ha fallado la operacion de guardar en el stick"),text,0);
    //                       return;
    //    }
}
