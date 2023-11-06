/***************************************************************************
                          bsladdoptionwidget.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "productoptioncreationwidget.h"

#include "menusystem/utils.h"

#include "database/productoptionsmoduledb.h"

#include <wposcore/config.h>
#include <wposwidget/floatkeyboardbox.h>
#include <libbslxml/xmlconfig.h>

#include <QLineEdit>
#include <QString>
#include <QStringList>
#include <QColor>
#include <QCheckBox>
#include <QTextEdit>
#include <QLayout>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QListWidget>
#include <QMatrix>
#include <QPixmap>
#include <QDir>
#include <QLabel>
#include <QLocale>

#include <iostream>
using namespace std;

static const QString SELECT_OPTION_TYPE = QObject::tr("Select a type of option");
static const QString SELECT_OPTION = QObject::tr("Select an option");
static const QString& OPTIONS_LIST_DTD = "/etc/ntp_backoffice/dtds/products_optionslist.dtd";

static const QString& OPTION_ICON_PATH = Files::Controls32Dir;
static const double ICON_SIZE = 30.00;

static const QString& XML_DESCRIPTION {"/etc/ntp_backoffice/mainscreen_description.xml"};

ProductOptionCreationWidget::ProductOptionCreationWidget(
        double _product_price,
        QWidget *parent,
        const QString& name):
    QWidget(parent), product_price { _product_price}
{
    setupUi(this);
    setObjectName(name);

    logo->hide();
    logo_label->hide();

    QHBoxLayout *layout{};
    float_keyboard_option = new FloatKeyboardBox(numblock_option_frame);
    float_keyboard_option->setObjectName( "float_keyboard_option");

    if( !( layout = qobject_cast<QHBoxLayout *>(numblock_option_frame->layout()) ))
        layout = new QHBoxLayout(numblock_option_frame);
    layout->addWidget(float_keyboard_option);

    connect(save_option_button, SIGNAL(clicked()), this, SLOT(saveOptionButtonClicked()));
    connect(option_type_combo, SIGNAL(activated(const QString&)), this, SLOT(optionTypeActivated(const QString&)));
    initLogos();
}

void ProductOptionCreationWidget::optionTypeActivated(const QString& text){

    if( text == SELECT_OPTION_TYPE){
        option_combo->clear();
        option_combo->addItem(SELECT_OPTION_TYPE);
    }
    else updateOptions();
}

void ProductOptionCreationWidget::saveOptionButtonClicked(){
    ProductOptionData option;
    option.option_type = option_type_combo->currentText();
    option.description_type = description_option_type->text();
    option.option_name = option_combo->currentText();
    option.description_option = description_option->text();
    option.is_default = option_default->isChecked();
    option.value = float_keyboard_option->getNumber() - product_price;

    if( option_default->isChecked() ) clearDefaults();
    options.append(option);
    updateOptions();
    option_default->setChecked(false);
    float_keyboard_option->clear();
}

void ProductOptionCreationWidget::updateOptions(){

    //read all options of this option's type
    QString xml_string = model.getOptions(option_type_combo->currentText());
    //read the description of this option's type
    QString description = model.getDescriptionOptionType(option_type_combo->currentText());

    XmlConfig xml;
    if(!xml.readXmlFromString(xml_string)){
        cerr << "cannot convert xml into string " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(OPTIONS_LIST_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    xml.delDomain();
    xml.setDomain("options[0]");

    QStringList tmp_options;
    for(auto i = 0; i < xml.howManyTags("option"); i++)
        tmp_options.append(xml.readString("option[" + QString::number(i) + "].option_name"));

    option_combo->clear();
    option_combo->addItem(SELECT_OPTION);
    option_combo->addItems(tmp_options);
    description_option_type->setText(description);
}

void ProductOptionCreationWidget::setProductPrice(double _product_price){
    product_price = _product_price;
}

void ProductOptionCreationWidget::initOptionTypes(){

    QString xml_string = model.getOptionTypes();
    XmlConfig xml;
    if(!xml.readXmlFromString(xml_string)){
        cerr << "can not convert string into xml " << __PRETTY_FUNCTION__ << ": " <<__LINE__ << endl;
        return;
    }

    if(!xml.validateXmlWithDTD(OPTIONS_LIST_DTD, true)){
        cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return;
    }

    QStringList option_types;
    xml.setDomain("options");
    for(auto i = 0; i < xml.howManyTags("option"); i++)
        option_types.append(xml.readString("option[" + QString::number(i) + "].option_type"));

    option_type_combo->clear();
    option_type_combo->addItem(QString(SELECT_OPTION_TYPE));
    option_type_combo->addItems(option_types);
    option_combo->clear();
    option_combo->addItem(QString(SELECT_OPTION_TYPE));
}

void ProductOptionCreationWidget::clearDefaults(){
    if( options.isEmpty() )  return;
    for(auto& option : options)
        option.is_default = false;
}

bool ProductOptionCreationWidget::saveOptionsProduct(const QString& product_code){

    if( options.isEmpty() ) return true;

    XmlConfig xml;
    xml.delDomain();
    xml.createElement("product_code", product_code);
    xml.createElementSetDomain("options");

    for(const auto& option : options){
        xml.createElementSetDomain("option");
        xml.createElement("option_type", option.option_type);
        xml.createElement("description_type", option.description_type);
        xml.createElement("option_name", option.option_name);
        xml.createElement("description_option", option.description_option);
        xml.createElement("value", QString::number(option.value));

        if( option.is_default ) xml.createElement("default", "t");
        else xml.createElement("default", "f");

        xml.releaseDomain("option");
    }

    xml.releaseDomain("options");
    if(!xml.validateXmlWithDTD(OPTIONS_LIST_DTD, true)){
        cerr << "can not convert xml into string " << __PRETTY_FUNCTION__  << ": " << __LINE__ << endl;
        xml.debug();
        return false;
    }

    return model.insertOptionsToProduct(xml.toString());
}

void ProductOptionCreationWidget::clear(){
    option_type_combo->clear();
    description_option_type->clear();
    option_combo->clear();
    description_option->clear();
    option_default->setChecked(false);
    options.clear();
}

void ProductOptionCreationWidget::setEditable(bool editable){
    option_type_combo->setEditable(editable);
    option_combo->setEditable(editable);

    if( !editable ){
        logo_label->hide();;
        logo->hide();
    }
}

void ProductOptionCreationWidget::saveOptions(){

    if(options.isEmpty()) return;

    for(const auto& option : options){
        XmlConfig xml;
        xml.createElementSetDomain("options");
        xml.createElementSetDomain("option");
        xml.createElement("option_type", option.option_type);
        xml.createElement("description_type", option.description_type);
        xml.createElement("option_name", option.option_name);
        xml.createElement("description_option", option.description_option);
        xml.createElement("value", QString::number(option.value));

        if( option.is_default ) xml.createElement("default", "t");
        else xml.createElement("default", "f");

        xml.releaseDomain("option", false);
        xml.releaseDomain("options", false);

        if( !xml.validateXmlWithDTD(OPTIONS_LIST_DTD) ){
            cerr << "xml does not validate against dtd " << __PRETTY_FUNCTION__ << ": " << __LINE__ << endl;
            return;
        }

        model.insertOption(xml.toString());
    }
}

void ProductOptionCreationWidget::initLogos(){
    QString absolute_file_path;
    logo->clear();

    QStringList files = QDir(OPTION_ICON_PATH).entryList(QStringList("*.png"), QDir::Files, QDir::Name);
    for(const auto& file : files){
        absolute_file_path = OPTION_ICON_PATH + file;
        auto item = new QListWidgetItem(cropedIcon(absolute_file_path, ICON_SIZE) , file,  logo);
        logo->addItem(item);
    }
}

const QList<ProductOptionData>& ProductOptionCreationWidget::getOptions() const{
    return options;
}

void ProductOptionCreationWidget::checkDefaultOptions(){

    int i;
    for(i = 0; i < options.size(); i++)
        if( options[i].is_default) break;

    if( i == options.size() ) options[0].is_default = true;
}



