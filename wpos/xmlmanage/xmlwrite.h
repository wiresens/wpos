/***************************************************************************
                          xmlwrite.h  -  description
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

#ifndef XMLWRITE_H
#define XMLWRITE_H


/**
  *@author Carlos Manzanedo
  */

#include <QStringList>
class XmlConfig;

class XmlWrite {
public: 
	XmlWrite();
	~XmlWrite();

     void write(QStringList *list);
     void writeAttribute(QStringList *list);

private:
     void writeNodes(QString program_name,QStringList *list);
     void getXml(QString file_name);
     void errorExit(QString output="");

     XmlConfig *xml;

};

#endif
