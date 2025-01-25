/***************************************************************************
                          nupdateoptionwidget.h  -  description
                             -------------------
    begin                : Mon Apr 28 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef OPTION_UPDATE_WIDGET_H
#define OPTION_UPDATE_WIDGET_H

#include "optioneditionwidget.h"
#include <QObject>

class OptionUpdateWidget : public OptionEditionWidget {

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.wiresens.wpos.wposbo.OptionUpdateModel")

public:
    static const QString DBusObjectPath;

    explicit OptionUpdateWidget(QWidget* parent = 0, const QString& name = QString {});
    ~OptionUpdateWidget() = default;

signals:
    Q_SCRIPTABLE void optionChanged(); // dbus_signals

protected slots:
    virtual void acceptSlot() override;
    virtual void notify() override;

    void optionClickedSlot(); // processOption()
    void applyToAllProductsSlot(); // processType()

protected:
    virtual void showEvent(QShowEvent* e) override;
    virtual void getTypes() override;
    virtual void getOptionTypes(const QString& type) override;
};

#endif // OPTION_UPDATE_WIDGET_H
