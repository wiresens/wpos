/***************************************************************************
                          xmlread.cpp  -  description
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

#include "xmlread.h"
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


XmlRead::XmlRead(){
xml=0;
}
XmlRead::~XmlRead(){
delete xml;
}


void XmlRead::read(QStringList *list){
QString arg;

          
     //check if there are enought parameters.
     if ((list->count()<2)||(list->count()>25)){
          errorExit();
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
               this->readAttribute(list);
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


     this->readNodes(arg,list);
     exit(EXIT_OK);
}


void XmlRead::getXml(QString file_name){
     if (!FileUtils::exists(file_name.toLatin1())){
          cerr << "can't stat the file " << file_name.toStdString()<< endl;
          cerr <<"The file does not exists or you've no permissions to reach the file" << endl;
          exit(EXIT_CANT_READ);
     }

     //check that it can be readed and it'sa regular file
     if (!FileUtils::isRegular(file_name.toLatin1())){
          cerr << "the file " << file_name.toStdString()<<" is not a regular file" << endl;
          exit(EXIT_CANT_READ);
     }
     //are perms to read???
     if (!FileUtils::checkReadPerm(file_name.toLatin1())){
          cerr << "the file "<< file_name.toStdString() << "has not read permissions" << endl;
          exit(EXIT_CANT_READ);
     }

     //try to create the XML
     xml = new XmlConfig(file_name);
     if (!xml->wellFormed()){
          cerr << "the file " << file_name.toStdString() <<" is not a valid XML file." << endl;
          delete xml;
          exit(EXIT_NOT_XML);
     }
}



void XmlRead::readNodes(QString file_name,QStringList *list){
int error_number;
QString arg;
     arg=file_name;


     this->getXml(arg);

     if (list->isEmpty()){
          errorExit("No domains have been specified");
     }

     error_number = EXIT_OK;

     //check if the domain exists.
     while (!list->isEmpty()){
          arg = list->takeFirst();

          xml->delDomain();
          //check if the domain exists
          if (!xml->setDomain(arg)){
               cerr << "the node "<< arg.toStdString() << " does not exists"<< endl;
               error_number = EXIT_BAD_DOMAIN;
          }
          xml->delDomain();
          arg = xml->readString(arg);
          cout << arg.toStdString() << endl;
     }

     cout << endl;
     delete xml;
     exit(error_number);
}




void XmlRead::readAttribute(QStringList *list){
QString arg;
QString domain;
QString attr;

     //get the file name
     arg = list->takeFirst();

     this->getXml(arg);

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
          errorExit("there are no attributes to read");
     }

     while (!list->isEmpty()){
          attr = list->takeFirst();

          arg = xml->readAttribute(domain,attr);
          cout << arg.toStdString() << endl;
     }
     
     cout << endl;
     exit(EXIT_OK);
}


void XmlRead::errorExit(QString output){

     if (!output.isEmpty()){
          cerr << output.toStdString() << endl;
     }
     HelpClass help;
     help.readHelp();
     exit(EXIT_ERROR);
}
