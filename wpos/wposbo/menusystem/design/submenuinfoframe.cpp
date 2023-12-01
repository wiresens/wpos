/***************************************************************************
                          bslsubmenuinfo.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "submenuinfoframe.h"
#include <QtWebEngineWidgets/QWebEngineView>

#include <QLayout>
#include <QEvent>

SubMenuInfoFrame::SubMenuInfoFrame(
    QWidget* parent,
    const QString& name):
    QFrame(parent)
{
    setObjectName(name);

    layout  = new QVBoxLayout(this);
    browser = new QWebEngineView(this);

//@benes    setSizePolicy(parentWidget()->sizePolicy());
    setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    layout->addWidget(browser);
}

void SubMenuInfoFrame::load(const QString &infoFile){
    browser->setHtml(infoFile, QUrl("file://"));
}

void SubMenuInfoFrame::showEvent(QShowEvent *event){
    browser->resize(width(), height());
    QWidget::showEvent(event);
}
