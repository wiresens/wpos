#include "supportwidget.h"

SupportWidget::SupportWidget(QWidget* parent, const QString& name)
    : QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
}
