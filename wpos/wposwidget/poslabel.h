/***************************************************************************
                          qposlabel.h  -  description
                             -------------------
    begin                : oct 25 2003
    copyright            : (C) 2004 by Napsis S.L.
    email                : carlos@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef POSLABEL_H
#define POSLABEL_H

#include <QLabel>
#include <QString>

#include <wposcore/hlist.h>

class QString;
class QMouseEvent;
class MPoint;

class PosLabel : public QLabel  {
    Q_OBJECT
public:
    explicit  PosLabel(QString path, QWidget *parent = nullptr, const QString& name = QString());
    explicit  PosLabel(const QPixmap& pixmap, QWidget *parent = nullptr, const QString& name = QString());
    ~PosLabel();

    /**
        * insert a mpoint at the hlist. when the doubleclick event happens
        * it cheks if the mouse is pressed at the positions given at the hlist.
        * and emits the proper signals.
        * @param a point to the mpoint.
        * @return true if the point has been added
        */
    bool insertMPoint(const MPoint *point);
    bool insert(QString name,int x,int y,int width,int height);

    /**
        * check if there exist the point at the hlist and delete from it
        * @param point that it's going to be deleted
        * @return true if the point has been deleted
        */
    bool deleteMPoint(const MPoint *point);
    /**
        * the same as above but only the name of the point should be given
        * @param name of the point that it's going to be deleted
        */
    bool deleteMPoint(QString name);

signals:
    void clickAt(QString);
    void doubleClickAt(QString);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void init();

    HList<MPoint> *point_list;
};

#endif
