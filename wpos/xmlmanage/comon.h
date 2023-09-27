/***************************************************************************
                          comon.h  -  description
                             -------------------
    begin                : Tue May 13 2003
    copyright            : (C) 2003 by BlueSock Linux
    email                : support@bluesock.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef COMON_H
#define COMON_H

#include <QString>

#include <iostream>

using namespace std;

#define VER 0
#define REALESE 4

#define EXIT_OK 0
#define EXIT_HELP 1
#define EXIT_ERROR 2
#define EXIT_BAD_DOMAIN 3
#define EXIT_CANT_WRITE 4
#define EXIT_CANT_READ 5
#define EXIT_NOT_XML 6

enum{
     Read=1,
     Write=2,
     List=3,
     Help=4,
     Delete=5,
     ListNodes=6,
     ListAttr=7,
     WriteAttr=8,
     ReadAttr=9,
     DeleteAttr=10,
     Version=11,
     Unmatched=255
};

static const QString& PROGRAM_NAME = "xmlmanage";
static const QString& PROGRAM_READ = "xmlread";
static const QString& PROGRAM_LIST = "xmlshow";
static const QString& PROGRAM_WRITE = "xmlwrite";

#endif
