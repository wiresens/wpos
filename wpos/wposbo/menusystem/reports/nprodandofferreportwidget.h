/***************************************************************************
                          nprodandofferreportwidget.h  -  description
                             -------------------
    begin                : jue may 6 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef NPRODANDOFFERREPORTWIDGET_H
#define NPRODANDOFFERREPORTWIDGET_H

#include "ui_prodandofferreportwidgetbase.h"
#include "reportinterface.h"

#include <QWidget>

class PdfViewer;
class UserModule;
class QTimer;

class NProdAndOfferReportWidget :
    public QWidget ,
    virtual public ReportInterface,
    private Ui::ProdAndOfferReportWidgetBase
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.ProductAndOfferReport")

public:

    const QString DBusObjectPath{"/wpos/wposbo/ProductAndOfferReport"};
    NProdAndOfferReportWidget(UserModule *user_module,QWidget *parent=0, const QString& name = QString());
    ~NProdAndOfferReportWidget();

public slots:
    virtual void reportSlot(QString) override; //ASYNC

protected slots:
    void clear();
    void reportButtonClickedSlot();
    virtual void toggleStates(QAbstractButton* button);

    void upUsersSlot();
    void downUsersSlot();

    void advancedButtonToggleSlot(bool on);
    void usersBoxCheckedSlot(bool on);
    void selectedUserSlot();

    void acceptSlot();
    void timerSlot();

    void saveReportSlot();
    void exitFromViewSlot();

protected:
    void init();
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);
    virtual bool doIsClosing() const;
    virtual void doSelectReport(QString& start_date, QString& end_date, const QString& informe = QString());
    virtual void doSetOutputStream(QDataStream& out_stream, const QString& informe, const QString& start_date, const QString& end_date, const QString& employee_id);
    void getUsers();
    void startShowing();

    QString getActualReport();
    bool checkAllValues();
    bool checkReportModule();
    bool checkStickModule();

    UserModule *mod;
    PdfViewer* part;
    QTimer *timer;
    QButtonGroup* button_group{};

    QString last_report_file;
    QString last_report_name;

    int pos;
};

#endif

