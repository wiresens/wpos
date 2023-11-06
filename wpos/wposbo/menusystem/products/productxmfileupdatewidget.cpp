/***************************************************************************
                          updatexml.cpp  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#include "productxmfileupdatewidget.h"

#include "productsmodule/productmodule.h"

#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QGroupBox>
#include <QApplication>

ProductXmFileUpdateWidget::ProductXmFileUpdateWidget(
        ProductModule *_product_mod,
        QWidget *parent, const QString& name):
    QWidget(parent),
    product_mod{_product_mod}
{
    setupUi(this);
    setObjectName(name);
    progress_frame->hide();
    ok_button->setIcon(QPixmap("controls48:button_ok_48.png"));
    connect(ok_button, &QPushButton::released, this, &ProductXmFileUpdateWidget::acceptSlot);
}

void ProductXmFileUpdateWidget::acceptSlot(){
    products = product_mod->getNumberOfProducts();
    progress->setValue(products);
    ok_button->setEnabled(false);

    connect(product_mod, &ProductModule::productAtXml, this, &ProductXmFileUpdateWidget::updatingProduct);

    advertise_label->setText(tr("The product's XML file is being written.\nThis may take some time"));
    progress_frame->show();
    product_mod->saveBarXml();
}

void ProductXmFileUpdateWidget::updatingProduct(int num, const QString &product){
    if ( num == products){
        progress->setValue(0);
        product_label->setText(0);
        progress_frame->hide();
        advertise_label->setText(tr("Click accept to start updating the products."));
        disconnect(product_mod, &ProductModule::productAtXml, this, &ProductXmFileUpdateWidget::updatingProduct);
        products = -1;
        ok_button->setEnabled(true);

        //this code can be removed. It has been coded to avoid errors of use.
        ok_button->setEnabled(false);
        advertise_label->setText(tr("La actualizacion del XML se ha realizado con exito."));
    }
    else{
        progress->setValue(num);
        product_label->setText(product);
        update();
        qApp->processEvents();
    }
}

void ProductXmFileUpdateWidget::showEvent(QShowEvent *e){
    if ( products < 0 ){
        progress->setValue(0);
        product_label->setText(0);
        progress_frame->hide();
        advertise_label->setText(tr("Pulse aceptar para comenzar con la actualizacion de los productos."));
        ok_button->setEnabled(true);
    }
    QWidget::showEvent(e);
}


