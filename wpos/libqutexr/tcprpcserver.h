
/*
wsjsonxmlrpclib QT-based XML/JSON-RPC library
Copyright (C) 2016-2023  P. Gilles Bene <gilles.bene@wiresens.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the MIT License
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

#ifndef H_TCP_RPC_SERVER
#define H_TCP_RPC_SERVER

#include "rpcserverimp.h"

#include <QTextStream>
#include <QtNetwork/QtNetwork>
#include <QString>
#include <QList>
#include <QVariant>
#include <QtXml/QtXml>
#include <QMap>

#include "xrmethodcall.h"
#include "xrmethodresponse.h"
#include "xrfaultcodes.h"

/**
 * Provides a minimal HTTP Server to handle XML/JSON-RPC
 * requests and emit signals when requests are recieved.
 * This server implements only what is required for XML/JSON-RPC, with a
 * few extras.  In particular, connections are left open
 * (when compatible with the HTTP spec).  Leaving TCP
 * connections open results in usually 3 packets being sent
 * per methodCall (assuming small calls and responses), as
 * opposed to 10-12 if the connection is set up and
 * destroyed each time.
 * It also supports sending compressed (deflate) responses
 * which DRAMATICALLY reduces the size of XML/JSON-RPC responses.
 *
 * @see http://www.xmlrpc.com/spec
 * @see http://www.ietf.org/internet-drafts/draft-megacz-xmc-01.txt
 */

class AbstractSocket;
class TcpRpcServer : public RpcServerImp{
    Q_OBJECT

public:
    explicit TcpRpcServer(PayloadFormater& formater, Port port = SERVER_DEFAULT_PORT,  QObject* parent = nullptr);
    ~TcpRpcServer() override;

    //    virtual void serveNewConnection() override;
    bool listen(const QHostAddress&address = QHostAddress::Any, Port port = SERVER_DEFAULT_PORT) override;
    void setMaxPendingConnections(int numConnections) override;
    AbstractSocket* nextPendingConnection() override ;

    void close() override;
    Port serverPort() override;
    QString serverName() override;
    QString errorString() const override;

protected:
    virtual PayloadFormater& formater() override{ return m_encoder; }
//    virtual EncodingStrategy& encoder() override{ return *m_encoder_ptr.get(); }

private:
    QTcpServer* listener{};
    PayloadFormater& m_encoder;
//    std::unique_ptr<EncodingStrategy> m_encoder_ptr{nullptr};
    Port m_port;
};

#endif
