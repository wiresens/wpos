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

#include <wposcore/hlist.h>

#include <QMenu>
#include <QRegExp>
#include <QPushButton>

class ProductExtraInfo;
class Lounge;
class XmlConfig;

class QSignalMapper;
class QButtonGroup;

namespace Ui {
class NKeyboardWidget;
}

class NKeyboard :
    public QMenu
{
    Q_OBJECT

public:
    explicit NKeyboard(
        QWidget *parent={}
    );

    ~NKeyboard();

    bool isAtTable();
    int getTable();
    QString getLounge();

    HList<ProductExtraInfo>*
    getOptionsFromLounge(const QString& lounge_name);

signals:
    void exitClicked();
    void textChanged(const QString& text);
    void loungeRequested(bool on);
    void enterPressed(const QString& text);

public slots:
    void setText(const QString& text);
    void showLounge();
    void showKeyBoard();
    void setUsedTables(const QStringList &tbl_codes);

    void setHasSendEvents(bool on);
    void setExitWithEnter(bool on);
    void clearSlot();
    void clearAllStacks(bool clear_enabled = false);
    void clearLoungeStack(const QString &lounge_name, bool clear_enabled=false);
    void setTableButtonUsed(const QString& name);
    void setTableButtonSelected(const QString& name);

protected slots:
    void keyClicked(int key);
    void exitSlot();

    void loungeButtonClicked(bool checked);
    void tableButtonClicked(QAbstractButton* button);
    void showTables(QAbstractButton* button);

private:
    void buildLounges(XmlConfig& xml);
    void buildKeyboard();
    void write(const QString& text);
    QString read() const;

private:
    Ui::NKeyboardWidget *ui;
    QObject             *m_obj_parent{};
    QButtonGroup        *m_lounge_selector_btn_group{};
    HList<Lounge>       m_lounges{};

    bool  m_caps_lock{false};
    bool  m_has_send_events{false};
    bool  m_exit_with_enter{false};
};

#endif
