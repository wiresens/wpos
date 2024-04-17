/***************************************************************************
                          xmldelete.cpp  -  description
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

#include "xmldelete.h"
#include "helpclass.h"
#include "comon.h"
#include "fileutils.h"

//libbslnet library.
#include <xmlconfig.h>

#include <QString>
#include <QRegExp>

extern "C"{
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
}


XmlDelete::XmlDelete(){
    xml=nullptr;
}

XmlDelete::~XmlDelete(){
    delete xml;
}


void XmlDelete::del(QStringList *list){
    QString arg;

    //check if there are enought parameters.
    if (list->count()<2){
        errorExit();
    }

    //check if the file exists. and create the xml
    arg = list->takeFirst();

    if (
        (arg=="attributes")||
        (arg=="attr")||
        (arg=="att")||
        (arg=="at")||
        (arg=="a")
        ){
        this->delAttribute(list);
    }
    else if(
        (arg=="nodes")||
        (arg=="node")||
        (arg=="no")||
        (arg=="n")
        ){
        arg = list->takeFirst();

    }


    this->delNodes(arg,list);
    exit(EXIT_OK);
}


void XmlDelete::delAttribute(QStringList *list){
    QString arg;
    QString attr;
    QString domain;

    //get the file name
    arg = list->takeFirst();

    getXml(arg);

    if (list->isEmpty()){
        errorExit("No domain has been specified");
    }

    //check if the domain exists.
    domain = list->takeFirst();
    xml->delDomain();

    if (!xml->setDomain(domain)){
        cerr << "the node "<< domain.toStdString() << " does not exists"<< endl;
        delete xml;
        exit(EXIT_BAD_DOMAIN);
    }
    xml->delDomain();

    //if there are no attributes
    if (list->isEmpty()){
        errorExit("there are no attributes to delete");
    }

    while (!list->isEmpty()){
        attr = list->takeFirst();
        xml->deleteAttribute(domain,attr);
    }


    xml->save();
    delete xml;
    xml=0;
    exit(EXIT_OK);
}




void XmlDelete::delNodes(QString file_name,QStringList *list){
    QString arg;
    int err_exit;

    err_exit=EXIT_OK;

    //get the file;
    this->getXml(file_name);

    if (list->isEmpty()){
        this->errorExit("there are no domains to delete");
    }


    while (!list->isEmpty()){
        arg = list->takeFirst();
        xml->delDomain();
        if (!xml->setDomain(arg)){
            cerr << "the node "<< arg.toStdString() << " does not exists"<< endl;
            err_exit = EXIT_BAD_DOMAIN;
            xml->delDomain();
        }
        else{
            xml->delDomain();
            xml->deleteElement(arg);
        }
    }

    xml->delDomain();
    xml->save();

    delete xml;
    xml=0;
    exit(err_exit);
}

void XmlDelete::getXml(QString file_name){
    QString arg;
    arg = file_name;

    //check if the file exists
    if (!FileUtils::exists(arg.toLatin1())){
        cerr << "can't stat the file " << arg.toStdString()<< endl;
        cerr <<"The file does not exists or you've no permissions to reach the file" << endl;
        exit(EXIT_CANT_READ);
    }

    //check that it can be readed and it'sa regular file
    if (!FileUtils::isRegular(arg.toLatin1())){
        cerr << "the file " << arg.toStdString() <<" is not a regular file" << endl;
        exit(EXIT_CANT_READ);
    }

    //are perms to read???
    if (!FileUtils::checkWritePerm(arg.toLatin1())){
        cerr << "the file "<< arg.toStdString() << "has not read permissions" << endl;
        exit(EXIT_CANT_WRITE);
    }


    //try to create the XML
    xml = new XmlConfig(arg);
    if (!xml->wellFormed()){
        cerr << "the file " << arg.toStdString() <<" is not a valid XML file." << endl;
        delete xml;
        xml = 0;
        exit(EXIT_NOT_XML);
    }

}

void XmlDelete::errorExit(QString output){
    if (!output.isEmpty()){
        cerr << output.toStdString() << endl;
    }
    HelpClass help;
    help.deleteHelp();
    exit(EXIT_ERROR);
}


