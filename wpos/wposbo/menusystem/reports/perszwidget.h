/***************************************************************************
                          perszwidget.h  -  description
                             -------------------
    begin                : mar may 11 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef PERSZWIDGET_H
#define PERSZWIDGET_H

#include "ui_perszwidgetbase.h"

#include "database/historicdb.h"

#include <wposcore/config.h>

#include <QWidget>

class HistoricDB;
class QTreeWidgetItem;
class Browser;
class XmlConfig;
class QTimer;
class QWebEngineView;

class PersZWidget :
        public QWidget,
        private Ui::PersZWidgetBase
{
    Q_OBJECT
public: 
    static const uint DEFAULT_WIDTH;
    static const uint DEFAULT_HEIGHT;

    explicit PersZWidget(QWidget *parent=0, const QString& name=QString());
    ~PersZWidget() = default;

public slots:
    void clear();

    void upTicketSlot();
    void downTicketSlot();
    void firstTicketSlot();
    void lastTicketSlot();

    void downBrowserSlot();
    void upBrowserSlot();

    void tableClickedSlot(const QDateTime& start);

    void zSelectedSlot();

    void acceptSlot();

    void printSlot();
    void exitSlot();
    void updateProgressBar();

protected slots:
    void setEndDateRange(const QDateTime& start);

protected:

    enum Section{
        Icon = 0,
        StartDate = 1,
        EndDate = 2,
        Amount = 3,
        EventCode = 4
    };

    enum SectionSize{
        IconSize = 60,
        DateSize = 200,
        AmountSize = 140,
        EventCodeSize = IconSize
    };

    virtual void showEvent(QShowEvent *e) override;
    virtual void hideEvent(QHideEvent *e) override;
    virtual bool isPrintServerAvailable() const;

    void getZ(const QDateTime& start, const QDateTime& end);
    bool checkAllValues();
    void getCurrentZ(XmlConfig& xml);

    void startGetingZ();
    void stopGettingZ();

    void printZ(const QString& xml_str);
    void printTicketTotal(const QString& xml_str);

    QString xmlToHtml(XmlConfig *xml);

    QString mainSection(XmlConfig *xml);
    QString productSection(XmlConfig *xml);
    QString waitersSection(XmlConfig *xml);
    QString offerSection(XmlConfig *xml);
    QString optionSection(XmlConfig *xml);

    bool createMainGraph(XmlConfig *xml,int width=DEFAULT_WIDTH,int height= DEFAULT_WIDTH + 30);
    bool createWaitersGraph(XmlConfig *xml, int width=DEFAULT_WIDTH,int height=DEFAULT_HEIGHT);
    bool createOffersGraph(XmlConfig *xml, int width=DEFAULT_WIDTH,int height=DEFAULT_HEIGHT);
    bool createOptionsGraph(XmlConfig *xml, int width=DEFAULT_WIDTH,int height=DEFAULT_HEIGHT);

    void setupButtons();
    void setupSections();
    void connectSignals();
    bool initBrowser();

private:
    void callPrintDeamon(const QString& xml_str);
    void sendToPrintDeamon(const QString& xml_str);
    void showHtml(const QString& html_str);

private:
    HistoricDB database{"getZConnection", Files::configFilePath("database")};
    QWebEngineView *browser{};
    QTimer *timer{};
    QVBoxLayout *b_layout{};

    QStringList headerLabels{"Icon", "Start Date", "End Date", "Price", "Code"};
    QString main_graph;
    QString waiters_graph;
    QString offers_graph;
    QString options_graph;
    int progress_step{};
};

#endif
