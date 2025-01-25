#include "techwidget.h"

TechWidget::TechWidget(QWidget* parent, const QString& name)
    : QWidget(parent)
{
    setupUi(this);
    setObjectName(name);
}
