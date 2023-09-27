/***************************************************************************
                          xmlread.h  -  description
                             -------------------
    begin                : Tue May 13 2003
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

#ifndef XMLREAD_H
#define XMLREAD_H


/**
  *@author Carlos Manzanedo
  */

#include <QStringList>
class QString;
class XmlConfig;

class XmlRead {
public: 
     XmlRead();
     ~XmlRead();

     /**
     * method that should read the specified domain.
     * and print at the std out the value of that domain
     */
     void read(QStringList *list);
     /**
     * method to read an attribute from a node
     */
     void readAttribute(QStringList *list);

private:
     void readNodes(QString file_name,QStringList *list);
     void getXml(QString file_name);
     void errorExit(QString output="");

     XmlConfig *xml;
};

#endif
