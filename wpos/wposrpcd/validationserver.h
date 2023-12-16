#include "tcprpcserver.h"

/**
 * An example XRServer which implements the validation spec.
 * @see http://www.xmlrpc.com/validator1Docs
 */

class XRValidationServer : public TcpRpcServer {
    Q_OBJECT
public:
    explicit XRValidationServer(
        PayloadFormater& formater,
        Port port = TcpRpcServer::SERVER_DEFAULT_PORT,
        QObject* parent = nullptr);
    
public slots:
    void arrayOfStructsTest(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported);
    
    void countTheEntities(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported);

    void easyStructTest(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported);

    void echoStructTest(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported);

    void manyTypesTest(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported);

    void moderateSizeArrayCheck(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported);

    void nestedStructTest(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported);

    void simpleStructReturnTest(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported);

    void sumAndDifference(
        int req,
        const QString& methodname,
        const QList<QVariant>& params,
        bool& is_method_supported);
};
