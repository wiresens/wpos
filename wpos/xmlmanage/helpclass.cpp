/***************************************************************************
                          helpclass.cpp  -  description
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

#include "helpclass.h"
#include <QString>


#include "comon.h"

HelpClass::HelpClass(){}

HelpClass::~HelpClass(){}

void HelpClass::showMainHelp(QString program_name){
    cerr << endl;
    cerr << program_name.toStdString() <<" manage XmlFiles. Its Designed to manage Xml from scripts, to use it at" << endl;
    cerr << "programs use the libbslxml. "<< program_name.toStdString() <<" has three actions, READ, WRITE and LIST" << endl;
    cerr << "the first mode will read a value of a xmlNode, the second will write a value, list will list the xml" << endl;
    cerr << "at the given domain. To obtain help of how to use this commands type : "<< program_name.toStdString() <<" -h [command]"<<endl;
    cerr << endl;
    cerr << "\t-h\t--help\twill show this help"<< endl;
    cerr << "\t-h [command]\t\tshow the usage of this command"<<endl;
    cerr << "\t-r\t--read XML_FILE DOMAIN"<< endl;
    cerr << "\t-d\t--delete XML_FILE DOMAIN" << endl;
    cerr << "\t-w\t--write XML_FILE DOMAIN VALUE"<< endl;
    cerr << "\t-l\t--list XML_FILE [DOMAIN]  (***Not Already implemented***)"<< endl;
    cerr << endl;
    cerr << "examples:"<< endl;
    listExamples(program_name);
    readExamples(program_name);
    writeExamples(program_name);
    cerr << endl << endl;
}

void HelpClass::unmatchedName(){
    cerr << "this program should be named " << PROGRAM_NAME.toStdString() << " or something else" << endl;
    cerr << "please rename the binary to " << PROGRAM_NAME.toStdString() << " and retry the operation" << endl;
    cerr << endl << endl;
}

void HelpClass::listHelp(){
    cerr << "To show or list an xml this command should be used as follows" << endl;
    cerr << "show or list can be both used to show a XML" << endl;
    cerr << endl;
    cerr << "\thelp list\t will show this help" << endl;
    cerr << "\t-l --list XML_FILE \t will show the file" << endl<<endl;
    cerr << "\t-s --show XML_FILE \t will show the file" << endl<<endl;
    cerr << "\t-l --list XML_FILE [DOMAIN]  (***Not Already implemented***)"<< endl<<endl;
    cerr << "\t-l nodes XML_FILE [DOMAIN] [NODE]  will show nodes at the XML"<< endl;
    cerr << "\t                                   if no domain and no node are given"<< endl;
    cerr << "\t                                   default will be root domain and '*' value"<< endl<< endl;
    cerr << "\t-l attribute XML_FILE node [ATTRIBUTE]  will show attribute at the node"<< endl;
    cerr << "\t                                   if no attribute is given default will be '*'"<< endl<<endl;
    cerr << endl << endl;
    this->listExamples(PROGRAM_NAME);
    cerr << endl << endl;
    
}

void HelpClass::readHelp(){
    cerr << "To read a node of an XML you should use the command as follows" << endl;
    cerr << endl;
    cerr << "\thelp read\t will show this help" << endl;
    cerr << "\t-read [node] XML_FILE DOMAIN [DOMAIN...]                 will show the values at the given domains" << endl;
    cerr << "\t-read attribute XML_FILE DOMAIN ATTR [ATTR...]    will show the attributes at the given domain" << endl;
    cerr << endl << endl;
    this->readExamples(PROGRAM_NAME);
    cerr << endl << endl;
    this->domainExplanation();
    cerr << endl << endl;
}

void HelpClass::writeHelp(){
    cerr << "To write a value to a node of an XML you should use the command as follows" << endl;
    cerr << endl;
    cerr << "\thelp read\t will show this help" << endl;
    cerr << "\t-write [node] XML_FILE DOMAIN [VALUE] [DOMAIN VALUE...]           will write value at the given domain" << endl;
    cerr << "\t-write attribute XML_FILE DOMAIN ATTR VALUE [ATTR VALUE] will write an attribute at the domin" << endl;
    cerr << endl << endl;
    writeExamples(PROGRAM_NAME);
    cerr << endl << endl;
    domainExplanation();
    cerr << endl << endl;
}

void HelpClass::deleteHelp(){
    cerr << "To delete a node and subnodes from an XML you should use the command as follows" << endl;
    cerr << endl;
    cerr << "\thelp read\t will show this help" << endl;
    cerr << "\t-delete [node] XML_FILE DOMAIN [DOMAIN...]        will delete the node and subnodes from the given domain" << endl;
    cerr << "\t-delete attribute XML_FILE DOMAIN ATTR [ATTR...]  will delete the node and subnodes from the given domain" << endl;
    cerr << endl << endl;
    deleteExamples(PROGRAM_NAME);
    cerr << endl << endl;
    domainExplanation();
    cerr << endl << endl;

}

void HelpClass::listExamples(QString program_name){
    cerr << "   list of the XML" << endl;
    cerr << "      " << program_name.toStdString() << " -l net.xml"<<endl;
    cerr << "      " << program_name.toStdString() << " -list net.xml"<< endl;
    cerr << "      " << program_name.toStdString() << " show net.xml" << endl;
    cerr << "      " << program_name.toStdString() << " l node net.xml net[0].configuration addr" << endl;
    cerr << "      " << program_name.toStdString() << " ln net.xml net[0].configuration addr" << endl;
    cerr << "      " << program_name.toStdString() << " l attribute net.xml net[0] '*'" << endl;

}
void HelpClass::readExamples(QString program_name){
    cerr << "   reading a value from a domain"<<endl;
    cerr << "      " << program_name.toStdString() << " -r net.xml netcards.netcard.ip.addr"<<endl;
    cerr << "      " << program_name.toStdString() << " -read net.xml netcards.netcard[2].ip.mask" << endl;
    cerr << "      " << program_name.toStdString() << " r node host.xml host[0].ip host[1].ip" << endl;
    cerr << "      " << program_name.toStdString() << " r attribute net.xml net.device name" << endl;
    cerr << "      " << program_name.toStdString() << " ra net.xml net.device name" << endl;
}

void HelpClass::writeExamples(QString program_name){
    cerr << "   writing a value from a domain"<< endl;
    cerr << "      " << program_name.toStdString() << " -w net.xml netcards.netcard.ip.addr 192.168.44.2"<<endl;
    cerr << "      " << program_name.toStdString() << " -wr net.xml netcards.netcard[2].ip.mask 255.255.255.0"<< endl;
    cerr << "      " << program_name.toStdString() << " write net.xml netcards.netcard[2].ip.mask 255.255.255.0"<< endl;
    cerr << "      " << program_name.toStdString() << " w a net.xml net.device name eth0 type ethernet"<< endl;
    cerr << "      " << program_name.toStdString() << " w node net.xml net[0] 10.0.0.0/24 net[1] 10.0.1.0/24"<< endl;
}

void HelpClass::deleteExamples(QString program_name){
    cerr << "   deleting a node and subnodes"<< endl;
    cerr << "      " << program_name.toStdString() << " -delete net.xml netcards.netcard.ip.addr"<<endl;
    cerr << "      " << program_name.toStdString() << " d net.xml netcards.netcard[2]"<< endl;
    cerr << "      " << program_name.toStdString() << " --del net.xml netcards.netcard[2].ip[0]"<< endl;
    cerr << "      " << program_name.toStdString() << " d attr host.xml host[3] OsType << " << endl;

}


void HelpClass::domainExplanation(){
    cerr << "A domain at the XML should be given as follows:" << endl;
    cerr << "the nodes from root node are given as the node name" << endl;
    cerr << "if there are more than one node which name is not unic, a id number should be added" << endl;
    cerr << "id numbers always start at 0" << endl;
    cerr << endl;
    cerr << "Example" <<endl;
    cerr << "\t rootnode.node" << endl;
    cerr << "\t rootnode.node[1]" << endl;
    cerr << endl;
}

int HelpClass::getMatch(const QStringList *list){
    QString command;

    command = (*list)[0];

    //separate the usefull string from the
    if (command[0]=='-')
        command=command.mid(1,command.length());
    if (command[0]=='-')
        command=command.mid(1,command.length());

    //check if it's the help command
    if (command=="help") return Help;
    if (command=="hel") return Help;
    if (command=="he") return Help;
    if (command=="h") return Help;

    if (command=="version") return Version;
    if (command=="ver") return Version;
    if (command=="v") return Version;

    //check if it's the write command
    if (command=="write") return Write;
    if (command=="writ") return Write;
    if (command=="wri") return Write;
    if (command=="wr") return Write;
    if (command=="w") return Write;
    if (command=="writeattributes") return WriteAttr;
    if (command=="writeattr") return WriteAttr;
    if (command=="wattr") return WriteAttr;
    if (command=="watt") return WriteAttr;
    if (command=="wat") return WriteAttr;
    if (command=="wa") return WriteAttr;

    //check if it's the read command
    if (command=="read") return Read;
    if (command=="rea") return Read;
    if (command=="re") return Read;
    if (command=="r") return Read;
    if (command=="readattributes") return ReadAttr;
    if (command=="readattr") return ReadAttr;
    if (command=="rattr") return ReadAttr;
    if (command=="ra") return ReadAttr;


    //the delete command
    if (command=="delete") return Delete;
    if (command=="delet") return Delete;
    if (command=="del") return Delete;
    if (command=="de") return Delete;
    if (command=="d") return Delete;
    if (command=="deleteattribute") return DeleteAttr;
    if (command=="deleteattr") return DeleteAttr;
    if (command=="delattribute") return DeleteAttr;
    if (command=="delattr") return DeleteAttr;
    if (command=="dattr") return DeleteAttr;
    if (command=="da") return DeleteAttr;

    //check if it's the show command
    if (command=="list") return List;
    if (command=="lis") return List;
    if (command=="li") return List;
    if (command=="l") return List;
    if (command=="ls") return List;
    if (command=="show") return List;
    if (command=="sho") return List;
    if (command=="sh") return List;
    if (command=="s") return List;

    if (command=="lnode") return ListNodes;
    if (command=="lnod") return ListNodes;
    if (command=="ln") return ListNodes;

    if (command=="lattributes") return ListAttr;
    if (command=="lattr") return ListAttr;
    if (command=="latt") return ListAttr;
    if (command=="lat") return ListAttr;
    if (command=="la") return ListAttr;
    if (command=="l") return ListAttr;

    return Unmatched;
}

void HelpClass::help(QStringList *list){
    QString arg;
    if(!list->isEmpty()){
        arg = list->first();

        if (
                (arg=="read")||
                (arg=="rea")||
                (arg=="re")||
                (arg=="r")
                ){
            this->readHelp();
        }
        else if(
                (arg=="list")||
                (arg=="ls")||
                (arg=="l")||
                (arg=="show")
                ){
            this->listHelp();
        }
        else if(
                (arg=="write")||
                (arg=="writ")||
                (arg=="wri")||
                (arg=="wr")||
                (arg=="w")
                ){
            this->writeHelp();
        }
        else if(
                (arg=="delete")||
                (arg=="delet")||
                (arg=="dele")||
                (arg=="del")||
                (arg=="de")||
                (arg=="d")
                ){
            this->deleteHelp();
        }
        else{
            this->showMainHelp(PROGRAM_NAME);
            exit(EXIT_ERROR);
        }

    }
    else{
        this->showMainHelp(PROGRAM_NAME);
        exit(EXIT_ERROR);
    }

}

void HelpClass::version(){
    cout << endl;
    cout << "VERSION   "<< VER << "." << REALESE << "  written by Carlos Manzanedo and Jordi Polo, Upgraded by Gilles Bene Pougoue" << endl;
    cout << "XmlManage Program based on libbslxml:  Code licensed under GPLv2 terms by BlueSock" << endl;
    cout << "Code currently maintained by Gilles Bene Pougoue, from WireSens" << endl;
    cout << "gilles.benepougoue@wiresens.com, carlos@bluesock.net,  jordi@bluesock.net, support@bluesock.net" << endl <<endl;
    cout << endl << endl << endl << endl;
    cout << endl << endl;
}
