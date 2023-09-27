#ifndef ORDERMANAGERCONFIG_H
#define ORDERMANAGERCONFIG_H

#include "iwidgetconfig.h"

class OrderFrame;

class OrderManagerConfig : public IWidgetConfig
{
public:
    void setCentralWidget(QWidget& target, const QString &layout_dir) override;

protected:
    void createOder(OrderFrame* order);
};

#endif // ORDERMANAGERCONFIG_H
