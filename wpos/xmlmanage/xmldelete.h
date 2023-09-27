/***************************************************************************
                          xmldelete.h  -  description
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

#ifndef XMLDELETE_H
#define XMLDELETE_H


/**
  *@author Carlos MAnzanedo
  */


#include <QStringList>

class QString;
class XmlConfig;

class XmlDelete {
public: 
	XmlDelete();
	~XmlDelete();

     void del(QStringList *list);
     void delAttribute(QStringList *list);

private:

     void delNodes(QString file_name,QStringList *list);
     void getXml(QString file_name);
     void errorExit(QString output="");

     XmlConfig *xml;

};

#endif
