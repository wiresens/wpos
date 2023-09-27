/***************************************************************************
                          fileutils.h  -  description
                             -------------------
    begin                : Thu May 22 2003
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

#ifndef FILEUTILS_H
#define FILEUTILS_H


/**
  *@author Carlos Manzanedo
  */

class FileUtils {
public: 
	FileUtils();
	~FileUtils();

     static bool exists(const char *file_name);
     static bool isRegular(const char *file_name);
     static bool checkReadPerm(const char *file_name);
     static bool checkWritePerm(const char *file_name);
};

#endif
