/***************************************************************************
                          xmllist.cpp  -  description
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

#include "xmllist.h"
#include "comon.h"
#include "helpclass.h"
#include "fileutils.h"

//xml libs
#include <xmlconfig.h>

extern "C"{
     #include <sys/types.h>
     #include <sys/stat.h>
     #include <unistd.h>
}

#include <QString>
#include <QRegExp>

XmlList::XmlList():xml{nullptr}{

}
XmlList::~XmlList(){
delete xml;
}

void XmlList::list (QStringList *list){
QString arg;


     if ((list->count()<1)||(list->count()>20)){
          HelpClass help;
          help.listHelp();
          exit(EXIT_ERROR);
     }

     //check if the file exists. and create the xml
     arg = list->first();
     list->removeFirst();

     if (list->isEmpty()){
          this->getXml(arg);
          xml->delDomain();
          xml->debug();
     }
     else{
          if (
               (arg=="nodes")||
               (arg=="node")||
               (arg=="nod")||
               (arg=="no")||
               (arg=="n")
               ){
                    this->listNodes(list);
               }
          else if (
               (arg=="attributes")||
               (arg=="attr")||
               (arg=="att")||
               (arg=="at")||
               (arg=="a")
               ){
                    this->listAttributes(list);
               }
          else{
               this->getXml(arg);
               this->partialList(list);

          }
     }
     delete xml;
     exit(EXIT_OK);
}


void XmlList::getXml(QString arg){
     if (!FileUtils::exists(arg.toLatin1())){
          cerr << "can't stat the file " << arg.toStdString() << endl;
          cerr <<"The file does not exists or you've no permissions to reach the file" << endl;
          exit(EXIT_CANT_READ);
     }

     //check that it can be readed and it'sa regular file
     if (!FileUtils::isRegular(arg.toLatin1())){
          cerr << "the file " << arg.toStdString()<<" is not a regular file" << endl;
          exit(EXIT_CANT_READ);
     }
     //are perms to read???
     if (!FileUtils::checkReadPerm(arg.toLatin1())){
          cerr << "the file "<< arg.toStdString() << "has not read permissions" << endl;
          exit(EXIT_CANT_READ);
     }

     xml = new XmlConfig(arg);
     if (!xml->wellFormed()){
          cerr << "the file " << arg.toStdString() <<" is not a valid XML file." << endl;
          delete xml;
          exit(EXIT_NOT_XML);
     }
}



void XmlList::listAttributes(QStringList *list){
QString arg;
QString value;
QStringList attr;
     arg = list->takeFirst();

     //if the only parametter is the file name exit with the help
     if (list->isEmpty()){
          HelpClass help;
          help.listHelp();
          exit(EXIT_ERROR);
     }

     this->getXml(arg);
     xml->delDomain();

     arg = list->takeFirst();

     //check if the domain exists.
     xml->delDomain();
     if (!xml->setDomain(arg)){
          cerr << "the domain does not exist at the XML" << endl;
          exit(EXIT_BAD_DOMAIN);
     }

     if (list->isEmpty()){
          value="*";
     }
     else{
          value = list->takeFirst();
     }

     xml->delDomain();
     
     while (!value.isEmpty()){
          QRegExp *exp = new QRegExp(value);
//          exp->setWildcard(true);
          int num;
          num = xml->howManyAttributes(arg);
          for (int i=0;i<num;i++){
              QString aux;
              aux = xml->readAttributeName(arg,i);
              if (exp->exactMatch(aux))
                   attr.append(aux);
          }
          delete exp;
          for(int i=0;i<(int)attr.count();i++){
               cout << attr[i].toStdString() <<endl;
          }
          if (list->isEmpty()){
               value="";
          }
          else{
               value=list->takeFirst();
          }
     }
     exit(EXIT_OK);
}


void XmlList::listNodes(QStringList *list){
QString arg="";
QString value;
QStringList node;
     arg = list->takeFirst();

     this->getXml(arg);
     xml->delDomain();

     arg="";
     if (!list->isEmpty()){
          arg = list->takeFirst();
     }

     //check if the domain exists.
     xml->delDomain();
     if (arg.isEmpty()){
          //we are at the right domain
     }
     else{
          if (!xml->setDomain(arg)){
               cerr << "the domain does not exist at the XML" << endl;
               exit(EXIT_BAD_DOMAIN);
          }
     }

     if (list->isEmpty()){
          value="*";
     }
     else{
          value = list->takeFirst();
     }

     cout << (xml->getDomain()).toStdString() << endl;
     
     while (!value.isEmpty()){
          QRegExp *exp= new QRegExp(value);
//          exp->setWildcard(true);
          node=xml->findNode(*exp,"",false);
          delete exp;
          for(int i=0;i<(int)node.count();i++){
               value = node[i];
               cout << value.toStdString() <<endl;
          }
          cout << endl;
          if (list->isEmpty()){
               value="";
          }
          else{
               value = list->takeFirst();
          }
     }
     exit(EXIT_OK);
}



void XmlList::partialList(QStringList *list){
int ret;
QString domain;
     ret = EXIT_OK;

     if (list->isEmpty()){
          xml->delDomain();
          xml->debug();
          exit(ret);
     }

     while(!list->isEmpty()){
          domain = list->takeFirst();

          //check if the domain exists
          xml->delDomain();
          if (!xml->setDomain(domain)){
               ret = EXIT_BAD_DOMAIN;
          }
          else{
               xml->delDomain();
               xml->debug(domain);
          }
     }

     exit(ret);
}
