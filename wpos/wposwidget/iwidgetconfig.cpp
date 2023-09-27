#include "iwidgetconfig.h"
#include "iwidgetconfighelper.h"

#include <libbslxml/xmlconfig.h>
#include <QtWidgets/QWidget>

void IWidgetConfig::setupWidget(QWidget& target, IWidgetConfigHelper &helper, const QString& file){

    XmlConfig xml{ file };
    if ( !QFile::exists(ordersConfig) || !xml.isValid() ){
        cerr << "[CRITICAL ERROR] Can't find " << file.toStdString() << " file. Can't configure widget.";
        return;
    }

    xml->delDomain(); //releaseDomain() : Position ourself at the root of the XML tree
    xml->setDomain("totaldescription.global");
    auto aux_string = xml->readString("backgroundcolor");
    if(!aux_string.isEmpty()) helper.setBackgroundColor(target, QColor(aux_string));

    auto width = xml.readString("width").toInt();
    auto height = xml.readString("height").toInt();
    helper.setSize(target, width, height);

    auto hsizepolicy = xml->readString("hsizepolicy");
    auto vsizepolicy = xml->readString("vsizepolicy");
    helper.setSizePolicy(target, hsizepolicy, vsizepolicy);

    auto layout_dir = xml.readString("layout");
    setCentralWidget(target, layout_dir);
}
