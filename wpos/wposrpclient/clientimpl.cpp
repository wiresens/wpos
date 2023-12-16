#include "clientimpl.h"

ClientImpl::ClientImpl(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi(this);
    // The server default port is set to 18300
    url_ledit->setText("http://192.168.182.17:18300/RPC2");
//    url_ledit->setText("http://10.137.3.115:18300/RPC2");
    x_ledit->setText( QString::number(_x));
    y_ledit->setText( QString::number(_y));

    client = new XRClient(QUrl(url_ledit->text()), this);
    connect(main_tab, &QTabWidget::currentChanged, this, &ClientImpl::setFunction);
    connect(url_ledit, &QLineEdit::textChanged,
            client, QOverload<const QString&>::of(&XRClient::setUrl));
    connect(x_ledit, &QLineEdit::textChanged, this, &ClientImpl::setX);
    connect(y_ledit, &QLineEdit::textChanged, this, &ClientImpl::setY);

    //We have to handle the responses from the server
    connect(client, &XRClient::methodResponse, this, &ClientImpl::responseSlot);
    connect(client, &XRClient::fault, this, &ClientImpl::faultSlot);
    main_tab->setCurrentWidget(sum_diff_tab);
    emit main_tab->currentChanged(0);
}

void ClientImpl::callSumDiff(){
    client->setUrl(QUrl(url_ledit->text()));

    QList<QVariant> params;
    QString method = "sample.sumAndDifference";
    params.push_back(_x);
    params.push_back(_y);
    auto req = client->call(method, params);
    _req_to_method[req]= method;
}

void ClientImpl::callValidate(){
    client->setUrl(QUrl(url_ledit->text()));

    /* fixup the output display */
    progress_bar->setValue(0);
    pass_tedit->clear();
    failed_tedit->clear();

    int req{0};
    _validation_step = 0;
    QList<QVariant> params;
    QString method;

    //validator1.arrayOfStructsTest
    method = "validator1.arrayOfStructsTest";
    params.clear();
    QMap<QString, QVariant> tmp_map;
    QList<QVariant> tmp_list;
    tmp_map["larry"] = 2;
    tmp_map["curly"] = 3;
    tmp_map["moe"] = 5;
    tmp_list.push_back(tmp_map);

    tmp_map["larry"] = 7;
    tmp_map["curly"] = 11;
    tmp_map["moe"] = 13;
    tmp_list.push_back(tmp_map);

    tmp_map["larry"] = 17;
    tmp_map["curly"] = 19;
    tmp_map["moe"] = 23;
    tmp_list.push_back(tmp_map);

    params.push_back(tmp_list);

    req = client->call( method, params);
    _req_result.insert( req, (3+11+19) );
    _req_to_method[req] = method;

    //validator1.countTheEntities
    method = "validator1.countTheEntities";
    params.clear();
    params.push_back("<<<>>&\'\'\"\"\"");
    req = client->call(method, params);
    tmp_map.clear();
    tmp_map["ctLeftAngleBrackets"] = 3;
    tmp_map["ctRightAngleBrackets"] = 2;
    tmp_map["ctAmpersands"] = 1;
    tmp_map["ctApostrophes"] = 2;
    tmp_map["ctQuotes"] = 3;
    _req_result.insert(req,tmp_map);
    _req_to_method.insert(req,method);

    //validator1.easyStructTest
    method = "validator1.easyStructTest";
    params.clear();
    tmp_map.clear();
    tmp_map["larry"] = 17;
    tmp_map["curly"] = 19;
    tmp_map["moe"] = 23;
    params.push_back(tmp_map);
    req = client->call(method, params);
    _req_result.insert(req,(17+19+23));
    _req_to_method.insert(req, method);

    //validator1.echoStructTest
    method = "validator1.echoStructTest";
    params.clear();
    params.push_back(tmp_map);
    req =  client->call(method,params);
    _req_result.insert(req, tmp_map);
    _req_to_method.insert(req,method);

    //validator1.manyTypesTest
    method = "validator1.manyTypesTest";
    params.clear();
    params.push_back(6);
    params.push_back(QVariant(false));
    params.push_back(QString("test string"));
    params.push_back(0.123456789);
    params.push_back(QDateTime::currentDateTime());
    params.push_back(QByteArray("This is a test of byte array."
                                "Hopefully this should be base64 encoded. "
                                "blah blah blah blah blah blah blah blah blah"));

    req = client->call(method, params);
    _req_result.insert(req,params);
    _req_to_method.insert(req,method);

    //validator1.moderateSizeArrayCheck
    method = "validator1.moderateSizeArrayCheck";
    params.clear();
    int i=187;
    tmp_list.clear();
    tmp_list.push_back("This is the start");
    for(i = 0; i < 187; i++)
        tmp_list.push_back("blah blah blah blah blah blah blah blah blah");

    tmp_list.push_back(", and this is the end");
    params.push_back(tmp_list);
    req = client->call(method, params);
    _req_result.insert(req,QString("This is the start, and this is the end"));
    _req_to_method.insert(req,method);

    //validator1.nestedStructTest
    method = "validator1.nestedStructTest";
    params.clear();
    tmp_map.clear();
    QMap<QString,QVariant> map2;
    map2["larry"] = 100;
    map2["curly"] = 200;
    map2["moe"] = 400;
    tmp_map["01"] = map2;
    map2.clear();
    map2["04"] = tmp_map;
    tmp_map.clear();
    tmp_map["2000"] = map2;
    map2["larry"] = 12314;
    map2["curly"] = 3842134;
    map2["moe"] = 343;
    tmp_map["junk"] = map2;
    params.push_back(tmp_map);
    req = client->call(method, params);
    _req_result.insert(req,700);
    _req_to_method.insert(req,method);

    //validator1.simpleStructReturnTest
    method = "validator1.simpleStructReturnTest";
    params.clear();
    params.push_back(42);
    req = client->call(method,params);
    tmp_map.clear();
    tmp_map["times10"] = 420;
    tmp_map["times100"] = 4200;
    tmp_map["times1000"] = 42000;
    _req_result.insert(req,tmp_map);
    _req_to_method.insert(req,method);
}

void ClientImpl::faultSlot(int req, int faultNum, const QString& s){
    QMessageBox::warning(0, "Server Faulted", + "Error " + QString::number(faultNum) + " : " + s, QMessageBox::Ok, QMessageBox::NoButton);
    _req_to_method.remove(req);
}

void ClientImpl::setFunction(int index){

    disconnect(call_btn, &QPushButton::clicked, this, &ClientImpl::callSumDiff);
    disconnect(call_btn, &QPushButton::clicked, this, &ClientImpl::callValidate);

    if(index == 0)
        connect(call_btn, &QPushButton::clicked, this, &ClientImpl::callSumDiff);

    else if( index == 1 )
        connect(call_btn, &QPushButton::clicked, this, &ClientImpl::callValidate);
}

void ClientImpl::setX(const QString& x){
    _x = x.toInt();
    x_ledit->setText( QString::number(_x) );
}

void ClientImpl::setY(const QString& y){
    _y = y.toInt();
    y_ledit->setText( QString::number(_y) );
}

void ClientImpl::responseSlot(int req, const QVariant& response){

    //Get the widget that this is a response for.
    QString s = _req_to_method[req];
    if( s == "sample.sumAndDifference" ) {
        QMap<QString, QVariant> resp(response.toMap());
        sum_lcd->display( resp["sum"].toInt() );
        diff_lcd->display( resp["difference"].toInt() );
    }
    else if( s == "validator1.arrayOfStructsTest") {
        if( response == _req_result[req] )
            pass_tedit->append(s + "\n");
        else
            failed_tedit->append(s + "\n");
        _req_result.remove(req);
    }
    else if( s == "validator1.countTheEntities") {

        if( response == _req_result[req] )
            pass_tedit->append(s + "\n");
        else
            failed_tedit->append(s + "\n");
        _req_result.remove(req);
    }
    else if( s == "validator1.easyStructTest") {
        if( response == _req_result[req] )
            pass_tedit->append(s + "\n");
        else
            failed_tedit->append(s + "\n");
        _req_result.remove(req);
    }
    else if( s == "validator1.echoStructTest") {
        auto vmap = response.toMap();
        auto it = vmap.begin();
        bool eq = true;
        QMap<QString,QVariant> tmp_map = _req_result[req].toMap();
        while((it != vmap.end()) && eq) {
            eq = eq && (tmp_map[it.key()] == *it);
            it++;
        }
        if( eq )
            pass_tedit->append(s + "\n");
        else
            failed_tedit->append(s + "\n");
        _req_result.erase(_req_result.find(req));
    }
    else if( s == "validator1.manyTypesTest") {
        if( response == _req_result[req] )
            pass_tedit->append(s + "\n");
        else
            failed_tedit->append(s + "\n");
        _req_result.remove(req);
    }
    else if( s == "validator1.moderateSizeArrayCheck") {
        if( response == _req_result[req] )
            pass_tedit->append(s + "\n");
        else
            failed_tedit->append(s + "\n");
        _req_result.remove(req);
    }
    else if( s == "validator1.nestedStructTest") {
        if( response == _req_result[req] )
            pass_tedit->append(s + "\n");
        else
            failed_tedit->append(s + "\n");
        _req_result.remove(req);
    }
    else if( s == "validator1.simpleStructReturnTest") {
        if( response ==  _req_result[req] )
            pass_tedit->append(s + "\n");
        else
            failed_tedit->append(s + "\n");
        _req_result.remove(req);
    }
    _validation_step++;
    progress_bar->setValue( _validation_step * 100/8 );
//    QThread::usleep(500000);

    //Forget about this request.
    _req_to_method.remove(req);
}
