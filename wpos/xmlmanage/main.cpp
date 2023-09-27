/***************************************************************************
*                          main.cpp  -  description
*                             -------------------
*    begin                : Tue May 13 14:47:35 CEST 2003
    copyright            : (C) 2003 by Napsis S.L.
    email                : carlos@napsis.com
****************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "comon.h"
#include "helpclass.h"
#include "xmlread.h"
#include "xmllist.h"
#include "xmlwrite.h"
#include "xmldelete.h"


#include <QCoreApplication>
#include <QString>
#include <QStringList>

int main(int argc, char *argv[]){

    //the main will only make the parse of the strings...
    //class declaration

    HelpClass help;

    XmlRead*    read = nullptr;
    XmlList*    list = nullptr;
    XmlWrite*  write = nullptr;
    XmlDelete*   del = nullptr;

    QString cmdline_arg{argv[0]};
    QString prog_name{ cmdline_arg.mid( cmdline_arg.lastIndexOf('/') + 1) };

    if (argc < 2 ){
        help.showMainHelp(prog_name);
        exit(EXIT_HELP);
    }

    //get out of the string the first arg
    QStringList arguments;
    cmdline_arg.clear();
    for(int i = 1;  i < argc; i++ ){
        cmdline_arg.append(argv[i]);
        arguments.append(argv[i]);
        if ( i != argc - 1 ) cmdline_arg.append(" ");
    }

    //strip the previous part before the order...
    if ( prog_name == PROGRAM_READ ){
        read = new XmlRead();
        read->read(&arguments);
        delete read;
        exit(EXIT_OK);
    }
    else if ( prog_name == PROGRAM_WRITE ){
        write = new XmlWrite();
        write->write(&arguments);
        delete write;
        exit(EXIT_OK);
    }
    else if ( prog_name == PROGRAM_LIST ){
        list = new XmlList();
        list->list(&arguments);
        delete list;
        exit(EXIT_OK);
    }
    //     else if (QString::compare(name,PROGRAM_NAME)==0){
    //          help.unmatchedName();
    //          exit(EXIT_ERROR);
    //     }

    //get which option has been selected
    const int pos = help.getMatch(&arguments);

    //remove the first argument
    arguments.removeFirst();
    switch ( pos ){
    case Help:
        help.help(&arguments);
        exit(EXIT_OK);
        break;
    case List:
        list = new XmlList();
        list->list(&arguments);
        delete list;
        exit(EXIT_OK);
        break;
    case ListAttr:
        list = new XmlList();
        list->listAttributes(&arguments);
        delete list;
        exit(EXIT_OK);
        break;
    case ListNodes:
        list = new XmlList();
        list->listNodes(&arguments);
        delete list;
        exit(EXIT_OK);
        break;
    case Write:
        write = new XmlWrite();
        write->write(&arguments);
        delete write;
        exit(EXIT_OK);
        break;
    case WriteAttr:
        write = new XmlWrite();
        write->writeAttribute(&arguments);
        delete write;
        exit(EXIT_OK);
        break;
    case Read:
        read = new XmlRead();
        read->read(&arguments);
        delete read;
        exit(EXIT_OK);
        break;
    case ReadAttr:
        read = new XmlRead();
        read->readAttribute(&arguments);
        delete read;
        exit(EXIT_OK);
        break;
    case Delete:
        del = new XmlDelete();
        del->del(&arguments);
        delete del;
        exit(EXIT_OK);
        break;
    case DeleteAttr:
        del = new XmlDelete();
        del->delAttribute(&arguments);
        delete del;
        exit(EXIT_OK);
        break;
    case Version:
        help.version();
        exit(EXIT_OK);
        break;
    case Unmatched:
        help.showMainHelp(prog_name);
        exit(EXIT_HELP);
        break;
    default:
        help.showMainHelp(prog_name);
        exit(EXIT_ERROR);
        break;
    }

    exit(EXIT_ERROR);
}
