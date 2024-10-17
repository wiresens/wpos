/***************************************************************************
                          bslsubmenuinfo.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef BSLSUBMENUINFO_H
#define BSLSUBMENUINFO_H

#include <QFrame>

class QVBoxLayout;
class Browser;
class QShowEvent;
class QWebEngineView;

class SubMenuInfoFrame : public QFrame{
    Q_OBJECT
public:
    explicit SubMenuInfoFrame(
        QWidget* parent=0,
        const QString &name = QString{}
    );

    ~SubMenuInfoFrame() = default;

    void load(const QString &infoFile);
    void showEvent(QShowEvent *event) override;

private:
    QVBoxLayout    *layout{};
    QWebEngineView *browser{};
};

#endif
