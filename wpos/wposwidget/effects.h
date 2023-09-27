/***************************************************************************
                          effects.h  -  description
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

#ifndef EFFECTS_H
#define EFFECTS_H


 /**
  * In this class we'll have the effects for the gui, the effects normaly require a image and returns other
  * image modified. This class may be shared between projects so maybe will become part of a library
  * Note that i'm supposing that the images will be 32bits, will we ever use other thing?
  * @author 
  */

class QImage;
class QWidget;
class QString;
class QPixmap;
class QFont;


class Effects {
public: 
	Effects();
	~Effects();

 /*
  *  Add gammna to a image, this is the classical gamma code
  * @param img the image to be lighted
  * @param value the amount of light, default 0.8
  * @return the image lighted
  */
  static QImage light(const QImage &img, float value=0.8);

 /*
  * double the pixels of an image, i want to achieve the same effect that has
  * kicker, what i don't know is if that will change all the layout
  * @param src the image we want to double
  * @return the image doubled
  */
  static QImage doublePixels(const QImage& src);

 /*
  * Returns the same image but grayscaled, if needed we can add a param to make more or less gray, as
  * we want, right now just make it gray.
  * @param img the image to be grayscaled
  * @return the image grayscaled
  */
  static QImage toGray(const QImage &img);

 /*
  * make the image semitranspared
  * We can make the image as transparent as needed , right now just make 50% transparent
  * @param original image
  * @return the semitransparent image
  */
  static QImage semiTransparent(const QImage &img);

 /*
  * "Activate" the pixmap, right now, give light and double pixels, this effects can change in the future
  * @param label the label we want to activate, the pixmap get changed
  */
  static void activate(QWidget *widget);

 /*
  * Draws the text in  that pixmap, note that the pixmap will be modified
  * @param text the text to draw in the pixmap
  * @param pm the pixmap to draw into.
  */
  static void drawTextInPixmap(const QString& text, QPixmap *pm );

  
  /*
  * Draws the text in  that pixmap, note that the pixmap will be modified
  * @param text the text to draw in the pixmap
  * @param pm the pixmap to draw into.
  * @param font with the font to write at the application.
  */
  static void drawTextInPixmap(const QString& text, QPixmap *pm, QFont font );

};

#endif
