#include "iwidgetconfighelper.h"
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
