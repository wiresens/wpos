/***************************************************************************
                          xmlwrite.cpp  -  description
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

#include "xmlwrite.h"
#include "helpclass.h"
#include "comon.h"
#include "fileutils.h"

//libbslnet library.
#include <libbslxml/xmlconfig.h>


#include <QString>
#include <QRegExp>

extern "C"{
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
}



XmlWrite::XmlWrite(){
    xml=0;
}
XmlWrite::~XmlWrite(){
    delete xml;
}

void XmlWrite::write(QStringList *list){
    QString arg;

    //check if there are enought parameters.
    if (list->count()<2){
        this->errorExit();
    }

    //check if the file exists. and create the xml
    arg = list->takeFirst();

    if (
        (arg=="attribute")||
        (arg=="attrib")||
        (arg=="attr")||
        (arg=="att")||
        (arg=="at")||
        (arg=="a")
        ){
        this->writeAttribute(list);
        exit(EXIT_OK);

    }
    if (
        (arg=="node")||
        (arg=="nod")||
        (arg=="no")||
        (arg=="n")
        ){
        arg = list->takeFirst();
    }

    this->writeNodes(arg,list);
    exit(EXIT_OK);
}


void XmlWrite::writeAttribute(QStringList *list){
    QString arg;
    QString domain;
    QString attr;

    //get the file name
    arg = list->takeFirst();

    this->getXml(arg);

    if (list->isEmpty()){
        errorExit("No domain has been specified");
    }


    domain = list->takeFirst();

    //if there are no attributes
    if (list->isEmpty()){
        errorExit("there are no attributes to read");
    }

    xml->delDomain();
    //check if the domain exists.
    if (!xml->setDomain(domain)){
        xml->createElement(domain);
    }

    xml->delDomain();


    while (!list->isEmpty()){
        attr = list->takeFirst();

        //check if the value exists
        if (list->isEmpty()){
            arg="";
        }
        else{
            arg = list->takeFirst();
        }
        //FIXME AT THE LIB
        if(!xml->writeAttribute(domain,attr,arg)){
            xml->createAttribute(domain,attr,arg);
        }
    }

    xml->save();
    delete xml;
    exit(EXIT_OK);
}


void XmlWrite::writeNodes(QString program_name,QStringList *list){
    QString arg;
    QString node;
    QString value;

    this->getXml(program_name);

    //no domain is given
    if (list->isEmpty()){
        this->errorExit();
    }

    node = list->takeFirst();

    if (list->isEmpty()){
        xml->delDomain();
        xml->createElement(node);

    }
    else{
        value = list->takeFirst();
        xml->delDomain();
        //FIXME AT THE LIB
        if (arg.contains(']')){
            xml->doWrite(node,value);
        }
        else{
            xml->createElement(node,value);
        }

        while (!list->isEmpty()){
            node = list->takeFirst();

            if (list->isEmpty()){
                this->errorExit(QString("node "+node+" has no value"));
            }

            value = list->takeFirst();

            xml->delDomain();

            //FIXME AT THE LIB
            if (arg.contains(']')){
                xml->doWrite(node,value);
            }
            else{
                xml->createElement(node,value);
            }
        }
    }
    xml->save();
    delete xml;
    exit(EXIT_OK);
}


void XmlWrite::getXml(QString file_name){
    QString arg;
    arg = file_name;
    if (!FileUtils::exists(arg.toLatin1())){
        //if it does not exists it should be created
        xml = new XmlConfig();
        if (!xml->save(arg)){
            delete xml;
            cerr <<"The file does not exists, and can't be created " << endl;
            exit(EXIT_CANT_WRITE);
        }
        delete xml;
    }

    //check that it can be readed and it's a regular file
    if (!FileUtils::isRegular(arg.toLatin1())){
        cerr << "the file " << arg.toStdString()<<" is not a regular file" << endl;
        exit(EXIT_CANT_WRITE);
    }

    //are perms to read???
    if (!FileUtils::checkWritePerm(arg.toLatin1())){
        cerr << "you don't have write permissions to write at : "<< arg.toStdString()<< endl;
        exit(EXIT_CANT_WRITE);
    }

    //try to create the XML
    xml = new XmlConfig(arg, QIODevice::ReadWrite);
    if (!xml->wellFormed()){
        cerr << "the file " << arg.toStdString() <<" is not a valid XML file." << endl;
        cerr << "or it has no read permissions."<< endl;
        delete xml;
        exit(EXIT_NOT_XML);
    }
}



void XmlWrite::errorExit(QString output){
    if (!output.isEmpty()){
        cerr << output.toStdString() << endl;
    }
    HelpClass help;
    help.writeHelp();
    exit(EXIT_ERROR);
}

