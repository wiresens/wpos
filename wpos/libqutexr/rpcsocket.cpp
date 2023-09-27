#include "rpcsocket.h"
#include <QTcpSocket>
#include <QtWebSockets/QWebSocket>
#include <QUuid>
#include <iostream>

AbstractSocket::AbstractSocket(QObject *parent) : QObject(parent){
    setObjectName( "AbstractSocket " + QUuid::createUuid().toString());
//    connect(this, &AbstractSocket::destroyed, [&](){ std::cout << "  " << objectName().toStdString() << " Destroyed\n" ;});
//    std::cout << objectName().toStdString() << " Created\n" ;
}

RpcTcpSocket::RpcTcpSocket(QObject * parent):
    AbstractSocket{parent}
{
    socket = new QTcpSocket(this);
    bindSocket();
}

RpcTcpSocket::RpcTcpSocket(QTcpSocket* socket, QObject * parent):
    AbstractSocket{parent}, socket{socket}
{
    socket->setParent(this);
    bindSocket();
}

void RpcTcpSocket::bindSocket(){
    socket->setObjectName("RpcTcpSocket "+ QString::number(socket->peerPort()));
    connect(socket , &QTcpSocket::readyRead, this, &RpcTcpSocket::readyRead );
    connect(socket , &QTcpSocket::aboutToClose, this, &RpcTcpSocket::aboutToClose);
    connect(socket , &QTcpSocket::stateChanged, this, &RpcTcpSocket::stateChanged );
    connect(socket, &AbstractSocket::destroyed, [&](){ std::cout << socket->objectName().toStdString() << " Destroyed\n";});
    std::cout << socket->objectName().toStdString() << " Created\n";
}

QHostAddress RpcTcpSocket::peerAddress() const{
  return socket->peerAddress();
}

quint16 RpcTcpSocket::peerPort() const {
    return socket->peerPort();
}

QAbstractSocket::SocketState RpcTcpSocket::state() const{
    return socket->state();
}

void RpcTcpSocket::close(){
    socket->close();
}

qint64 RpcTcpSocket::bytesAvailable() const {
    return  socket->bytesAvailable();
}

bool RpcTcpSocket::canReadLine() const {
    return  socket->canReadLine();
}

QByteArray RpcTcpSocket::readLine(qint64 maxlen ) {
    return  socket->readLine(maxlen);
}

QByteArray RpcTcpSocket::readAll() {
    return  socket->readAll();
}

qint64 RpcTcpSocket::write(const QByteArray &data) {
    return socket->write(data);
}

RpcWebSocket::RpcWebSocket(QObject * parent)
    :AbstractSocket{parent}
{
    socket = new QWebSocket();
    bindSocket();
}

RpcWebSocket::RpcWebSocket(QWebSocket* socket, QObject * parent):
    AbstractSocket{parent}, socket{socket}
{
    bindSocket();
}

void RpcWebSocket::bindSocket() {
    connect(socket , &QWebSocket::textMessageReceived, this, &RpcWebSocket::readyRead );
    connect(socket , &QWebSocket::aboutToClose, this, &RpcWebSocket::aboutToClose);
    connect(socket , &QWebSocket::stateChanged, this, &RpcWebSocket::stateChanged );
}

QHostAddress RpcWebSocket::peerAddress() const{
  return socket->peerAddress();
}

quint16 RpcWebSocket::peerPort() const {
    return socket->peerPort();
}

QAbstractSocket::SocketState RpcWebSocket::state() const{
    return socket->state();
}

void RpcWebSocket::close(){
    socket->close();
}

qint64 RpcWebSocket::bytesAvailable() const {
//    return socket->bytesAvailable();
}

bool RpcWebSocket::canReadLine() const {
//    return socket->canReadLine();
    return true;
}

QByteArray RpcWebSocket::readLine(qint64 maxlen ) {
//    return socket->readLine(maxlen);
}

QByteArray RpcWebSocket::readAll() {
//    return socket->readAll();
}

qint64 RpcWebSocket::write(const QByteArray &data) {
//    return socket->write(data);
    return socket->sendTextMessage(data);
}
