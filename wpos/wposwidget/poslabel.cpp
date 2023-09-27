/***************************************************************************
                          qposlabel.cpp  -  description
                             -------------------
    begin                :  Oct 25 2003
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

#include "poslabel.h"

#include <QPixmap>
#include <QMouseEvent>

struct MPoint{
    QString name;
    int x;
    int y;
    int width;
    int height;
};

PosLabel::PosLabel(QString path, QWidget *parent,
                   const QString& name ) :
    QLabel(name, parent),
    point_list(0)
{
    init();
    setPixmap(QPixmap(path));
}

PosLabel::PosLabel(const QPixmap& pixmap,
                   QWidget *parent,
                   const QString& name):
    QLabel(name,parent),
    point_list(0)
{
    init();
    setPixmap(pixmap);
}

PosLabel::~PosLabel(){
    point_list->clear();
    delete point_list;
}

void PosLabel::init(){
    point_list = new HList<MPoint>;
    point_list->setAutoDelete(true);
}

bool PosLabel::insertMPoint(const MPoint *point){
    MPoint *p = 0;
    p = new MPoint;
    p->name = point->name;
    p->x = point->x;
    p->width = point->width;
    p->y = point->y;
    p->height = point->height;
    point_list->append(p,p->name);
    return true;
}

bool PosLabel::insert(QString name,int x,int y,int width,int height){
    MPoint p;
    p.name = name;
    p.x = x;
    p.y = y;
    p.width = width;
    p.height = height;
    return insertMPoint(&p);
}

bool PosLabel::deleteMPoint(const MPoint *point){
    if (!point)
        return false;

    return deleteMPoint(point->name);
}

bool PosLabel::deleteMPoint(QString name){
    MPoint *p = 0;
    p = point_list->find(name);
    if (!p)
        return false;
    point_list->remove(name);
    return true;
}

void PosLabel::mouseDoubleClickEvent(QMouseEvent *event){
    int count,i;
    MPoint *p;

    //        cout << "X : "<< e->x() << "  Y : " << e->y() << endl;

    count = point_list->count();
    for(i=0;i<count;i++){
        p = point_list->at(i);
        if (((event->x()) >= (p->x))&&
                ((event->x()) <= (p->x + p->width))&&
                ((event->y()) >= (p->y))&&
                ((event->y()) <= (p->y + p->height))){
            //the point was the elected point
            emit doubleClickAt(p->name);
        }
    }
    //        QLabel::mouseDoubleClickEvent(e);
}

void PosLabel::mousePressEvent(QMouseEvent *event){

    MPoint *p;
    for( auto i=0; i < point_list->count(); i++){
        p = point_list->at(i);
        if ( event->x() >= p->x  &&  event->x() <= p->x + p->width && event->y() >= p->y &&  event->y() <= p->y + p->height )
            emit clickAt(p->name);

    }
    //        QLabel::mouseDoubleClickEvent(e);
}

