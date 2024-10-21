#ifndef I_WIDGET_CONFIG_HELPER_H
#define I_WIDGET_CONFIG_HELPER_H

#include <QAbstractItemView>
#include <QFrame>
#include <QMap>
#include <QString>

class XmlConfig;
class QColor;
class QWidget;
class QSize;
class QSizePolicy;

class IWidgetConfigHelper{
public:
    virtual void setBackgroundColor(QWidget& target, const QColor& color);
    virtual void setSize(QWidget& target, uint w, uint h);
    virtual void setSizePolicy(QWidget& target, const QString& hpolicy,
                               const QString& vpolicy, uint hs = 0 , uint vs = 0);
    virtual void setLayoutSpacing(QWidget& target, uint space);
    virtual void setLayoutMargin(QWidget& target, uint margin);
    virtual void setFrameShape(QWidget& target, const QString& shape);
    virtual void setFrameShadow(QWidget& target, const QString &shadow);
    virtual void setAlignment(QWidget& target, const QString& h, const QString& v = "Center");
    virtual void setFont(QWidget& target, const QFont& font);
    virtual void setFont(QWidget& target, XmlConfig &xmlDesc, int pos = -1);

    virtual void setSelectionMode(QAbstractItemView& view, const QString& mode);
    virtual void setHorizontalScrollBarPolicy(QAbstractScrollArea& area, const QString& policy);
    virtual void setVerticalScrollBarPolicy(QAbstractScrollArea& area, const QString& policy);

private :
    static const QMap<QString, QSizePolicy::Policy> sizePolicies;
    static const QMap<QString, QFrame::Shadow> frameShadows;
    static const QMap<QString, QFrame::Shape> frameShapes;
    static const QMap<QString, Qt::Alignment> alignments;
    static const QMap<QString, QAbstractItemView::SelectionMode> selectionModes;
    static const QMap<QString, Qt::ScrollBarPolicy> scrolbarPolicies;
};

#endif // I_WIDGET_CONFIG_HELPER_H
