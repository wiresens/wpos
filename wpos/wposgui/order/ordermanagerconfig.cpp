#include "ordermanagerconfig.h"
#include <QLayout>

void OrderManagerConfig::setCentralWidget(QWidget& target, const QString& layout_dir){

    QLayout* l_layout{};

    if(layout_dir == "HBox") l_layout = new QHBoxLayout(&target);
    else if(layout_dir == "VBox") l_layout = new QVBoxLayout(&target);
    layout_dir->setObjectName("MAIN_LAYOUT");

    createOder(order);

    m_order = new OrderFrame(this, "ORDER");
    m_main_blayout->addWidget(m_order);
}
