/***************************************************************************
                          xmllist.h  -  description
                             -------------------
    begin                : Wed May 14 2003
    copyright            : (C) 2003 by Napsis S.L.
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

#ifndef XMLLIST_H
#define XMLLIST_H


/**
  *@author Carlos Manzanedo
  */

#include <QStringList>
class XmlConfig;

class XmlList {
public: 
	XmlList();
	~XmlList();

     void list(QStringList *list);
     void listAttributes(QStringList *list);
     void listNodes(QStringList *nodes);
     void partialList(QStringList *list);

private:
     void getXml(QString arg);
     XmlConfig *xml;

};

#endif
