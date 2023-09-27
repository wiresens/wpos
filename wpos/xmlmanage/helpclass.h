/***************************************************************************
                          helpclass.h  -  description
                             -------------------
    begin                : mar may 13 2003
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

#ifndef HELPCLASS_H
#define HELPCLASS_H


/**
  *@author Carlos Manzanedo
  */

#include <QStringList>

class QString;


class HelpClass {
public: 
     HelpClass();
     ~HelpClass();

     /**
     *    Method to show the help of the command at the main.
     */
     void showMainHelp(QString program_name);
     void unmatchedName();

     /**
     * this class is used to clasify the words that have been used at
     * the string, it will return an integer declared at the comon.h enum
     * that can be Read, Write, Help or List by now
     */
     int getMatch(const QStringList *list);

     void help(QStringList *list);

     void listHelp();
     void readHelp();
     void writeHelp();
     void deleteHelp();

     void version();

private:
     void listExamples(QString program_name);
     void readExamples(QString program_name);
     void writeExamples(QString program_name);
     void deleteExamples(QString program_name);
     void domainExplanation();

};

#endif
