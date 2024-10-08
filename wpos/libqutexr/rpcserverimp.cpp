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

#include "rpcserverimp.h"
#include "rpcsocket.h"
#include "rpcencoder.h"

#include "xrfaultcodes.h"
#include <iostream>

const QString RpcServerImp::SERVER_NAME{"WiresensRPC"};
const RpcServerImp::Port RpcServerImp::SERVER_DEFAULT_PORT{18300};
const size_t RpcServerImp::PLAINTEXT_MAX_SIZE{64};

RpcServerImp::RpcServerImp(QObject*parent)
    : QObject(parent)
{}

RpcServerImp::~RpcServerImp(){}

void RpcServerImp::serveNewConnection(){
    AbstractSocket* client = nextPendingConnection();
    connect(client, &AbstractSocket::readyRead, this, &RpcServerImp::readFromClient);
    connect(client, &AbstractSocket::aboutToClose, this, &RpcServerImp::dropClient);
    connect(client, &AbstractSocket::stateChanged, this, &RpcServerImp::showSocketState);
}

void RpcServerImp::readFromClient(){
    AbstractSocket* socket = qobject_cast<AbstractSocket*>( sender() );
    QMap<AbstractSocket*, QString> header_strings;
    header_strings.insert(socket, QString()); //Insert empty header

    if(m_headers.contains(socket)){
        if( m_headers[socket]["Content-Length"].toLongLong() <= socket->bytesAvailable() )
            parseContentAndEmit(socket); //We have a header, we are ready to go:
    }
    else
    {   //We don't have the headers for this connection yet:
        while( socket->canReadLine() && ! m_headers.contains(socket) )
        {
            //Read the header until the last line then cache the header in m_headers
            QString header_line = socket->readLine();
            header_strings[socket] += header_line;
            if( header_line == "\r\n" )
            {  //This is the last header line
                QString data =  header_strings[socket];
                auto pos = data.indexOf("\r\n\r\n");
                auto headers = data.left(pos).trimmed().split("\r\n");
                m_headers[socket] = QMap<QString, QVariant>();
                m_headers[socket]["Command"] = headers[0];

                //Caching the request header
                for( auto i = 1; i < headers.size() ; i++){
                    QStringList elts = headers[i].split(':');
                    if( i == 1 ) m_headers[socket][elts[0]] = elts[1] +":"+ elts[2];
                    else m_headers[socket][elts[0]] = elts[1];
                }

                showRequestHeader(*socket);
                if (! m_headers[socket]["Command"].toString().startsWith("POST") )   //They MUST be posts:
                    sendHttpError(socket, 405, "HTTP Method Not Allowed", "This is an Http RPC server. I only accept POST\n");
                else if (m_headers[socket].contains("Content-Length")){
                    if( m_headers[socket]["Content-Length"].toLongLong() <= socket->bytesAvailable() ){
                        parseContentAndEmit(socket);
                    }
                }
                else
                { //This is not a valid header
                    int curr_req_id = m_next_req_id++;
                    m_req_to_socket.insert(curr_req_id, socket);
                    sendFault(curr_req_id, XR_SERVER_ERROR_INVALID_XMLRPC, "Invalid Headers.  Missing content length");
                }
            }
        }
    }
}

void RpcServerImp::dropClient() {
    AbstractSocket* client = qobject_cast<AbstractSocket*>( sender() );
    cleanCache(client);
}

void RpcServerImp::showSocketState(QAbstractSocket::SocketState state) {

    AbstractSocket* client { qobject_cast<AbstractSocket*>(sender()) };
    auto address = clientAddress(*client);
    QString desc;

    switch (state) {
    case QAbstractSocket::UnconnectedState :
        client->deleteLater();
        desc = "Client %1 Disconnected";
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
        desc = "Client %1 Closing  Connection";

        break;

    case QAbstractSocket::ListeningState :
        desc = "For Internal Use Only.";
        break;
    }
    std::cout << desc.arg(address).toStdString() << "\n";
}

void RpcServerImp::parseContentAndEmit(AbstractSocket* socket){
    QString pareErrorMsg, invalidMethodMsg;

    //save the current request number and increment
    int curr_req_id = m_next_req_id++;
    m_req_to_socket.insert(curr_req_id, socket);
    QByteArray payload = socket->readAll();

    if( isDeflated(socket) ){
        auto uncompressed_payload = qUncompress(payload);
        std::cout << "Request : Uncompressed Payload " << uncompressed_payload.size()
                  <<", Compressed Payload " << payload.size() << "\n";
        payload = uncompressed_payload;
    }

    if( formater().buildMethodCall(payload, pareErrorMsg, invalidMethodMsg) ){
        if( invalidMethodMsg.isEmpty())
        {
            showPayload(formater().callString(), *socket );
            bool is_method_supported = false;
            emit methodCall(curr_req_id, formater().method(), formater().params(), is_method_supported);
            if( !is_method_supported )
                sendFault(curr_req_id, XR_SERVER_ERROR_METHOD_NOT_FOUND, "Method **"+ formater().method()+ "** Not Found");
        }
        else //This is a malformed XML-RPC message:
        {
            showPayload(payload, *socket );
            sendFault(curr_req_id, XR_SERVER_ERROR_INVALID_XMLRPC, invalidMethodMsg);
        }
    }
    else //There was a parse error:
    {
        showPayload(payload, *socket );
        sendFault(curr_req_id, XR_PARSE_ERROR_NOT_WELL_FORMED, pareErrorMsg);
    }
}

void RpcServerImp::sendHttpError(
        AbstractSocket* socket,
        int error_code,
        const QString& reason,
        const QString data)
{
    QNetworkRequest request;
    //We close the connection on errors.
    request.setRawHeader("Connection", "close");
    sendHttpResponse(socket, error_code, reason, request, data);
}


void RpcServerImp::sendFault(
        int req_id,
        int fault_code,
        const QString& fault_string)
{
    //This guarantees that each request is only answered once
    if( m_req_to_socket.contains(req_id) ) {
        AbstractSocket* socket = m_req_to_socket[req_id];
        QNetworkRequest request;
        //The RPC spec requires this:
        formater().buildMethodResponse(fault_code, fault_string);
        request.setHeader(QNetworkRequest::ContentTypeHeader, formater().contentType());
        sendHttpResponse( socket, 200, "OK", request, formater().responseString() );
        m_req_to_socket.remove(req_id);
    }
}

void RpcServerImp::sendHttpResponse(
        AbstractSocket* socket,
        int status_code,
        const QString& reason,
        QNetworkRequest& request,
        QString resp)
{
    if( m_headers.contains( socket ) ){
        //If it is http 1.0 and it doesn't have keep-alive,
        //then close the connection
        if(  m_headers[socket]["Command"].toString().endsWith("HTTP/1.0") &&
             m_headers[socket]["Connection"].toString() != "Keep-Alive" )
            request.setRawHeader(QByteArray("Connection"), QByteArray("close"));

        //This is the body of the response:
        QByteArray payload;
        QTextStream payload_stream( &payload, QIODevice::WriteOnly );
        payload_stream << resp, payload_stream.flush();
        auto uncompressed_payload = payload;

        if( payload.size() > RpcServerImp::PLAINTEXT_MAX_SIZE &&
                m_headers[socket]["Accept-Encoding"].toString().contains("deflate"))
        {
            request.setRawHeader("Content-Encoding", "gzip, deflate");
            QByteArray compressed_payload = qCompress(payload);
            std::cout << "Response : Uncompressed " << payload.size() <<", Compressed " << compressed_payload.size() << "\n";
            payload = compressed_payload;
        }
        request.setHeader(QNetworkRequest::ContentLengthHeader, payload.size());

        //This is the headers:
        QByteArray header;
        QTextStream header_stream(&header, QIODevice::WriteOnly);
        header_stream << "HTTP/1.1 " << status_code <<  " " << reason << "\r\n";

        for( auto header_item : request.rawHeaderList())
            header_stream << header_item << ": " << request.rawHeader(header_item) << "\r\n";
        header_stream << "\r\n", header_stream.flush();

        showReplyHeader(header, *socket);
        showPayload(uncompressed_payload, *socket );

        //Write the header then payload in this order:
        //        socket->write(header), socket->write(payload);
        socket->write(header + payload);

        //        m_headers.erase(m_headers.find(socket));
        m_headers.remove(socket);

        //Close the connection if we are supposed to
        if( request.hasRawHeader(QByteArray("Connection"))
                && request.rawHeader("Connection").compare("close", Qt::CaseInsensitive) == 0)
            socket->close();
    }
}

void RpcServerImp::sendMethodResponse(int req_id, const QVariant& result){
    if( ! m_req_to_socket.contains(req_id) ) return;
    //This guarantees that each request is only answered once
    auto socket = m_req_to_socket[req_id];
    formater().buildMethodResponse(result);
    QNetworkRequest request;
    //The XML-RPC spec requires this:
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(formater().contentType()));
    sendHttpResponse(socket, 200, "OK", request, formater().responseString() );
    m_req_to_socket.remove(req_id);
    //Leave the connection open in order to save tcp setup! (HTTP/1.1)
}

void RpcServerImp::showRequestHeader(AbstractSocket& socket) const {
    if( m_headers.contains(&socket) ){
        std::cout << "\nClient " << clientAddress(socket).toStdString() << " HTTP Request Header :\n";
        for( auto key : m_headers[&socket].keys())
            std::cout << key.toStdString() << " : " << m_headers[&socket][key].toString().toStdString() << "\n" ;
    }
}

void RpcServerImp::showReplyHeader(const QNetworkRequest& reply, const AbstractSocket& socket ) const {
    std::cout << "\nClient " << clientAddress(socket).toStdString() << " HTTP Response Header :\n";
    for( auto header_item : reply.rawHeaderList())
        std::cout << header_item.toStdString() << " : " << reply.rawHeader(header_item).toStdString() << "\n";
}

void RpcServerImp::showReplyHeader(const QByteArray& headerList, const AbstractSocket& socket ) const {
    std::cout << "\nClient " << clientAddress(socket).toStdString() << " HTTP Response Header :\n";
    std::cout << headerList.toStdString() ;
}

void RpcServerImp::showPayload(const QString& payload, const AbstractSocket& socket ) const {
    std::cout << "\nClient " << clientAddress(socket).toStdString() << " Uncompressed Payload :\n";
    std::cout << payload.toStdString() << "\n" ;
}

bool RpcServerImp::isDeflated(AbstractSocket* socket) const{
    return m_headers[socket].contains("Content-Encoding") &&
            m_headers[socket]["Content-Encoding"].toString().contains("deflate");
}

void RpcServerImp::cleanCache(AbstractSocket* socket){
    for(auto req_id : m_req_to_socket.keys())
        if(m_req_to_socket[req_id]->peerPort() == socket->peerPort()){
            m_req_to_socket.remove(req_id);
            QString msg = "ReqId  %1, Socket %2 : Cache Cleaned\n";
            std::cout << msg.arg(QString::number(req_id), QString::number(socket->peerPort())).toStdString();
        }

    if (m_headers.remove(socket))
        std::cout << "Header Socket " << socket->peerPort() << ": Cache Cleaned\n";
}
