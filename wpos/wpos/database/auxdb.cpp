/***************************************************************************
                          auxdb.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "auxdb.h"
#include <QVariant>
#include <xmlconfig.h>
#include <QString>

#include <optionnode.h>

#include <iostream>
namespace std{}
using namespace std;

AuxDB::AuxDB(const QString& _connection_name,
             const QString& _hostname,
             const QString& _database,
             const QString& _username,
             const QString& _passwd):
    BasicDatabase(_connection_name,_hostname,_database,_username,_passwd){}

AuxDB::AuxDB(const QString& _connection_name,
             XmlConfig *xml):
    BasicDatabase(_connection_name,xml){}

AuxDB::AuxDB(const QString& _connection_name,
             const QString& configuration_path):
    BasicDatabase(_connection_name,configuration_path){}

AuxDB::~AuxDB(){
}

HList<OptionNode>* AuxDB::getOptionNodes(){
    HList<OptionNode> *option_list = 0;
    OptionNode *o = 0;
    QSqlQuery *q=0;
    QString query;
    QString type;
    QString value;

    if ( !isConnected() )
        return option_list;

    query = "SELECT option_type, prod_option from products_options_list ;";
    q = new QSqlQuery(query,this->getDB());

    //prepare the query execution
    if (!q->isActive()){
        delete q;
        return option_list;
    }
    if (!q->size()){
        delete q;
        return option_list;
    }

    option_list = new HList<OptionNode>;
    while (q->next()){
        type = q->value(0).toString();
        value = q->value(1).toString();
        o = option_list->find(type);
        if (!o){
            o = new OptionNode(type);
            option_list->append(o,type);
            //                        cout << "INSERTANDO OPCION ---> " << type << endl;
        }
        o->addOption(value);
        //                cout << "ANADIDA OPCION TIPO :"<< type <<" VALOR :" << value << endl;
    }
    delete q;
    return option_list;
}
