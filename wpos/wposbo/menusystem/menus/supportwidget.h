#ifndef SUPPORTWIDGET_H
#define SUPPORTWIDGET_H

#include "ui_supportwidgetbase.h"
#include <QWidget>

class SupportWidget :
        public QWidget,
        private Ui::SupportWidgetBase
{
    Q_OBJECT
public:
    explicit SupportWidget(QWidget *parent = 0, const QString & name = QString());
};

#endif // SUPPORTWIDGET_H
