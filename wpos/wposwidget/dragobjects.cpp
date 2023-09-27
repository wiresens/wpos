/***************************************************************************
                          bslgenericscreenwidget.h  -  description
                             -------------------
    begin                : Tue Sep 9 2003
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
#include "dragobjects.h"
#include <QEvent>
#include <QDragEnterEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QDrag>
#include <QMimeData>
#include <iostream>

using namespace std;

/***********************************************************************
 * ListBox part, it only defines the acceptbox and insert ittems...
 * it should also insert at one list
 ***********************************************************************/
// ListBox -- low level drag and drop

BslDDListBox::BslDDListBox(QWidget* parent, Qt::WindowFlags f) :
    QListView(parent){
    setAcceptDrops(true);
    dragging = false;
    setViewMode(QListView::IconMode);
    setWindowFlags(f);
}

void BslDDListBox::dragEnterEvent(QDragEnterEvent* evt){
    if (evt->mimeData()->hasText() || evt->mimeData()->hasImage()) evt->accept();
}

void BslDDListBox::dropEvent(QDropEvent* evt){
    if( evt->mimeData()->hasText() ) emit(textEnter( evt->mimeData()->text()));
}

void BslDDListBox::mousePressEvent( QMouseEvent* evt ){
    QListView::mousePressEvent(evt);
    dragging = true;
}

void BslDDListBox::mouseMoveEvent( QMouseEvent* evt){
    if ( dragging ) {
        QDrag *dg = new QDrag(  this );
        QMimeData *mimeData = new QMimeData;
//        mimeData->setText(currentText());
        dg->setMimeData(mimeData);
        dragging = false;
        evt->accept();
    }
}

/***************************************************************
 * this widget is the main iconview and methods to control the 
 * drag'n drog events.
 ***************************************************************/
// IconView -- high level drag and drop

QDrag *BslDDIconView::dragObject(){
    QDrag* dg { new QDrag( this )};
    return dg;
}

void BslDDIconView::slotNewItem( QDropEvent *evt ){

//    if ( evt->mimeData()->hasText()) {
//        BslDDIconViewItem *item = new BslDDIconViewItem( this, evt->mimeData()->text() );
////        item->setRenameEnabled(true);
//        item->isSelected();
//    }
}

/***************************************************************
 * this widget is the main iconview and methods to control the 
 * drag'n drog events.
 ***************************************************************/
// IconViewIcon -- high level drag a
// nd drop
bool BslDDIconViewItem::acceptDrop( const QMimeData *mime ) const{
    return mime->hasFormat("text/plain") ;
}

void BslDDIconViewItem::dropped( QDropEvent* evt){

    if ( evt->mimeData()->hasText() )
    setText( evt->mimeData()->text() );
}

BslDDTable::BslDDTable(QWidget* parent, const QString& name) :
    QTableWidget( parent )
{
    setObjectName(name);
    setAcceptDrops(true);
    setDragEnabled(true);   
    dragging = false;
}

void BslDDTable::dragEnterEvent( QDragEnterEvent* evt ){
//    if ( QDrag::canDecode(evt) ) evt->accept();
      evt->accept();
}

void BslDDTable::dropEvent( QDropEvent* evt ){
    if ( evt->mimeData()->hasText()){
            emit(textEnter(evt->mimeData()->text()));

        QPoint p = evt->pos();
        emit(textEnter(p.x(), p.y(), evt->mimeData()->text()));
	 	emit(textEnter(p.x(), p.y()));
	}
}

void BslDDTable::mousePressEvent( QMouseEvent* evt ){
    dragging = true;
    QTableWidget::mousePressEvent( evt );
}

void BslDDTable::mouseMoveEvent( QMouseEvent* evt){
    cout << "Creation of Dragobject" << endl;
    if ( dragging ) {
        QDrag *dg = new QDrag(  this );
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(currentItem()->text());
        dg->setMimeData(mimeData);
        dragging = false;
        evt->accept();
    }
}

void BslDDTable::swapRows(int row1, int row2){

}
