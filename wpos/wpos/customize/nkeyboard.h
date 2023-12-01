/***************************************************************************
                          nkeyboard.h  -  description
                             -------------------
    begin                : lun jun 7 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef NKEYBOARD_H
#define NKEYBOARD_H

#include "ui_nkeyboardwidget.h"
#include <wposcore/hlist.h>

#include <QMenu>

class ProductExtraInfo;
class XmlConfig;

class QSignalMapper;
class QButtonGroup;
class QRegExp;

struct LoungeData {

    LoungeData() = default;

    ~LoungeData(){
        table_buttons->clear();

        if (option_nodes){
            option_nodes->clear();
            delete option_nodes;
        }

        delete table_buttons;
        delete button;

        button_group = 0;
    }

    QPushButton *button{};
    HList<QPushButton> *table_buttons{ new HList<QPushButton> };
    HList<ProductExtraInfo> *option_nodes{};
    QButtonGroup *button_group{};

    QRegExp regex;
    QString name;
    uint stack_index{};
    uint tables{};
};

class NKeyboard :
    public QMenu,
    private Ui::NKeyboardWidget
{
    Q_OBJECT

protected:
    explicit NKeyboard(
        const QString& text,
        QWidget *parent=nullptr,
        const QString& name = QString()
    );

public:
    explicit NKeyboard(
        QWidget *parent=nullptr,
        const QString& name = QString()
    );

    bool isAtTable();
    QString getLounge();
    int getTable();
    HList<ProductExtraInfo>* getOptionsFromLounge(const QString& lounge);

public slots:
    void setSendEvents(bool on);
    void setExitWithEnter(bool on);
    void clearSlot();
    void showLounge();
    void kbButtonClicked();
    void setText(const QString& text);

    void clearAllStacks(bool clear_enabled = false);
    void clearLoungeStack(const QString &name, bool clear_enabled=false);

    void setUsedTables(QStringList list);
    void setTableButtonUsed(const QString& name);
    void setTableButtonSelected(const QString& name);

protected slots:
    void keyClicked(int num);
    void exitSlot();

    void loungeButtonClicked(bool checked);
    void tableButtonClicked(QAbstractButton* button);
    void switchLounge(QAbstractButton* button);

signals:
    void exitClicked();
    void textChanged(const QString& text);
    void enterPressed(const QString& text);
    void loungeSignal(bool on);

protected:
    void loadLounges(XmlConfig* xml);

private:
    void setMapping(QSignalMapper *signalMapper);

private:
    HList<QPushButton> keyboard_buttons{};
    HList<LoungeData> lounges{};

    QSignalMapper* signalMapper{};
    QButtonGroup* lounge_buttongroup{};
    QObject* o_parent{};

    const QString state;
    bool has_send_events{false};
    bool caps_lock{false};
    bool exit_with_enter{false};
};

#endif
