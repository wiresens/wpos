/***************************************************************************
                          bslxzwidget.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef XZWIDGET_H
#define XZWIDGET_H

#include "ui_xzwidget.h"
#include <QWidget>

class XmlConfig;
class QString;
class PrinterManager;
class QVBoxLayout;
class QWebEngineView;

class XZWidget :
        public QWidget,
        private Ui::XZWidget
{
    Q_OBJECT

public:
    enum OperationMode {
        XOperation = 0,
        ZOperation = 1
    };

    explicit XZWidget(QWidget *parent=0, const QString& name = QString());
    ~XZWidget();
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

public slots:
    void xSlot(XmlConfig *xml);
    void zSlot(XmlConfig *xml);
    void scrollDownSlot();
    void scrollUpSlot();
    void genericDataSignalSlot(const QString& signal_name, XmlConfig *xml);
    void printSlot();
    void returnToADMmenuSlot();

signals:
    void genericDataSignal(const QString& signal_name, XmlConfig *xml);
    
protected:
    QString mainSection(XmlConfig *xml);
    QString productSection(XmlConfig *xml);
    QString waitersSection(XmlConfig *xml);
    QString offerSection(XmlConfig *xml);
    QString optionSection(XmlConfig *xml);

    bool createMainGraph(XmlConfig *xml, QString file_path, int width=200,int height=230);
    bool createWaitersGraph(XmlConfig *xml ,QString file_path, int width=200,int height=250);
    bool createOffersGraph(XmlConfig *xml, QString file_path, int width=200,int height=250);
    bool createOptionsGraph(XmlConfig *xml, QString file_path, int width=200,int height=250);

    bool initBrowser();
        
    QWebEngineView *browser{};
    QVBoxLayout *b_layout{};
    QString html_page;
    OperationMode operation{OperationMode::XOperation}; // X is selected as standart operation mode
};

#endif
