/***************************************************************************
                          readconfigwidget.h  -  description
                             -------------------
    begin                : may 12 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef READ_CONFIG_SCREEN_H
#define READ_CONFIG_SCREEN_H

#include "ui_readconfigwidget.h"
#include <QtWidgets/QWidget>

class XmlConfig;
class MenuPage;

namespace Ui{
class ReadConfigWidget;
}
class ReadConfigScreen :
    public QWidget
{
    Q_OBJECT

public:
    explicit ReadConfigScreen(
        MenuPage *parent,
        const QString& name = QString{});
    ~ReadConfigScreen(){ delete ui;}

public slots:
    void genericDataSignalSlot(
        const QString& signal_name,
        XmlConfig* xml);

protected:
    void showEvent(QShowEvent *e);

private:
    Ui::ReadConfigWidget *ui;
    uint m_pos{0};
};

#endif
