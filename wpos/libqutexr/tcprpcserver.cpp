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

#include "tcprpcserver.h"
#include "rpcsocket.h"
#include <iostream>

TcpRpcServer::TcpRpcServer(PayloadFormater& encoder, Port port, QObject* parent)
    : RpcServerImp(parent), m_encoder{encoder}, m_port{port}
{
    listener = new QTcpServer(this);
    connect(listener, &QTcpServer::newConnection, this, &TcpRpcServer::newConnection);
}

TcpRpcServer::~TcpRpcServer(){
    close();
}

//void TcpRpcServer::serveNewConnection(){
//    auto client = tcp_server->nextPendingConnection();
//    connect(client, &QTcpSocket::readyRead, this, &TcpRpcServer::readFromClient);
//    connect(client, &QTcpSocket::aboutToClose, this, &TcpRpcServer::deleteClient);
//    connect(client, &QTcpSocket::stateChanged, this, &TcpRpcServer::socketStateChanged);

//    qDebug() << "New Connection From " + clientAddress(*client);
//}

AbstractSocket* TcpRpcServer::nextPendingConnection(){
    auto client = listener->nextPendingConnection();
    std::cout << "\nNew Connection From " + clientAddress(*client).toStdString() << std::endl;
    return new RpcTcpSocket(client, this);
}

//AbstractSocket* TcpRpcServer::nextPendingConnection(){
//    return new RpcTcpSocket(tcp_server->nextPendingConnection(), this);
//}

void TcpRpcServer::setMaxPendingConnections(int numConnections){
    listener->setMaxPendingConnections(numConnections);
}

QString TcpRpcServer::errorString() const {
    return listener->errorString();
}

void TcpRpcServer::close(){
    std::cout << "Closing the Server : Will drop all connections" << std::endl;
    listener->close();
}

bool TcpRpcServer::listen(const QHostAddress &address, Port port){
   return  listener->listen(address, port);
}

TcpRpcServer::Port TcpRpcServer::serverPort(){
    return m_port;
}

QString TcpRpcServer::serverName(){
    return RpcServerImp::SERVER_NAME;
}
