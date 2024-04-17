
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

#ifndef H_RPC_SERVER
#define H_RPC_SERVER

#include <QTextStream>
#include <QtNetwork/QtNetwork>
#include <QString>
#include <QList>
#include <QVariant>
// #include <QRegExp>
#include <QtXml/QtXml>
#include <QMap>

#include "xrmethodcall.h"
#include "xrmethodresponse.h"
#include "xrfaultcodes.h"


/**
 * Provides a minimal HTTP Server to handle XML-RPC
 * requests and emit signals when requests are recieved.
 * This server implements only what is required for XML-RPC, with a
 * few extras.  In particular, connections are left open
 * (when compatible with the HTTP spec).  Leaving TCP
 * connections open results in usually 3 packets being sent
 * per methodCall (assuming small calls and responses), as
 * opposed to 10-12 if the connection is set up and
 * destroyed each time.
 * It also supports sending compressed (deflate) responses
 * which DRAMATICALLY reduces the size of XML-RPC responses.
 *
 * @see http://www.xmlrpc.com/spec
 * @see http://www.ietf.org/internet-drafts/draft-megacz-xmc-01.txt
 */

class RpcServerImp;

class RpcServer : public QObject {
    Q_OBJECT

public:
    using Port = unsigned int;
    explicit RpcServer(RpcServerImp* imp, QObject* parent = nullptr);

    ~RpcServer();
    virtual bool start(Port port = 0, const QHostAddress &address = QHostAddress::Any) final;
    virtual void close() final;
    Port serverPort();
    QString serverName();

protected slots:
    void serveNewConnection();

protected:
    bool listen(Port port, const QHostAddress &address = QHostAddress::Any);

private:
    RpcServerImp* server_imp;
};

#endif
