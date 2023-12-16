#ifndef CLIENTIMPL_H
#define CLIENTIMPL_H
#include "ui_client.h"
#include <libqutexr/xrclient.h>

#include <QMap>
#include <QLCDNumber>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QTabWidget>
#include <QProgressBar>
#include <QTextEdit>

class ClientImpl :
    public QMainWindow,
    private Ui::Client
{
    Q_OBJECT

public:
    ClientImpl( QWidget* parent = 0);

protected slots:
    void callSumDiff();
    void callValidate();

    //  connected to the QTabWidget's currentChanged signal
    void setFunction(int index);

    // Sets the x and y values for the sumAndDifference method
    void setX(const QString& x);
    void setY(const QString& y);

    void responseSlot(int num, const QVariant& response);
    void faultSlot(int num, int fault_num, const QString& string);

private:
    XRClient* client;
    // This map keeps track of which requests are for what function.
    QMap<int, QString> _req_to_method;

    // For validation client.  Holds the result we are expecting
    QMap<int, QVariant> _req_result;
    int _validation_step{0};
    int _x{16}, _y{9};

};

#endif // CLIENTIMPL_H
