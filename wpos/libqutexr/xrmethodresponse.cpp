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

#include "xrmethodresponse.h"

//XRMethodResponse::XRMethodResponse():
//    QDomDocument{}
//{}

XRMethodResponse::XRMethodResponse(const QVariant& resp) :
    XRMethodResponse()

{
    _response = resp ;
    responseToDomDoc(_response, *this);
}

XRMethodResponse::XRMethodResponse(int fault_code, const QString& str):
    XRMethodResponse()
{
    _fault_string = str;
    _fault_code = fault_code;
    _is_fault = true;
    faultToDomDoc(_fault_code, _fault_string, *this);
}

void XRMethodResponse::faultToDomDoc(
        int fault_code,
        const QString& fault_string,
        QDomDocument& doc)
{
    QDomElement root{doc.createElement("methodResponse")};
    doc.appendChild(root);

    QDomElement fault{doc.createElement("fault")};
    root.appendChild(fault);

    QMap<QString, QVariant> fault_map;
    fault_map.insert("faultCode", QVariant(fault_code));
    fault_map.insert("faultString" , QVariant(fault_string));
//    _response = QVariant(fault_map);

    fault.appendChild( XRVariant(fault_map).toDomElement(doc) );
}

bool XRMethodResponse::fromDomDoc(const QDomDocument& doc,
                                  QVariant& result,
                                  bool& isFault){
//   QDomNodeList nodes;
   QDomElement docElem{doc.documentElement()};

   if( docElem.tagName() != "methodResponse" )
       return false;

   QDomNode node{docElem.firstChild()};
   while( !node.isNull() ) {
       QDomElement elmt = node.toElement();
       if( !elmt.isNull() ) {
           if( elmt.tagName() == "fault" ) {
               QDomNode fault_value = elmt.firstChild();
               QDomElement fault_e = fault_value.toElement();
               XRVariant fault(fault_e);
               if( fault.type() == QVariant::Map ){
//                   if( fault.mapFind("faultCode") != fault.mapEnd() && fault.mapFind("faultString") != fault.mapEnd() ){
                   if( fault.toMap().contains("faultCode")  && fault.toMap().contains("faultString")){
                       isFault = true;
                       result = fault;
                       return true;
                   }
               }
               else{
                   return false;
               }
           }
           if( elmt.tagName() == "params") {
               QDomNode p = elmt.firstChild();
               if( !p.isNull() ) {
                   QDomElement param = p.toElement();
                   if( param.tagName() == "param") {
                       QDomNode value = param.firstChild();
                       if( value.isNull() ) {
                           return false;
                       }
                       else {
                           QDomElement value_e = value.toElement();
                           result = XRVariant(value_e);
                       }
                   }
               }
               else {
                   return false;
               }
           }
       }
       node = node.nextSibling();
   }
   return true;
}

bool XRMethodResponse::getFault(int& fault_code, QString& fault_string) const
{
  if( _is_fault ) {
    fault_code = _fault_code;
    fault_string = _fault_string;
  }
  return _is_fault;
}

bool XRMethodResponse::parseXmlRpc() {

   bool result = fromDomDoc(*this, _response, _is_fault);
   if( _is_fault ) {
       auto map = _response.toMap();
       _fault_code = map["faultCode"].toInt();;
       _fault_string = map["faultString"].toString();
       _response.clear();
   }
   return result;
}

void XRMethodResponse::responseToDomDoc(const QVariant& result, QDomDocument& doc)
{
    QDomElement root = doc.createElement("methodResponse");
    doc.appendChild(root);
    QDomElement params_xml = doc.createElement("params");
    root.appendChild(params_xml);
    QDomElement param_node = doc.createElement("param");
    param_node.appendChild( XRVariant(result).toDomElement(doc) );
    params_xml.appendChild(param_node);
}
