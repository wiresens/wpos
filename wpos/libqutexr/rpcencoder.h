#ifndef RPC_ENCODER_H
#define RPC_ENCODER_H

#include "xrmethodcall.h"
#include "xrmethodresponse.h"

#include <QString>
#include <QVariant>
class PayloadFormater
{
public:
    virtual ~PayloadFormater() = default;

    virtual QString responseString() const{ return QString(); }
    virtual QString callString() const{ return QString(); }

    virtual QString contentType() const = 0;
    virtual QString method() const = 0;
    virtual QList<QVariant> params() const = 0;
    virtual bool buildMethodCall(const QString& content, QString& parseErrorMsg, QString& invalidMethodErrorMsg) = 0;
    virtual void buildMethodResponse(const QVariant& result) = 0;
    virtual void buildMethodResponse(int fault_code, const QString&  fault_string) = 0;

protected:
    PayloadFormater() = default;
};

class XmlPayloadFormater : public PayloadFormater{
public:
    ~XmlPayloadFormater() override = default;

    virtual bool buildMethodCall(const QString& content, QString& parseErrorMsg, QString& invalidMethodErrorMsg) override;
    virtual void buildMethodResponse(const QVariant& result) override;
    virtual void buildMethodResponse(int fault_code, const QString&  fault_string) override;

    virtual QString contentType() const override{
        return QString("text/xml");
    }

    virtual QString method() const override{
        return xmlcall.getMethodName();
    }

    virtual QList<QVariant> params() const override{
        return xmlcall.getParams();
    }

    virtual QString responseString() const override{
        return xmlresp.toString();
    }

    virtual QString callString() const override{
        return xmlcall.toString();
    }

private:
    XRMethodCall xmlcall;
    XRMethodResponse xmlresp;
};

#endif // RPC_ENCODER_H
