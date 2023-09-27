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

#include "xrmethodcall.h"
#include "xrvariant.h"

//XRMethodCall::XRMethodCall():
//    QDomDocument(),
//    _method_name{},
//    _params{}
//{
//    //	QString aux = "<?xml version='1.0'?>";
//    //	this->setContent(aux);
//}

XRMethodCall::XRMethodCall(const QString& method,
                           const QList<QVariant>& params)
    : QDomDocument(),
      _method_name{method},
      _params{params}
{
    setContent(QString("<?xml version='1.0'?>"));
    toDomDoc(_method_name, _params, *this);
}

bool XRMethodCall::parseXmlRpc()
{
    return fromDomDoc(*this, _method_name, _params);
}

bool XRMethodCall::fromDomDoc(const QDomDocument& doc,
                              QString& method_name,
                              QList<QVariant>& params)
{
    //Reset method name and params:
    method_name.clear();
    params.clear();

    QDomElement root_element = doc.documentElement();
    if( root_element.tagName() != "methodCall" ) return false;

    QDomNode child_node = root_element.firstChild();
    while( !child_node.isNull() ) {
        QDomElement child_element = child_node.toElement();
        if( !child_element.isNull() ) {
            if( child_element.tagName() == "methodName" ) {
                method_name = child_element.text();
            }
            if( child_element.tagName() == "params") {
                QDomNode p = child_element.firstChild();
                while( !p.isNull() ) {
                    QDomElement param = p.toElement();
                    if( param.tagName() == "param") {
                        QDomNode value = param.firstChild();
                        if( value.isNull() ) {
                            return false;
                        }
                        else {
                            QDomElement value_e = value.toElement();
                            params.push_back( XRVariant(value_e) );
                        }
                    }
                    p = p.nextSibling();
                }
            }
        }
        child_node = child_node.nextSibling();
    }
    return true;
}

void XRMethodCall::toDomDoc(const QString& method,
                            const QList<QVariant>& params,
                            QDomDocument& xml_method_call)
{

    QDomElement root = xml_method_call.createElement("methodCall");
    xml_method_call.appendChild(root);
    
    QDomElement mn = xml_method_call.createElement("methodName");
    root.appendChild(mn);
    QDomText method_t = xml_method_call.createTextNode(method);
    mn.appendChild(method_t);

    QDomElement params_xml = xml_method_call.createElement("params");
    root.appendChild(params_xml);

    QList<QVariant>::ConstIterator p_it;
    QDomElement param_node;
    for(p_it = params.begin(); p_it != params.end(); p_it++) {
        param_node = xml_method_call.createElement("param");
        param_node.appendChild( XRVariant((*p_it)).toDomElement(xml_method_call) );
        params_xml.appendChild(param_node);
    }
}
