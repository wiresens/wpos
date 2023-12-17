#include "iwidgetconfighelper.h"
#include <libbslxml/xmlconfig.h>

#include <QLayout>
#include <QLabel>

const QMap<QString, QSizePolicy::Policy>
    IWidgetConfigHelper::sizePolicies = {
    {"Expanding", QSizePolicy::Expanding},
    {"Preferred", QSizePolicy::Preferred},
    {"Fixed" , QSizePolicy::Fixed},
    {"Minimum" , QSizePolicy::Minimum},
    {"Maximum" , QSizePolicy::Maximum}
};

const QMap<QString, QFrame::Shadow>
    IWidgetConfigHelper::frameShadows = {
    {"Plain" , QFrame::Plain},
    {"Raised" , QFrame::Raised},
    {"Sunken" , QFrame::Sunken}
};

const QMap<QString, QFrame::Shape>
    IWidgetConfigHelper::frameShapes = {
    {"NoFrame" , QFrame::NoFrame},
    {"Box" , QFrame::Box},
    {"StyledPanel" , QFrame::StyledPanel},
    {"HLine" , QFrame::HLine}
};

const QMap<QString, Qt::Alignment>
    IWidgetConfigHelper::alignments = {
    {"Top" , Qt::AlignTop},
    {"Left" , Qt::AlignLeft},
    {"Bottom" , Qt::AlignBottom},
    {"Right" , Qt::AlignRight},
    {"Center", Qt::AlignCenter}
};

const QMap<QString, QAbstractItemView::SelectionMode>
    IWidgetConfigHelper::selectionModes = {
    {"SingleRow" , QAbstractItemView::SingleSelection},
    {"MultiRow" , QAbstractItemView::MultiSelection},
    {"Contigus" , QAbstractItemView::ContiguousSelection},
    {"Extended" , QAbstractItemView::ExtendedSelection},
    {"No", QAbstractItemView::NoSelection}
};

const QMap<QString, Qt::ScrollBarPolicy>
    IWidgetConfigHelper::scrolbarPolicies = {
    {"off" , Qt::ScrollBarAlwaysOff},
    {"on" , Qt::ScrollBarAlwaysOn},
    {"auto" , Qt::ScrollBarAsNeeded}
};

void  IWidgetConfigHelper::setBackgroundColor(
    QWidget &target,
    const QColor& color)
{
    QPalette plte;
    plte.setColor(target.backgroundRole(), color);
    target.setPalette(plte);
}

void  IWidgetConfigHelper::setSize(
    QWidget& target, uint w, uint h)
{
    target.setGeometry(
        target.geometry().x(),
        target.geometry().y(), w, h
    );
}

void  IWidgetConfigHelper::setSizePolicy(
    QWidget& target,
    const QString& hpolicy,
    const QString& vpolicy,
    uint hs,
    uint vs)
{
    auto szpolicy = target.sizePolicy();
    szpolicy.setHorizontalStretch(hs);
    szpolicy.setVerticalStretch(vs);
    szpolicy.setHorizontalPolicy(sizePolicies[hpolicy]);
    szpolicy.setVerticalPolicy(sizePolicies[vpolicy]);
    target.setSizePolicy( szpolicy);
}

void  IWidgetConfigHelper::setLayoutSpacing(
    QWidget& target,
    uint space)
{
    auto layout = target.layout();
    if(layout) layout->setSpacing(space);
}

void  IWidgetConfigHelper::setLayoutMargin(QWidget& target,
    uint margin)
{
    auto layout = target.layout();
    if(layout) layout->setMargin(margin);
}

void  IWidgetConfigHelper::setFrameShape(
    QWidget& target,
    const QString &shape)
{
    auto frame = qobject_cast<QFrame*>(&target);
    if( frame) frame->setFrameShape(frameShapes[shape]);
}

void  IWidgetConfigHelper::setFrameShadow(
    QWidget& target,
    const QString& shadow)
{
    auto frame = qobject_cast<QFrame*>(&target);
    if( frame) frame->setFrameShadow(frameShadows[shadow]);
}

void  IWidgetConfigHelper::setAlignment(
    QWidget& target,
    const QString &h,
    const QString &v)
{
    auto label = qobject_cast<QLabel*>(&target);
    label->setAlignment(alignments[h]);
    label->setAlignment(alignments[v]);
}

void  IWidgetConfigHelper::setFont(
    QWidget& target,
    const QFont& font)
{
    target.setFont(font);
}

void IWidgetConfigHelper::setFont(
    QWidget& target,
    XmlConfig &xmlDesc, int pos)
{
    QFont font ;
    QString tag;

    if( pos >= 0 )
        tag = "label["+QString::number(pos)+"].";
    auto tmpString = xmlDesc.readString( tag + "fontfamily");
    font.setFamily("SansSerif");

    tmpString = xmlDesc.readString( tag + "fontsize");
    font.setPixelSize(tmpString.toInt());

    tmpString = xmlDesc.readString( tag + "bold");
    font.setBold(tmpString == "true");

    tmpString = xmlDesc.readString( tag + "underline");
    font.setUnderline(tmpString == "true");

    tmpString = xmlDesc.readString( tag + "italic");
    font.setItalic(tmpString == "true");

    tmpString = xmlDesc.readString( tag + "strikeout");
    font.setStrikeOut(tmpString == "true");
    target.setFont(font);
}

void IWidgetConfigHelper::setSelectionMode(
    QAbstractItemView& view,
    const QString& mode)
{
    view.setSelectionMode(selectionModes[mode]);
}

void IWidgetConfigHelper::setHorizontalScrollBarPolicy(
    QAbstractScrollArea& area,
    const QString& policy)
{
    area.setHorizontalScrollBarPolicy(scrolbarPolicies[policy]);
}

void IWidgetConfigHelper::setVerticalScrollBarPolicy(
    QAbstractScrollArea& area,
    const QString& policy)
{
    area.setVerticalScrollBarPolicy(scrolbarPolicies[policy]);
}
