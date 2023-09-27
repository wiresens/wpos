/*

qutexrlib QT-based XML-RPC library
Copyright (C) 2003  P. Oscar Boykin <boykin@pobox.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Modified by Carlos Manzanedo <carlos@napsis.com>
Ported to Qt 5.12.8 by Gilles Bene Pougoue <gilles.benepougoue@gmail.com>

*/

#include "xrvariant.h"

XRVariant::XRVariant(const QVariant& aqv): QVariant{aqv}{}

XRVariant::XRVariant(QDomElement& qde)
{
    fromDomElement(qde);
}

XRVariant XRVariant::arrayFromDomElement(QDomElement& qde)
{
    QDomNode a_n = qde.firstChild();
    QDomElement tmp_a = a_n.toElement();

    if( tmp_a.tagName() == "data" ) {
        //All arrays must have data
        a_n = tmp_a.firstChild();
        QList<QVariant> tmp_list;
        bool failed = false;

        while((!a_n.isNull()) && (failed == false)) {
            QDomElement a_e = a_n.toElement();
            XRVariant tmp_xrv(a_e);
            if(tmp_xrv.typeName() == 0) {
                //This means it is an invalid variant.
                failed = true;
            }
            else {
                tmp_list.push_back(tmp_xrv);
            }

            a_n = a_n.nextSibling();
        }

        if( failed == false ) {
            return QVariant(tmp_list);
        }
    }
    return QVariant();
}

void XRVariant::fromDomElement(QDomElement& qde)
{
    if( !qde.isNull() ) {
        //qde should be a dom node which holds a value.  Check to see:
        if( qde.tagName() == "value" ){ //This looks good so far.
            if( qde.hasChildNodes() ) { //According to the xml-rpc spec a value should have only one child:
                QDomElement child_elt {qde.firstChild().toElement()};
                if( child_elt.isNull() )
                    this->QVariant::operator=( QVariant(qde.text()) );

                if( child_elt.tagName() == "i4" || child_elt.tagName() == "int" )
                    this->QVariant::operator=( QVariant( child_elt.text().toInt() ) );

                else if( child_elt.tagName() == "boolean"){
                    if( child_elt.text() == "0" )
                        //The second arg is a dummy that Qt needs for compatibility
                        this->QVariant::operator=( QVariant(false) );

                    else if(child_elt.text() == "1")
                        //The second arg is a dummy that Qt needs for compatibility
                        this->QVariant::operator=( QVariant(true) );

                }
                else if( child_elt.tagName() == "string") {
                    QString aux;
                    aux = child_elt.text();
                    aux = this->checkXml(aux);
                    this->QVariant::operator=( QVariant(aux));
                }
                else if( child_elt.tagName() == "double")
                    this->QVariant::operator=( QVariant( child_elt.text().toDouble()) );

                else if( child_elt.tagName() == "dateTime.iso8601"){
                    //We must insert the "-" for Qt to recogize them:
                    QString tmps_date{child_elt.text()};
                    tmps_date.insert(4,'-');
                    tmps_date.insert(7,'-');
                    QVariant tmp_date{ QDateTime::fromString(tmps_date,Qt::ISODateWithMs)};
                    if( tmp_date.isValid() )
                        this->QVariant::operator=( tmp_date );
                }
                else if( child_elt.tagName() == "base64")
                    this->QVariant::operator=( QVariant( XRBase64::decode( child_elt.text()) ) );

                else if( child_elt.tagName() == "array")
                    this->operator=( arrayFromDomElement(child_elt) );

                else if( child_elt.tagName() == "struct")
                    this->operator=( structFromDomElement(child_elt) );
            }
        }
    }
}

bool XRVariant::isXmlRpcType(const QVariant& qv)
{
    if( qv.type() == QVariant::String ||
            qv.type() == QVariant::Int ||
            qv.type() == QVariant::Bool ||
            qv.type() == QVariant::Double ||
            qv.type() == QVariant::DateTime ||
            qv.type() == QVariant::ByteArray )
    {
        return true;
    }
    else if( qv.type() == QVariant::List ) {
        bool all_good{true};

        for( const auto v : qv.toList() )
            all_good = all_good && isXmlRpcType(v);
        return all_good;
    }
    else if( qv.type() == QVariant::Map ) {
        bool all_good{true};

        for( auto it = qv.toMap().cbegin(); it != qv.toMap().cend(); it++) {
            all_good = all_good && isXmlRpcType( *it);
        }

        for(const auto v : qv.toMap()){
            all_good = all_good && isXmlRpcType( v );
        }
        return all_good;
    }
    return false;
}

XRVariant XRVariant::structFromDomElement(QDomElement& qde)
{
    QMap<QString, QVariant> tmp_map;
    QDomNode a_n = qde.firstChild();
    bool failed = false;
    while( (!a_n.isNull()) && (!failed)) {
        QDomElement a_e = a_n.toElement();
        if( a_e.tagName() == "member" ) {
            bool have_name=false, have_value=false;
            QDomNode one_node = a_e.firstChild();
            QString member_name;
            QVariant member_value;

            while( !one_node.isNull() ) {
                QDomElement member_e = one_node.toElement();
                if( member_e.tagName() == "name") {
                    have_name=true;
                    member_name = member_e.text();
                }
                else if( member_e.tagName() == "value") {
                    have_value=true;
                    member_value = XRVariant(member_e);
                }

                if( have_name && have_value ) {
                    tmp_map.insert(member_name, member_value);
                    have_name=false;
                    have_value=false;
                }

                one_node = one_node.nextSibling();
            }
        }
        a_n = a_n.nextSibling();
    }
    return QVariant(tmp_map);
}

QDomElement XRVariant::toDomElement(QDomDocument& doc) const
{
    QDomElement ret_el = doc.createElement("value");
    QDomElement type_el;
    QDomText t;

    QVariant::Type this_type = type();

    if( this_type == QVariant::String /*||
                this_type == QVariant::CString */)
    {
        type_el = doc.createElement("string");
        t = doc.createTextNode(toString());
        type_el.appendChild(t);
    }
    else if( this_type == QVariant::Int || this_type == QVariant::UInt) {
        type_el = doc.createElement("int");
        t = doc.createTextNode(toString());
        type_el.appendChild(t);
    }
    else if( this_type == QVariant::Bool ) {
        type_el = doc.createElement("boolean");
        if( toBool() )
            t = doc.createTextNode("1");
        else
            t = doc.createTextNode("0");
        type_el.appendChild(t);
    }
    else if( this_type == QVariant::Double ) {
        type_el = doc.createElement("double");
        t = doc.createTextNode(toString());
        type_el.appendChild(t);
    }
    else if( this_type == QVariant::DateTime ) {
        type_el = doc.createElement("dateTime.iso8601");

        /* QT has ISODate type, but since XML-RPC requires
         * it to be exact, just tell it the format
        */
        QString tmps_date(toDateTime().toString("yyyyMMddThh:mm:ss.zzz"));
        t = doc.createTextNode( tmps_date );
        type_el.appendChild(t);
    }
    else if( this_type == QVariant::Date ) {
        type_el = doc.createElement("dateTime.iso8601");

        /*
         * We will store the date with Time 00:00:00
        */
        QDateTime tmp_dt(toDate(), QTime(0,0));
        QString tmps_date(tmp_dt.toString("yyyyMMddThh:mm:ss"));
        t = doc.createTextNode( tmps_date );
        type_el.appendChild(t);
    }
    else if( this_type == QVariant::ByteArray ) {
        type_el = doc.createElement("base64");
        t = doc.createTextNode( XRBase64::encode( toByteArray() ));
        type_el.appendChild(t);
    }
    else if( this_type == QVariant::List || this_type == QVariant::StringList) {
        type_el = doc.createElement("array");
        QDomElement data_el = doc.createElement("data");
        type_el.appendChild(data_el);

        for( const auto  v : toList() )
            data_el.appendChild( XRVariant(v).toDomElement(doc) );
    }
    else if( this_type == QVariant::Map ){
        type_el = doc.createElement("struct");

        QMap<QString, QVariant>::ConstIterator it;
        for(it = toMap().cbegin(); it != toMap().cend(); it++) {
            QDomElement mem_el = doc.createElement("member");
            //Add the name:
            QDomElement name_el = doc.createElement("name");
            t = doc.createTextNode(it.key());
            name_el.appendChild(t);
            mem_el.appendChild(name_el);
            //Add the value:
            mem_el.appendChild( XRVariant(*it).toDomElement(doc) );
            //Put the member into the struct:
            type_el.appendChild(mem_el);
        }
    }
    else if( canConvert( QVariant::String ) ) {
        /* Here is a type we don't know what else to do with,
        * but we can cast it into a string
        */
        type_el = doc.createElement("string");
        t = doc.createTextNode(toString());
        type_el.appendChild(t);
    }
    else {
        /* this is the catch all, empty string */
        type_el = doc.createElement("string");
        t = doc.createTextNode("");
        type_el.appendChild(t);
    }
    ret_el.appendChild(type_el);
    return ret_el;
}

QString XRVariant::checkXml(const QString& str){
    QString aux{str};
    if (aux.contains("---XML---")) {
        aux.replace("---XML---","");
        aux.replace("+++","<");
        aux.replace("---",">");
    }
    return aux;
}

























