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

#include "xmlrpcserver.h"
#include <QTcpSocket>
#include <iostream>
#include <QDebug>

const QString XmlRpcServer::SERVER_NAME{"WiresensRPC"};
const unsigned int XmlRpcServer::SERVER_DEFAULT_PORT{18300};

XmlRpcServer::XmlRpcServer(quint16 port, QObject * parent)
    : QObject(parent),
      m_port{port} {}

bool XmlRpcServer::start(){
    xml_rpc_server = new QTcpServer(this);
    xml_rpc_server->setMaxPendingConnections(50);

    connect(xml_rpc_server, &QTcpServer::newConnection, this, &XmlRpcServer::serveNewConnection);

    if (xml_rpc_server->listen(QHostAddress::Any, m_port)){
        qDebug() << "Starting" << XmlRpcServer::SERVER_NAME << " XML RPC Server, Listening On :"
                 << QString::number(xml_rpc_server->serverPort());
        return true;
    }

    qDebug() << "Failed to Start  XML RPC Server. Error : " + xml_rpc_server->errorString();
    return false;
}

XmlRpcServer::~XmlRpcServer(){
    xml_rpc_server->close();
    qDeleteAll(m_open_sockets.begin(), m_open_sockets.end());
}

void XmlRpcServer::serveNewConnection(){
    QTcpSocket* client {xml_rpc_server->nextPendingConnection()};
    connect(client, &QTcpSocket::readyRead, this, &XmlRpcServer::readFromClient);
    connect(client, &QTcpSocket::aboutToClose, this, &XmlRpcServer::deleteClient);
    connect(client, &QTcpSocket::stateChanged, this, &XmlRpcServer::socketStateChanged);

    m_open_sockets << client ;
    qDebug() << "New Connection From " + clientAddress(*client);
}

QHostAddress XmlRpcServer::getPeerForRequest(unsigned int req_id) const{
    QHostAddress result;
    if( m_req_to_socket.contains(req_id) ) {
        auto socket = m_req_to_socket[req_id];
        result = socket->peerAddress();
    }
    return result;
}

void XmlRpcServer::socketStateChanged(QAbstractSocket::SocketState state) {

    QString desc;
    switch (state) {
    case QAbstractSocket::UnconnectedState :
        desc = "Client %1 Disconnected.";
        break;

    case QAbstractSocket::HostLookupState :
        desc = "Client  %1 Performing Host Name Lookup.";
        break;

    case QAbstractSocket::ConnectingState :
        desc = "Client %1 Connecting.";
        break;

    case QAbstractSocket::ConnectedState:
        desc = "Connection %1 Established.";
        break;

    case QAbstractSocket::BoundState:
        desc = "Client %1 Bound to an Address and Port.";
        break;

    case QAbstractSocket::ClosingState:
        desc = "Client %1 Closing Connection.";
        break;

    case QAbstractSocket::ListeningState :
        desc = "For internal use only.";
        break;
    }

    QTcpSocket* client { qobject_cast<QTcpSocket*>(sender()) };
    qDebug() << desc.arg(clientAddress(*client));
}

void XmlRpcServer::deleteClient() {
    QTcpSocket* client = qobject_cast<QTcpSocket*>( sender() );
    qDebug() << "Client " + clientAddress(*client) + " deleted";
    m_open_sockets.removeAll(client);
    //    client->close();
    //    client->deleteLater();
}

void XmlRpcServer::readFromClient(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>( sender() );

    QMap<QTcpSocket*, QString> header_strings;
    //Insert empty header
    header_strings.insert(socket, QString());

    if(m_headers.contains(socket)){
        if( m_headers[socket]["Content-Length"].toLongLong() <= socket->bytesAvailable()){
            //We have a header, we are ready to go:
            parseContentAndEmit(socket);
        }
    }
    else {
        //We don't have the headers for this connection yet:
        while( socket->canReadLine() && ! m_headers.contains(socket) ) {
            QString request_line{ socket->readLine() };
            header_strings[socket] += request_line;

            if( request_line == "\r\n" ) {  //This is the last header line
                QString data =  header_strings[socket];
                auto pos = data.indexOf("\r\n\r\n");
                auto headers = data.left(pos).trimmed().split('\n');
                m_headers[socket] = QMap<QString, QVariant>();
                m_headers[socket]["Command"] = headers[0].trimmed();

                for( auto i = 1; i < headers.size() ; i++)
                {
                    QStringList elts = headers[i].trimmed().split(':');
                    if( i == 1 )
                        m_headers[socket][elts[0]] = QString(elts[1]).trimmed() + ":" + elts[2];
                    else
                        m_headers[socket][elts[0]] = QString(elts[1]).trimmed();
                }

                if (! m_headers[socket]["Command"].toString().startsWith("POST") )   //They MUST be posts:
                    sendHttpError(socket, 405, "HTTP Method Not Allowed", "This is an Http XML-RPC server.  I only accept POST");

                else if (m_headers[socket].contains("Content-Length"))
                {
                    if( m_headers[socket]["Content-Length"].toLongLong() <= socket->bytesAvailable() ){
                        //We are ready to go!
                        parseContentAndEmit(socket);

                        //else we will exit from this function
                        //because the while condition is false.
                        //m_headers.contains(socket) == true now.
                    }
                }
                else
                { //This is not a valid header
                    int curr_req_id = m_last_req_id++;
                    m_req_to_socket.insert(curr_req_id, socket);
                    sendFault(curr_req_id, XR_SERVER_ERROR_INVALID_XMLRPC, "Invalid XML-RPC headers. Missing content length");
                }
            }
        }
    }

    if( m_headers[socket].keys().size() ) {
        qDebug() << "Client " + clientAddress(*socket) << " HTTP Headers :";

        for( auto key : m_headers[socket].keys())
            qDebug() << key + " : " + m_headers[socket][key].toString();
    }
}

/*
 * Parses the XML Payload. If parsed succesfully, emits the methodCalled signal with the is_method_supported
 * sets to false. Any slots that get this signal look to see if it is a method call they handle.  If it is,
 * the first one that can handle it, sets is_method_supported to true.  Any other slot recieving the
 * signal with is_method_supported set to true MUST ignore it. After the methodCall signal is
 * recieved and the call is made, one of the two slots: sendFault or sendMethodResponse must be signalled. ONLY ONE OF THEM, ONE TIME
 */

void XmlRpcServer::parseContentAndEmit(QTcpSocket* socket){
    XRMethodCall xrmc;
    QString errorString;
    int curr_req_id = m_last_req_id++; //save the current request number and increment
    m_req_to_socket.insert(curr_req_id, socket);
    if( xrmc.setContent(socket->readAll(), false, &errorString) ){
        if( xrmc.parseXmlRpc() ){
            bool is_method_supported = false;
            emit methodCall(curr_req_id, xrmc.getMethodName(), xrmc.getParams(), is_method_supported);
            if( !is_method_supported )/*Method Not Found Error, fault codes at http://xmlrpc-epi.sourceforge.net/specs/rfc.fault_codes.php*/
                sendFault(curr_req_id, XR_SERVER_ERROR_METHOD_NOT_FOUND, "Method **"+ xrmc.getMethodName()+ "** Not Found");
        }
        else //This is a malformed XML-RPC message:
            sendFault(curr_req_id, XR_SERVER_ERROR_INVALID_XMLRPC, "Invalid XML-RPC methodCall");
    }
    else //There was a parse error:
        sendFault(curr_req_id, XR_PARSE_ERROR_NOT_WELL_FORMED, errorString);
}

void XmlRpcServer::sendHttpError(
        QTcpSocket* socket,
        int error_code,
        const QString& reason,
        const QString data)
{
    QNetworkRequest request;
    //We close the connection on errors.
    //    request.setAttribute(QNetworkRequest::HttpStatusCodeAttribute, "closed");
    request.setRawHeader(QByteArray("Connection"), QByteArray("close"));
    sendHttpResponse(socket, error_code, reason, request, data);
}

void XmlRpcServer::sendFault( int req_id,int fault_code, const QString& fault_string)
{
    //This guarantees that each request is only answered once
    if( m_req_to_socket.contains(req_id) ){
        QTcpSocket* socket = m_req_to_socket[req_id];
        XRMethodResponse resp(fault_code, fault_string);
        QNetworkRequest request;
        //The XML-RPC spec requires this:
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/xml"));
        sendHttpResponse( socket, 200, "OK", request, resp.toString() );
        m_req_to_socket.erase(m_req_to_socket.find(req_id));
    }
}

void XmlRpcServer::sendMethodResponse(int req, const QVariant& result){
    //This guarantees that each request is only answered once
    if( m_req_to_socket.contains(req) ) {
        auto socket = m_req_to_socket[req];
        XRMethodResponse resp(result);
        QNetworkRequest request;
        //The XML-RPC spec requires this:
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/xml"));
        sendHttpResponse(socket, 200, "OK", request, resp.toString() );
        m_req_to_socket.erase(m_req_to_socket.find(req));
    }
    //Leave the connection open in order to save tcp setup! (http 1.1)
}

void XmlRpcServer::sendHttpResponse(
        QTcpSocket* socket,
        int status_code,
        const QString& reason,
        QNetworkRequest& request,
        QString resp)
{
    if( m_headers.contains( socket ) )
    {
        //If it is http 1.0 and it doesn't have keep-alive,
        //then close the connection
        if(  m_headers[socket]["Command"].toString().endsWith("HTTP/1.0") &&
             m_headers[socket]["Connection"].toString() != "Keep-Alive" )
            request.setRawHeader(QByteArray("Connection"), QByteArray("close"));

        //This is the body of the response:
        QByteArray payload;
        QTextStream ts_resp( &payload, QIODevice::WriteOnly );
        ts_resp << resp;
        ts_resp.flush();
        auto length = payload.size();
        if( length > 256 && m_headers[socket]["Accept-Encoding"].toString().contains("deflate"))
        {
            request.setRawHeader("Content-Encoding", "gzip, deflate");
            QByteArray compressed_payload = qCompress(payload);
            length = compressed_payload.size();
            payload = compressed_payload;
        }
        request.setHeader(QNetworkRequest::ContentLengthHeader, length);

        //This is the headers:
        QByteArray head_out;
        QTextStream header_resp(&head_out, QIODevice::WriteOnly);
        header_resp << "HTTP/1.1 " << status_code <<  " " << reason << "\r\n";

        for( auto header : request.rawHeaderList())
            header_resp << header << ": " << request.rawHeader(header) << "\r\n";

        header_resp << "\r\n";
        header_resp.flush();

        //Write the headers out:
        socket->write(head_out);

        //Write the body out:
        socket->write(payload);
        m_headers.erase(m_headers.find(socket));

        //Close the connection if we are supposed to
        if( request.hasRawHeader(QByteArray("Connection"))
                && request.rawHeader("Connection").compare("close", Qt::CaseInsensitive) == 0)
            socket->close();
    }
}

QString XmlRpcServer::clientAddress(const QTcpSocket& client){
    return std::move( client.peerAddress().toString().replace("::ffff:","") +":"+ QString::number(client.peerPort()));
}
