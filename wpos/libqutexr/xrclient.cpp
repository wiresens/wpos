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

#include "xrclient.h"
#include <QVariant>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <iostream>

const QString XRClient::USER_AGENT{"Wiresens QuteRpc/0.0"};

//don't waste bytes on indenting, so we set indent to 0
const int XRClient::CONTENT_INDENTATION{0};
const size_t XRClient::PLAINTEXT_MAX_SIZE{64};

XRClient::XRClient(const QUrl& server_url, QObject* parent):
    QObject(parent), m_server_url{server_url}{

    if( ! m_server_url.isValid() || m_server_url.port() <= 1024) throw QException{};

    qDebug() << "Starting XML RPC Client User Agent : " << XRClient::USER_AGENT;
//    QNetworkProxy proxy;
//    proxy.setType(QNetworkProxy::HttpProxy);
//    proxy.setHostName("172.16.174.2");
//    proxy.setPort(8080);
//    QNetworkProxy::setApplicationProxy(proxy);

    network_mgr = new QNetworkAccessManager(this);
    connect( network_mgr, &QNetworkAccessManager::finished,
             this, &XRClient::processHttpResponse );
}

XRClient::~XRClient()
{
    /* tell the http client to abort all on-going requests
   * this may result in the slot processHttpResponse()
   * being called for the current request.
   */
    //  networkManager->abort();
    network_mgr->clearConnectionCache();

    /* now we must delete all the buffers */
    //    for( auto it = _buffer_map.begin(); it != _buffer_map.end(); it++) {
    //        delete it.value();
    //    }
}

void XRClient::processHttpResponse(QNetworkReply* reply){

    if( reply->error() != QNetworkReply::NoError )
        /* Use the standard fault codes at
        * http://xmlrpc-epi.sourceforge.net/specs/rfc.fault_codes.php.*/
//        emit fault(reply->error(), XR_TRANSPORT_ERROR, reply->errorString());
        emit fault(reply->error(), reply->error(), reply->errorString());
    else{
        XRMethodResponse xml_response;
        QString parse_error_string, contents;
        bool no_parse_error{ false };
        if( !isDeflated(*reply) )
            contents = reply->readAll();
        else
            contents = qUncompress(reply->readAll());

        contents = processXmlResponse(contents);
        no_parse_error = xml_response.setContent(contents, &parse_error_string );
        if( no_parse_error ){
            if( xml_response.parseXmlRpc() ){

                int fault_code;
                QString fault_string;

                if( xml_response.getFault(fault_code, fault_string) )
                    emit fault(m_replies[reply], fault_code, fault_string);
                else{
                    /* It looks good! */
                    qDebug() << XRClient::USER_AGENT << " : Processing Remote Server Reply ";
                    emit methodResponse( m_replies[reply], xml_response.getResponse());
                    qDebug() << XRClient::USER_AGENT << " : Remote Server Reply Parsed Succesfully";
                }
            }
            else{
                  /** Use the standard fault codes at
                   *  http://xmlrpc-epi.sourceforge.net/specs/rfc.fault_codes.php.
                   */
                emit fault(m_replies[reply], XR_SERVER_ERROR_INVALID_XMLRPC,
                           "Server Error: Recieved Bad XML-RPC Grammar From Remote Server");
            }
        }
        else {
            emit fault(m_replies[reply], XR_PARSE_ERROR_NOT_WELL_FORMED, parse_error_string);
        }
    }
    reply->deleteLater();
    m_replies.remove(reply);
}

int XRClient::call(const QString& method, const QList<QVariant>& params){

    //  Serialize the request
    QByteArray payload;
    QTextStream payload_stream(&payload, QIODevice::WriteOnly);
    payload_stream.setEncoding(QStringConverter::System);

    XRMethodCall xmlcall(method, params);
    xmlcall.save( payload_stream, XRClient::CONTENT_INDENTATION );

    /* make the buffer and make the call */
    QNetworkRequest request;
    request.setUrl(m_server_url);
    request.setHeader(QNetworkRequest::UserAgentHeader, QVariant(USER_AGENT));

    /* XML-RPC *REQUIRES* the following to be set: */

    if( payload.size() > XRClient::PLAINTEXT_MAX_SIZE ){
        request.setRawHeader("Content-Encoding", "gzip, deflate");
        QByteArray compressed_payload = qCompress(payload);
        std::cout << "Request : Uncompressed " << payload.size() <<", Compressed " << compressed_payload.size() << "\n";
        payload = compressed_payload;
    }
    request.setHeader(QNetworkRequest::ContentLengthHeader, payload.size());
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/xml"));

    if( m_accept_compressed ) {
        /* Accept deflated content */
        //No need to specify. It's the default with HTTP/1.1
        // request.setHeader(QNetworkRequest::AcceptEncodingHeader, QVariant("deflate"));
    }

    qDebug() << XRClient::USER_AGENT << " : Contacting Remote server at " << m_server_url.toString() ;
    auto reply = network_mgr->post(request, payload);
    qDebug() << XRClient::USER_AGENT << " : Waiting for Remote server reply ";
    return m_replies[reply] = ++m_curr_req;
}

//int XRClient::call(const QString& method, const QList<QVariant>& params){
//    /** Bridge and Template Method Design Pattern
//     * All the calls to imp.method() , will be replaced by calls  method()
//     * which will be virtual final for must of them. The impl methods are all
//     * pure virtual methods.
//     * and XRClient will implement them by forwading as :
//     * XRClient::method(params){
//     *  imp.method(params);
//     * }
//     * The imp object will be stored as referenced in XRClient and initialized at
//     * in XRClient constructor. We use reference instead of pointer to reduced
//     * systems call for memory allocaton( new calls)
//     */
//    auto methodcall = imp.methodCall(method, params);
//    //  Serialize the request
//    QByteArray payload;
//    auto stream = imp.stream(&payload, imp.openMode());
//    stream.setCodec( imp.codec());

//    methodcall.save( stream, imp.indent() );
//    QNetworkRequest request = imp.buildNetworkRequest(payload);

//    /* We garanty sure that all these hearders are set to these
//     * values, event if they were already set by imp*/
//    request.setUrl(imp.url());
//    request.setHeader(QNetworkRequest::UserAgentHeader, QVariant(imp.userAgent()));
//    request.setHeader(QNetworkRequest::AcceptEncodingHeader, QVariant(imp.incoding()));

//    /* RPC *REQUIRES* the following to be set: */
//    request.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(payload.size()));
//    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(imp.contentType()));

//    qDebug() << imp->userAgent() << " : Contacting Remote server at " << imp->url().toString() ;
//    auto reply = network_mgr->post(request, payload);
//    qDebug() << imp->userAgent() << " : Waiting for Remote server reply ";
//    return _req[reply] = ++_curr_req;
//}

bool XRClient::isDeflated(const QNetworkReply &reply){
     return reply.hasRawHeader("Content-Encoding") &&
            reply.rawHeader("Content-Encoding").contains( "deflate" );
}

//bool XRClient::isDeflated(const QNetworkReply &reply){
//    m_is_deflated = false;
//    if( reply.hasRawHeader("Content-Encoding"))
//        if( reply.rawHeader("Content-Encoding").contains( "deflate" ) )
//            m_is_deflated = true;
//        else
//            m_is_deflated = false;
//    return m_is_deflated;
//}

void XRClient::setUrl(const QUrl& url){
    m_server_url = url;
}

void XRClient::setUrl(const QString& url_str){
    m_server_url.setPath(url_str);
}

QString XRClient::processXmlResponse(const QString& xml_response){
    QString xml_value, ret;
    int pos{0}, start{0}, end{0};

    if (xml_response.isEmpty()) return xml_response;

    //check if there is a xml inside the xml
    if ( xml_response.contains("<?xml") )
        return xml_response;

    //there is a xml inside the xml... we are going to substitute the <?xml version.....?> to other value
    //to make posible the change
    start = xml_response.indexOf("?>");
    start += 2;
    ret = xml_response.mid(pos, start);
    pos = start;
    start = xml_response.indexOf("<\?xml version", start);

    while ( start > 0 ){
        ret += xml_response.mid(pos,start-pos);
        end = xml_response.indexOf("</AUTOMATICALLY_GENERATED_XML>", start);
        xml_value = xml_response.mid(start, (end+30) - start);
        xml_value.replace('<',"+++");
        xml_value.replace('>',"---");
        ret += "---XML---" + xml_value;
        pos = end +30;
        start = end + 30;
        start  = xml_response.indexOf("<\?xml version",start);
    }

    if ( pos < xml_response.length())
        ret += xml_response.mid(pos,xml_response.length() - pos);

    return ret;
}
