/***************************************************************************
                          bslgenericscreenwidget.cpp  -  description
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

#include "treeview.h"
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QFont>

/************************************************************************************************************
 *
 *
 ************************************************************************************************************/
TreeView::TreeView(QWidget* parent) : QTreeWidget(parent){

    // Initialization and defalut values
//@benes    setFixedWidth(230);

    /*@benes
    addColumn("");
    setTreeStepSize(40);
    header()->adjustHeaderSize();
    header()->setStretchEnabled(true);
    //@benes*/

    header()->hide();
    setFont(QFont("SansSeriff", 9, QFont::Normal, true));
    setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    // Make some connections
    //  connect(this,SIGNAL(clicked(QTreeWidgetItem *)),this,SLOT(nodeClicked(QTreeWidgetItem *)));
}

TreeView::~TreeView(){}

void TreeView::nodeClicked(QTreeWidgetItem* item){
    //  cerr << "@ node clicked in BsListView class" << endl;

    //  if(item == 0)
    //    return;
    //
    //  if(item->childCount() == 0){
    ////    QString aux_string = "";
    ////
    ////    if((aux_string = menu_item_relantionships->find(item)) != 0){
    ////      setMenuSignal(aux_string);
    //  } else {
    //    // Do nothing
    //    ;
    //  } // end_if
}
