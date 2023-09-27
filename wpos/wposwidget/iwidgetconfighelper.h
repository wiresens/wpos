#ifndef IWIDGETCONFIGHELPER_H
#define IWIDGETCONFIGHELPER_H

#include <QFrame>
#include <QMap>
#include <QString>

class QColor;
class QWidget;
class QSize;
class QSizePolicy;

class IWidgetConfigHelper{
public:
    virtual void setBackgroundColor(QWidget& target, const QColor&color);
    virtual void setSize(QWidget& target, uint w, uint h);
    virtual void setSizePolicy(QWidget& target, const QString& hpolicy, const QString& vpolicy, uint hs = 0 , uint vs = 0);
    virtual void setLayoutSpacing(QWidget& target, uint space);
    virtual void setFrameShape(QWidget& target, const QString& shape);
    virtual void setFrameShadow(QWidget& target, const QString &shadow);
    virtual void setAlignment(QWidget& target, const QString& h, const QString& v = "Center");
    virtual void setFont(QWidget& target, const QFont& font);

private :
    static const QMap<QString, QSizePolicy::Policy> sizePolicies;
    static const QMap<QString, QFrame::Shadow> frameShadows;
    static const QMap<QString, QFrame::Shape> frameShapes;
    static const QMap<QString, Qt::Alignment> alignments;
};

#endif // IWIDGETCONFIGHELPER_H
