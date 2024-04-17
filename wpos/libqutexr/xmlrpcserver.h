
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

#ifndef H_XRSERVER
#define H_XRSERVER

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

class XmlRpcServer : public QObject {
    //Macro for QObject subclasses:
    Q_OBJECT

public:

    static const QString SERVER_NAME;
    static const unsigned int SERVER_DEFAULT_PORT;
    /**
     * Create an xml-rpc server on given port.
     * @param port the port to bind to
     */
    explicit XmlRpcServer(quint16 port = XmlRpcServer::SERVER_DEFAULT_PORT, QObject* parent = nullptr);
    /**
     * Delete (which closes) all the sockets
     * immediately.  Does not do any graceful closing of
     * the HTTP connections.
     */
    ~XmlRpcServer();

    /**
    Start the server and listen to a specific port
    */
    bool start();

    /**
     * Lightweight authentication function.
     * Useful if you only want certain hosts to be
     * able to call a method.  Returns 0.0.0.0 if
     * the request is not an active request.
     * @param req a request number
     * @return the host address for the peer that made the request.
     */
    QHostAddress getPeerForRequest(unsigned int req_id) const;

signals:
    /**
     * This signal is emitted when a new method call
     * comes in.  Handlers should connect to this
     * signal.  Qt's signal and slot mechanism will make
     * sure it gets to all handlers.  Each handler
     * is responsible for checking the method name and
     * seeing if this is a request they should handle.
     * If they do, they set handled to true.
     *
     * @param req This is an identifier for the call
     * @param handled This is initialized as false, and
     * when a slot handles this methodcall, it should be
     * set to true.
     * @see sendMethodResponse
     * @see sendFault
     */
    void methodCall(int req,
                    const QString& methodname,
                    const QList<QVariant>& params,
                    bool& handled);
public slots:
    /**
     * When a methodCall is handled with a successful
     * (non-fault) result, this slot is used to send the
     * response back to the caller.  Either this, or
     * XRServer::sendFault may be called, but not both.
     *
     *
     * @param req the request this is a response for
     * @param result the result of the methodcall.  If
     * the result cannot be converted to a XML-RPC type
     * (using XRVariant) an application fault is sent
     * (faultCode -32500).
     * @see methodCall
     * @see sendHttpResponse
     * @see XRVariant
     */
    void sendMethodResponse(int req, const QVariant& result);

    /**
     * When a methodCall is handled with an unsuccessful
     * (fault) result, this slot is used to send the
     * fault back to the caller.  Either this, or
     * XRServer::sendMethodResponse may be called, but not both.
     *
     * @param req the request this is a fault for
     * @param fault_code the number of the fault that
     * occured.
     * @param fault_string the string representation of
     * the fault
     * @see http://xmlrpc-epi.sourceforge.net/specs/rfc.fault_codes.php
     * @see sendHttpResponse
     */
    void sendFault(int req_id,
                    int fault_code,
                    const QString& fault_string);

protected slots:
    /**
     * The constructor connects this slot to QTcpSocket::readyRead()
     */
    void readFromClient();

    /**
     * The constructor connects this slot to QTcpSocket::delayedCloseFinished()
     */
    void deleteClient();
    void serveNewConnection();
    void socketStateChanged(QAbstractSocket::SocketState state);

protected:
    /**
     * @param socket, the socket to parse the XML in and then
     * emit a signal to handle the request
     */
    void parseContentAndEmit(QTcpSocket* socket);

    /**
     * Convienience function to handle the HTTP
     * transport.
     */
    void sendHttpError(QTcpSocket* s,
                       int status_code,
                       const QString& reason,
                       QString resp);

    /**
     * Convienience function to handle the HTTP
     * transport.
     * Used to send responses and faults
     * @see sendMethodResponse
     * @see sendFault
     */
    void sendHttpResponse(QTcpSocket* s,
                          int status_code,
                          const QString& reason,
                          QNetworkRequest& request,
                          QString resp);

    /**
     * Until we read all the headers,
     * each time we can read data, we check to see if we
     * can read an entire line.  If we can, we append to
     * header strings recieved so far.  When we get an
     * empty line (which in HTTP means the headers are
     * over) we create an HttpRequestHeader and add it
     * to _headers
     */
private:
    QString clientAddress(const QTcpSocket& client);

private:

    QTcpServer* xml_rpc_server{};
    /**
     * Stores the complete headers for the current
     * request from a socket.  If there is no current
     * request, this map will have no entry for the
     * socket.  In this way, it also represents a set of
     * sockets that have active requests.
     */
    //    QMap<QTcpSocket*, QHttpRequestHeader> _headers;
    QMap<QTcpSocket*, QMap<QString, QVariant>> m_headers;

    /**
     * Since QT does not have a Set object, and we don't
     * want to introduce an STL dependence for one
     * class, we hack QMap to be a set of open sockets.
     * When the destructor is called, all these sockets
     * are deleted.
     */
    QList<QTcpSocket*> m_open_sockets;
    QMap<int, QTcpSocket*> m_req_to_socket; //Keeps track of requests for each socket.

    int m_last_req_id{0}; //Counter that increases with each request.
    quint16 m_port;

};

#endif
