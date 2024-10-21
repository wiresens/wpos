#ifndef IWIDGETCONFIG_H
#define IWIDGETCONFIG_H

class IWidgetConfigHelper;
class QWidget;
class QLayout;
class QString;

class IWidgetConfig{
public:
    virtual void setupWidget(QWidget& target, IWidgetConfigHelper& helper, const QString& file);
    virtual void setCentralWidget(QWidget&, const QString& layout_dir = "HBox"){}
};

#endif // IWIDGETCONFIG_H
