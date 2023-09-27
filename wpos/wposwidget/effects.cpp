/***************************************************************************
                          effects.cpp  -  description
                             -------------------
    begin                : mar sep 24 2002
    copyright            : (C) 2004 by Napsis S.L.
    email                : carlos@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "effects.h"
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QPainter>
#include <QLabel> //for activate
#include <QPushButton>
#include <QAbstractButton>
#include <math.h> //for the good gamma aproximation, we can do without this

Effects::Effects(){}
Effects::~Effects(){}

QImage Effects::light(const QImage &img, float value){
    int pixels, r, g, b;
    unsigned int *raw;
    QColor color;
    QImage result;
    result = img;
    float gamma;

//    raw=static_cast<unsigned int *>result.bits();
    raw = (unsigned int *)result.bits();
    pixels = result.width()*result.height();

    gamma = 1/(2*value+0.5);
//   gamma=0.3;
    for (int i = 0; i < pixels; i++){
        color.setRgb(raw[i]);
        color.setRgb(r,g,b);
        r = static_cast<int>(pow(static_cast<float>(r)/255 , gamma)*255);
        g = static_cast<int>(pow(static_cast<float>(g)/255 , gamma)*255);
        b = static_cast<int>(pow(static_cast<float>(b)/255 , gamma)*255);
        raw[i] = qRgba(r,g,b, qAlpha(raw[i]));
    }
    return result;
}

QImage Effects::doublePixels(const QImage& src){
    int x, y;
    int width = src.width();
    int height = src.height();
    QImage result { width*2, height*2, QImage::Format_ARGB32};

//    if (src.hasAlphaChannel())
//        result.setAlphaBuffer(true);
//    else
//        result.setAlphaBuffer(true);

    QRgb *l1, *l2;
    for (y=0; y<height; y++)
    {                                                         // This do this
      l1 = (QRgb *) src.scanLine(y); //escanea una linea      // a --> aa
      l2 = (QRgb *) result.scanLine(y*2);     //inicializa    //       aa
      for (x=0; x<width; x++)
      {
        l2[x*2] = l2[x*2+1] = l1[x];
      }
      memcpy(result.scanLine(y*2+1), l2, result.bytesPerLine());
    }
    return result;
}

QImage Effects::toGray(const QImage &img){
    int pixels,hue,sat,val;
    unsigned int *raw;
    QColor color;
    QImage result;
    result=img;
    raw=(unsigned int *)result.bits();
    pixels=result.width()*result.height();

    for (int i=0; i<pixels; i++){
//      val=qGray(raw[i]);
//      raw[i]=qRgba(val,val,val, qAlpha(raw[i]));

//i think this is faster than calling qt algorithm
//the amount of colour is 1 so is gray, that value can be changed
//to make differents grays
        color.setRgb(raw[i]);
        color.getHsv(&hue, &sat, &val);
        color.setHsv(hue, 1 , val);
        raw[i] = qRgba(color.red(), color.green(), color.blue(),
                qAlpha(raw[i]));
    }
    return result;
}

//NOTE: the image must be 32bits to work
QImage Effects::semiTransparent(const QImage &img){
    int x, y,width,height;
    QImage result=img;

//  result.setAlphaBuffer(true); //if we don't do this we are really dead

    width  = result.width();
    height = result.height();
    for (y=0; y<height; y++){
        QRgb *line = (QRgb *) result.scanLine(y);
        for (x=(y%2); x<width; x+=2)  //2 we delete 1 point each 2
             line[x] &= 0x00ffffff;
        }
    return result;
}

//void Effects::activate(QLabel *widget)
//{
//  if (!widget)
//    return;
//  const QPixmap *pix=0;
//
//  pix= widget->pixmap();
//  if (pix)
//  {
//    QImage img=pix->convertToImage();
//    if (!img.isNull())
//    {
//      img=Effects::light(img,1);
//      img=Effects::doublePixels(img);
//

void Effects::activate(QWidget* widget){
    if (!widget)  return;
//    const QPixmap* pix {nullptr};

    QPixmap pix;
    if (widget->inherits("QLabel"))
        pix = ((QLabel*) widget)->pixmap()->copy();
    else if (widget->inherits("QPushButton"))
//        pix = new QPixmap{((QPushButton*) widget)->icon().pixmap(widget->size())};
          pix =  ((QPushButton*) widget)->icon().pixmap(widget->size());
    if (!pix.isNull()){
        QImage img = pix.toImage();
        if(!img.isNull()){
            img = Effects::light(img,1);
            img = Effects::doublePixels(img);
            pix.convertFromImage(img);
            if (widget->inherits("QLabel"))
                ((QLabel *) widget)->setPixmap(pix);
            else if (widget->inherits("QPushButton")){
//To do                ((QPushButton *) widget)->setPixmap(pix);

            }
       }
    }
}

void Effects::drawTextInPixmap(const QString& text, QPixmap *pm ){
    if (!pm) return;
    QPainter p(pm);
    p.setFont(QFont("helvetica", 9, QFont::Bold));
    p.setPen(Qt::white);
    p.drawText(3, 2, pm->width(), pm->height()-2, Qt::AlignHCenter | Qt::AlignBottom, text);
    p.setPen(Qt::black);
    p.drawText(1, 0, pm->width(), pm->height()-2, Qt::AlignHCenter | Qt::AlignBottom, text);
}

void Effects::drawTextInPixmap(const QString& text, QPixmap *pm, QFont font){
    if (!pm) return;
    QPainter p(pm);

    p.setFont(font);
    p.setPen(Qt::white);
    p.drawText(3, 2, pm->width(), pm->height()-2, Qt::AlignHCenter | Qt::AlignBottom, text);
    p.setPen(Qt::black);
    p.drawText(1, 0, pm->width(), pm->height()-2, Qt::AlignHCenter | Qt::AlignBottom, text);
}
