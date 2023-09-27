#ifndef RPCSOCKET_H
#define RPCSOCKET_H

#include <QObject>
#include <QHostAddress>

class QTcpSocket;
class QWebSocket;

class AbstractSocket : public QObject{
    Q_OBJECT

public:
    virtual QHostAddress peerAddress() const =0;
    virtual quint16 peerPort() const =0;
    virtual QAbstractSocket::SocketState state() const = 0;
    virtual void close() =0;
    virtual qint64 bytesAvailable() const =0;
    virtual bool canReadLine() const =0;
    virtual QByteArray readLine(qint64 maxlen = 0) =0;
    virtual QByteArray readAll() =0;
    virtual inline qint64 write(const QByteArray &data) =0;

protected:
    virtual void bindSocket() = 0;

signals:
    void readyRead();
    void aboutToClose();
    void stateChanged(QAbstractSocket::SocketState);

protected:
    explicit AbstractSocket(QObject * parent = nullptr);

};

class RpcTcpSocket : public AbstractSocket {
    Q_OBJECT

public:
    explicit RpcTcpSocket(QObject * parent);
    RpcTcpSocket(QTcpSocket* socket, QObject * parent) ;

    QHostAddress peerAddress() const override;
    quint16 peerPort() const override;
    QAbstractSocket::SocketState state() const override;
    void close();
    qint64 bytesAvailable() const override;
    bool canReadLine() const override;
    QByteArray readLine(qint64 maxlen = 0) override;
    QByteArray readAll() override;
    qint64 write(const QByteArray &data) override;

protected:
    void bindSocket() override;

private:
    QTcpSocket* socket;
};

class RpcWebSocket : public AbstractSocket {
    Q_OBJECT

public:
    explicit RpcWebSocket(QObject* parent);
    RpcWebSocket(QWebSocket* socket, QObject* parent);

    QHostAddress peerAddress() const override;
    quint16 peerPort() const override;
    QAbstractSocket::SocketState state() const override;
    void close() override;
    qint64 bytesAvailable() const override;
    bool canReadLine() const override;
    QByteArray readLine(qint64 maxlen = 0) override;
    QByteArray readAll() override;
    qint64 write(const QByteArray &data) override;

protected:
    void bindSocket() override;

private:
    QWebSocket* socket;
};

#endif // RPCSOCKET_H
