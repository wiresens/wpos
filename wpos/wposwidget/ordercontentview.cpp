/***************************************************************************
                          bslorderwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Jose Ignacio Ferrero-Carrera
Modified by Carlos Manzanedo Rueda.

%LICENCIA%
 ***************************************************************************/

#include "dbusprinter_interface.h"

#include "ordercontentview.h"
#include "visualproduct.h"
#include "toolkit.h"
#include "global.h"
#include "iwidgetconfighelper.h"

#include <wposcore/genericsignalmanager.h>
#include <libbslxml/xmlconfig.h>

#include <QDataStream>
#include <QLayout>
#include <QDateTime>
#include <QTimer>
#include <QStringList>
#include <QHeaderView>
#include <iostream>

using namespace std;

using IDBusReceiptPrinter = com::wiresens::wpos::dbusprinter::DBusReceiptPrinter;
static const QString dbusService {"com.wiresens.wpos.dbusprinter"};
static const QString dbusObject  {"/wpos/dbusprinter/DBusReceiptPrinter"};

OrderContentView::OrderContentView(
    XmlConfig &contentXmlDesc,
    QWidget *parent,
    QString name) :
    QTableWidget(parent),
    products  {new HList<VisualProduct>}
{
    setObjectName(name);
    auto gsm = GenericSignalManager::instance();
    gsm->subscribeToGenericSignal(GSIGNAL::WRONG_PRODUCT, this);
    gsm->subscribeToGenericDataSignal(GDATASIGNAL::SETCORE_MODE, this);

    // Order has 0 rows at start time
    setRowCount(0);
    setAlternatingRowColors(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    // Default profile for order is obtained from the description XML.
    // <PART 2> extracting values from XML
    parseXmlDescription(contentXmlDesc);
}

OrderContentView::~OrderContentView(){
    delete products;
}

void OrderContentView::printTicket(){
    auto bus = QDBusConnection::sessionBus();
    IDBusReceiptPrinter *iface;
    iface = new IDBusReceiptPrinter(dbusService, dbusObject, bus, this);
    if (!iface->isValid()){
        qDebug() << "DBus Service :" << dbusService << "for object :" << dbusObject
                 << "Requested by OrderContentView::printTicket()"
                 << "From DBusReceiptPrinter::printTicket()"
                 << "Not Available";
        return;
    }

    iface->printTicket(local_products_xml->toString(), 1);
}

void OrderContentView::parseXmlDescription(XmlConfig &contentXmlDesc){
    IWidgetConfigHelper widgetConfigurer;

    contentXmlDesc.delDomain();
    contentXmlDesc.setDomain("tabledescription.global");
    widgetConfigurer.setBackgroundColor(*this, QColor(contentXmlDesc.readString("backgroundcolor")));
    row_height = contentXmlDesc.readInt("rowheight");

    auto aux_string = contentXmlDesc.readString("showgrid").toLower();
    setShowGrid( aux_string == "true");

    auto showhorheader = contentXmlDesc.readString("showhorheader");
    auto showverheader = contentXmlDesc.readString("showverheader");
    horizontalHeader()->setVisible(showhorheader == "true");
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setVisible(showverheader == "true");

    //Set widget size
    auto height = contentXmlDesc.readString("height");
    auto width = contentXmlDesc.readString("width");
    widgetConfigurer.setSize(*this, width.toInt(), height.toInt());

    auto hsizepolicy = contentXmlDesc.readString("hsizepolicy");
    auto vsizepolicy = contentXmlDesc.readString("vsizepolicy");
    widgetConfigurer.setSizePolicy(*this, hsizepolicy, vsizepolicy);

    widgetConfigurer.setFrameShape(*this, contentXmlDesc.readString("frameshape"));
    widgetConfigurer.setFrameShadow(*this, contentXmlDesc.readString("frameshadow"));

    // Sets the selection mode to the value readed
    aux_string = contentXmlDesc.readString("selectionmode");
    widgetConfigurer.setSelectionMode(*this, aux_string);

    // Do you want an acumulative sum for each row?
    aux_string = contentXmlDesc.readString("usepartialsums");
    use_partial_sums = ( aux_string == "true");

    auto hscroll = contentXmlDesc.readString("hscroll");
    auto vscroll = contentXmlDesc.readString("vscroll");
    widgetConfigurer.setHorizontalScrollBarPolicy(*this, hscroll);
    widgetConfigurer.setVerticalScrollBarPolicy(*this, vscroll);

    setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
    setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

    widgetConfigurer.setFont(*this, contentXmlDesc);

    // Parse and create the column section
    contentXmlDesc.delDomain();
    contentXmlDesc.setDomain("tabledescription.columns");

    setColumnCount(contentXmlDesc.howManyTags("column"));
    QStringList headerLabels;
    for( auto i=0;  i < columnCount(); i++){
        auto name = contentXmlDesc.readString("column["+QString::number(i)+"].name");
        auto label = contentXmlDesc.readString("column["+QString::number(i)+"].text");
        auto pos = contentXmlDesc.readString("column["+QString::number(i)+"].position");

        bool ok{false};
        int aux_num = pos.toInt(&ok);
        if (  !name.isEmpty()  && ok ){
            auto column_pos = aux_num;
            columnPositions[name] = column_pos;

            //We just collect the labels here and set the header at the end of the loop
            headerLabels << label ;
            aux_string = contentXmlDesc.readString("column["+QString::number(i)+"].width");

            aux_num = aux_string.toInt(&ok);
            if (ok) setColumnWidth(column_pos, aux_num);

            auto hidden = contentXmlDesc.readString("column["+QString::number(i)+"].show");
            QTableWidget::setColumnHidden(column_pos, hidden == "false");
        }
    }
    setHorizontalHeaderLabels(headerLabels);
}

void OrderContentView::setDefaultColor(){
    QPalette palette;
    switch (color_state){
    case 0:
        palette.setColor(QPalette::Window, QColor(Colors::ORDER_WARNING_COLOR));
        setPalette(palette);
        color_state++;
        QTimer::singleShot(200,this, &OrderContentView::setDefaultColor);
        break;

    case 1:
        palette.setColor(QPalette::Window, QColor(Colors::ORDER_NORMAL_COLOR));
        setPalette(palette);
        color_state++;
        QTimer::singleShot(100, this, &OrderContentView::setDefaultColor);
        break;

    case 2:
        palette.setColor(QPalette::Window, QColor(Colors::ORDER_WARNING_COLOR));
        setPalette(palette);
        color_state++;
        QTimer::singleShot(200,this, &OrderContentView::setDefaultColor);
        break;

    default:
        palette.setColor(QPalette::Window, QColor(last_color));
        setPalette(palette);
        break;
    }
}

void OrderContentView::genericSignalSlot(const QString& signal){
    if (signal == GSIGNAL::WRONG_PRODUCT){
        color_state = 0;
        last_color = palette().window().color();
        update();
        QTimer::singleShot(50, this, &OrderContentView::setDefaultColor);
    }
}

void OrderContentView::genericDataSignalSlot(const QString& signal, XmlConfig *xml){
    QString aux;
    if (signal == GDATASIGNAL::SETCORE_MODE){
        xml->delDomain();
        aux = (xml->readString("mode")).toLower();
        QPalette palet;
        if ( aux == "normal"){
            palet.setColor(backgroundRole(), QColor(Colors::ORDER_NORMAL_COLOR));
            setPalette(palet);
        }
        else if (aux == "product_anulation"){
            palet.setColor(backgroundRole(), QColor(Colors::ORDER_ANULATION_COLOR));
            setPalette(palet);
        }
        else if (aux == "default_offer"){
            palet.setColor(backgroundRole(), QColor(Colors::ORDER_DEFAULT_COLOR));
            setPalette(palet);
        }
    }
}

void OrderContentView::usePartialSumsByRow(bool use_it){
    use_partial_sums = use_it;
}

void OrderContentView::showColumn(const QString& name, bool visible){
    setColumnHidden(columnPositions[name], !visible);
}

void OrderContentView::selectUp(){
    auto selected_row = currentRow();
    if( !selected_row) return;

    clearSelection();
    auto selection = QTableWidgetSelectionRange(selected_row - 1, 0, selected_row - 1, columnCount());
    setRangeSelected(selection, true);
    setCurrentCell(selected_row - 1, 0);
}

void OrderContentView::selectFirst(){
    clearSelection();
    auto selected_row = 0;
    auto selection = QTableWidgetSelectionRange(selected_row, 0, selected_row, columnCount());
    setRangeSelected(selection, true);
    setCurrentCell(selected_row, 0);
}

void OrderContentView::selectDown(){
    auto selected_row = currentRow();
    if(selected_row == rowCount() - 1 )  return;

    clearSelection();
    auto selection =  QTableWidgetSelectionRange(selected_row + 1, 0, selected_row + 1, columnCount());
    setRangeSelected(selection, true);
    setCurrentCell(selected_row + 1, 0);
}

void OrderContentView::selectLast(){
    clearSelection();
     auto selected_row = rowCount();
     auto selection =  QTableWidgetSelectionRange(selected_row - 1, 0, selected_row - 1, columnCount());
    setRangeSelected(selection, true);
    setCurrentCell(selected_row - 1, 0);
}

void OrderContentView::clearOrder(){
    for(int i = 0; i < rowCount(); i++) removeRow(i);
    setRowCount(products->count());
    for(int i = 0; i < rowCount(); i++) setRowHeight(i, row_height);
}

void OrderContentView::reset(){
    setRowCount(0);

    local_products_xml->pushDomain();
    local_products_xml->delDomain();
    local_products_xml->deleteElement("description");
    local_products_xml->deleteElement("products");
    local_products_xml->createElement("products");
    local_products_xml->popDomain();
    buildProductsList();
    fillVisualOrderWithProductsList();

    emit contentChanged(local_products_xml);
}

void OrderContentView::delRow(){
    int last_row;
    QString key;
    VisualProduct *vproduct = 0;
    // This is the current selection in table
    auto selected_row = currentRow();
    if (selected_row == -1)
        return;
    last_row = selected_row;
    vproduct = products->at(selected_row);

    local_products_xml->pushDomain();
    key = vproduct->name + vproduct->options + QString::number(vproduct->price,'f',2);
    while ((vproduct = products->find(key))){
        local_products_xml->delDomain();
        local_products_xml->deleteElement("products.product["+QString::number(vproduct->xmlAnchor)+"]");
        buildProductsList();
    }
    fillVisualOrderWithProductsList();
    local_products_xml->popDomain();

    if (last_row > rowCount())
        last_row--;
    selectRow(last_row);
    selected_row = last_row;
    emit contentChanged(local_products_xml);
}

void OrderContentView::incrementQuantity(){
    int i,count;
    int last_row;
    float quantity;
    QTableWidgetItem *item{};
    VisualProduct *vproduct{};

    // This is the current selection in table
    auto selected_row = currentRow();
    last_row = selected_row;

    // Take quantity of this pruduct
    int column_pos = columnPositions["units"];
    item = QTableWidget::item(selected_row, column_pos);

    if (!item) return;

    // Update quantity of this product in 1 unit more in the visual order
    quantity = item->text().toFloat();
    quantity++;
    item->setText(QString::number((int)quantity));

    update(model()->index(selected_row, column_pos));

    // Take in count if there is partial sums by row
    column_pos = columnPositions["price"];
    if ( use_partial_sums && column_pos != -1){
        vproduct = products->at(selected_row);
        item = QTableWidget::item(selected_row,column_pos);
        item->setText( QString::number((int)quantity* vproduct->price,'f',2));
        update(model()->index(selected_row, column_pos));
    }
    total();

    vproduct = products->at(selected_row);
    local_products_xml->pushDomain();
    local_products_xml->delDomain();

    XmlConfig xml;
    xml.copy(local_products_xml, "products.product["+QString::number(vproduct->xmlAnchor)+"]", "product");
    //delete the printer true sections

    xml.setDomain("product");
    if (xml.howManyTags("printer"))
        for( auto i=0; i < xml.howManyTags("printer"); i++)
            xml.deleteAttribute("printer", "printed");

    xml.releaseDomain("product");

    local_products_xml->copy( &xml, "product", "products.product");
    setTimeStamp("products.product["+QString::number(vproduct->xmlAnchor)+"]");

    local_products_xml->popDomain();

    buildProductsList();
    //        fillVisualOrderWithProductsList();
    selectRow(last_row);
    selected_row = last_row;

    emit contentChanged(local_products_xml);
}

void OrderContentView::decrementQuantity(){
    int last_row;
    int column_pos;
    float quantity;
    QTableWidgetItem *item{};
    VisualProduct *vproduct = 0;


    // This is the current selection in table
     auto selected_row = currentRow();
    last_row = selected_row;

    // Take quantity of this pruduct
    column_pos = columnPositions["units"];
    if ( column_pos != -1 )
        item = QTableWidget::item(selected_row,column_pos);

    if (!item)  return;

    // Update quantity of this product in 1 unit less in the visual order
    quantity = item->text().toFloat();
    quantity--;

    // Check quantity to decide if we must delete the row 'cos it reach 0
    if ((int)quantity){
        item->setText(QString::number((int)quantity));
        //        updateCell(selected_row,column_pos);
        update(model()->index(selected_row, column_pos));

        // Take in count if there is partial sums by row
        if(use_partial_sums){
            column_pos =  columnPositions["price"];
            if (column_pos != -1){
                vproduct = products->at(selected_row);
                item = QTableWidget::item(selected_row,column_pos);
                item->setText(QString::number((int)quantity*vproduct->price,'f',2));
                update(model()->index(selected_row, column_pos));
            }
        }
    }
    else {
        removeRow(currentRow());
        setCurrentCell(currentRow(),currentColumn());
    }
    total();
    vproduct = products->at(selected_row);
    local_products_xml->pushDomain();
    local_products_xml->delDomain();
    local_products_xml->deleteElement("products.product["+QString::number(vproduct->xmlAnchor)+"]");
    local_products_xml->popDomain();

    buildProductsList();
    //          fillVisualOrderWithProductsList();
    if ( rowCount()< last_row)
        last_row = rowCount()-1;
    selectRow(last_row);

    emit contentChanged(local_products_xml);
}

/**********************************************************************************************************************
 * This slot is used to calculate the total amount of bill, based only in the visual elements
 * displayed in the visual order
 **********************************************************************************************************************/
void OrderContentView::total(){
    float quantity = 0;

    float price = 0;
    float amount = 0;
    int column_pos = 0 ;

    for(int i = 0; i < rowCount(); i++){
        //take the units
        column_pos = columnPositions["units"];
        if(column_pos != -1)
            quantity = model()->data(model()->index(i, column_pos)).toFloat();
        else
            quantity = 0.0;

        // Take price
        column_pos = columnPositions["price"];
        if(column_pos != -1)
            price =  model()->data(model()->index(i, column_pos)).toFloat();
        else
            //           quantity = 0.0;
            price = 0.0;
        // Check if we are using partial sums in rows and take it in count
        if (!use_partial_sums)
            amount += (quantity*price);
        else
            amount += price;
    }
    emit totalChanged(amount);
}

QString OrderContentView::readOptions(QString options_domain){
    QString options = "";
    QString type = "";
    QString value = "";
    int options_number,i;

    local_products_xml->pushDomain();
    local_products_xml->setDomain(options_domain);

    options_number = local_products_xml->howManyTags("option");
    for(i=0; i<options_number; i++){
        type = local_products_xml->readString("option["+QString::number(i)+"].type");
        value = local_products_xml->readString("option["+QString::number(i)+"].value");
        options += "<"+type+">"+value + ",";
    }

    options = options.mid(0,options.length()-1);
    local_products_xml->releaseDomain(options_domain);
    local_products_xml->popDomain();
    return options.simplified();
}

/**********************************************************************************************************************
 * This slot is used to set the order timestamp in the XML of products
 **********************************************************************************************************************/
void OrderContentView::setTimeStamp(const QString &domain){
//    auto  timeStamp = QDateTime::currentDateTime().toString("yyyyMMddThh:mm:ss.zzz");
//    auto timeStamp =  QDate::currentDate().toString("dd/MM/yyyy") + " " + QTime::currentTime().toString("hh:mm:ss");
    auto timeStamp = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
    local_products_xml->pushDomain();
    local_products_xml->delDomain();
    local_products_xml->doWrite(domain+".timestamps.ordertime", timeStamp);
    local_products_xml->popDomain();
}

/**********************************************************************************************************************
 * This method selects the last item in visual order. It should be called each time we clear the
 * order and add new elements
 **********************************************************************************************************************/
void OrderContentView::selectLastOrderItem(){
    auto selected_row = rowCount();
    if (!selected_row) return;

    clearSelection();
    auto selection =  QTableWidgetSelectionRange(selected_row, 0, selected_row, columnCount());
    setRangeSelected(selection, true);
    setCurrentCell(selected_row, 0);
}

/***********************************************************************************************************************
 * Clear dictionary and list of products. Parse the xml sended by core module and build a new
 * one dictionary and list of products. The products are those ones called VisualProduct which
 * have among other things, a name, quantity, price and xmlAnchor attributes.
 ***********************************************************************************************************************/
void OrderContentView::buildProductsList(){
    int i;
    int productsNumber = 0;
    QString aux = "";
    QString key = "";
    VisualProduct *vproduct_finded = 0;
    VisualProduct *vproduct = 0;

    // Clear products list each time this method is called
    products->clear();
    local_products_xml->delDomain();
    local_products_xml->setDomain("products");

    // Looks for products in XML form
    productsNumber = local_products_xml->howManyTags("product");
    for(i=0; i<productsNumber; i++){
        // and create its visual representation
        vproduct = new VisualProduct();
        aux = local_products_xml->readString("product["+QString::number(i)+"].name");
        vproduct->name =aux;
        aux = local_products_xml->readString("product["+QString::number(i)+"].billing.price");
        vproduct->price = aux.toFloat();
        aux = readOptions("product["+QString::number(i)+"].options");
        vproduct->options =aux;
        vproduct->quantity = 1.0;
        vproduct->xmlAnchor = i;

        key = vproduct->name + vproduct->options + QString::number(vproduct->price,'f',2);
        vproduct_finded = products->find(key);
        if (!vproduct_finded)
            products->append(vproduct, key);
        else {
            vproduct_finded->quantity = vproduct_finded->quantity+1;
            vproduct_finded->xmlAnchor = i;
        }
    }
}

void OrderContentView::fillVisualOrderWithProductsList(){
    int i, count,j,o_count;
    int column_pos = 0;
    QString options = "";
    QString aux;
    QStringList list;
    QString line = "";
    QTableWidgetItem *description_item = 0;
    VisualProduct *vproduct = 0;

    // Clear visual order
    clearOrder();

    // Add the products given in the XML
    count = products->count();
    for(i=0; i<count; ++i){
        vproduct = products->at(i);

        // Prepare options, if there are someone add it to description of product
        list = vproduct->options.split(",");
        options = "";
        o_count = list.count();
        for (j=0;j<o_count;j++){
            options+= list[j];
            options.replace(QRegExp("<.*>"),"");
            options.replace('_'," ");
            if (j!=o_count-1)
                options+=", ";
        }
        list.clear();
        if(!options.isEmpty())
            line = vproduct->name + " ("+ options +")";
        else
            line = vproduct->name;

        // Create the visual element to be added to order
        description_item = new QTableWidgetItem(line);
        //        description_item->setWordWrap(true);

        // Check for units column and update it
        //int *aux_int = (column_positions_list->find("units"));
        column_pos = columnPositions["units"];
        if(column_pos != -1)
            setItem(i, column_pos, new QTableWidgetItem(QString::number(vproduct->quantity)));

        // Check for description column and update it
        column_pos = columnPositions["description"];
        if(column_pos != -1)
            setItem(i, column_pos, description_item);

        // Check for price column and update
        column_pos = columnPositions["price"];
        if (column_pos != -1){
            if(use_partial_sums)
                setItem(i,column_pos,new QTableWidgetItem(QString::number((vproduct->quantity * vproduct->price),'f',2)));
            else
                setItem(i,column_pos,new QTableWidgetItem(QString::number(vproduct->price,'f',2)));
        }
    }

    // Select last item and ensure this is visible in the order
    scrollTo( model()->index(rowCount(), 0));
    selectLastOrderItem();

    // Recalculate the amount of the products in this order
    total();
}

void OrderContentView::updateProductsInfo(XmlConfig *xml){
    delete local_products_xml;
    local_products_xml = new XmlConfig();
    local_products_xml->readXmlFromString(xml->toString());

    // Build the products list each time a signal arrived
    buildProductsList();
    fillVisualOrderWithProductsList();
}

void OrderContentView::debugVProductList(){
    VisualProduct *vproduct = 0;
    int count, i;
    count = products->count();
    for(i=0; i<count; i++){
        vproduct = products->at(i);
        cout << "(" << i << ") [" << vproduct->name.toStdString() << "-" << vproduct->quantity;
        cout << "-"<< vproduct->xmlAnchor <<"]  KEY[";
        cout << vproduct->name.toStdString() << vproduct->options.toStdString() << (QString::number(vproduct->price,'f',2)).toStdString() <<"]"<<endl;
    }
}

void OrderContentView::debug(){
    if (local_products_xml)
        local_products_xml->debug();
    debugVProductList();
}

XmlConfig* OrderContentView::orderAsXml() const{
    return local_products_xml;
}

