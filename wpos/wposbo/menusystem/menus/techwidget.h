#ifndef TECHWIDGET_H
#define TECHWIDGET_H

#include <QString>
#include <QWidget>

#include "ui_techwidgetbase.h"

class TechWidget : public QWidget,
                   private Ui::TechWidgetBase {
    Q_OBJECT
public:
    explicit TechWidget(QWidget* parent = 0, const QString& name = QString {});
};

#endif // TECHWIDGET_H
