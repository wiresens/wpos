/***************************************************************************
                          bslmenubutton.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QCheckBox>
#include <QPushButton>
#include <wposcore/hlist.h>

class QMenu;
class QFrame;
class QBoxLayout;
class QGridLayout;
class GenericMenuButton;

class MenuButton : public QPushButton{
    Q_OBJECT

public:
    MenuButton(
        Qt::Orientation orientation,
        QWidget* parent,
        const QString& name);

    ~MenuButton();

    void setOrientation(Qt::Orientation orientation);
    Qt::Orientation orientation();

    bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    void openPopUp();
    void closePopUp();
    void toggleMenuButtonColor(QAbstractButton* button);

signals:
    void genericSignal(const QString& signal_name);
    void closePopUpSignal();

protected:
    void alignInHGrid();
    void addMenuButton(QAbstractButton* button, QGridLayout *layout );
    void childEvent(QChildEvent *event) override;
    void setEnabled(bool enabled);

    QFrame *frame{};
    QBoxLayout *frame_layout{};
    QGridLayout *grid_layout{};
    QButtonGroup *button_group{};
    HList<QAbstractButton> buttons;
    Qt::Orientation popup_orientation{};
};

#endif
