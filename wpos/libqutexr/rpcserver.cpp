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

#include "rpcserver.h"
#include "rpcserverimp.h"

RpcServer::RpcServer(RpcServerImp* imp, QObject * parent)
    : QObject(parent),
      server_imp{imp}
{
    server_imp->setMaxPendingConnections(50);
    connect(server_imp, &RpcServerImp::newConnection, this, &RpcServer::serveNewConnection);
}

bool RpcServer::start(Port port, const QHostAddress &address){

    if( port == 0 ) port = serverPort();
    if ( listen( port, address) ){
        qDebug()<< "Starting " << serverName()
                << " listening on : " << port ;
        return true;
    }

    qDebug() << "Failed to Start  XML RPC Server. Error : "
             << server_imp->errorString();
    return false;
}

void RpcServer::serveNewConnection(){
    server_imp->serveNewConnection();
}

bool RpcServer::listen(Port port , const QHostAddress &address){
    return server_imp->listen(address, port);
}

void RpcServer::close(){
    server_imp->close();
}

RpcServer::Port RpcServer::serverPort(){
    return server_imp->serverPort();
}

QString RpcServer::serverName(){
    return server_imp->serverName();
}

RpcServer::~RpcServer(){
    close();
}
