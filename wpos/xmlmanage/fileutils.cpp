/***************************************************************************
                          fileutils.cpp  -  description
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

#include "fileutils.h"

extern "C"{
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
}

FileUtils::FileUtils(){
}
FileUtils::~FileUtils(){
}

bool FileUtils::exists(const char *file_name){
    struct stat *f_stat=0;

    f_stat = new struct stat;
    if (stat(file_name,f_stat)!=0){
        delete f_stat;
        return false;
    }

    delete f_stat;
    return true;
}

bool FileUtils::isRegular(const char *file_name){
    struct stat *f_stat=0;

     f_stat = new struct stat;
     if (stat(file_name,f_stat)!=0){
          delete f_stat;
          return false;
     }

     //check that it can be readed and it's a regular file
     if (!((f_stat->st_mode&S_IFREG)==S_IFREG)){
          delete f_stat;
          return false;
     }

     delete f_stat;
     return true;
}

bool FileUtils::checkReadPerm(const char *file_name){
     if (access(file_name, R_OK))
          return false;

return true;
}

bool FileUtils::checkWritePerm(const char *file_name){
     if (access(file_name,R_OK|W_OK))
          return false;

return true;
}
