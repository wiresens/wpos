/*

qutexrlib QT-based XML-RPC library
Copyright (C) 2003  P. Oscar Boykin <boykin@pobox.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Modified by Carlos Manzanedo <carlos@napsis.com>
Ported to Qt 5.12.8 by Gilles Bene Pougoue <gilles.benepougoue@gmail.com>

*/

#ifndef _XRBASE64
#define _XRBASE64
#include <QString>

/**
 * for some god forsaken reason, QT does not include Base64 conversion
 * code.  Here is something simple.
 */

struct XRBase64 {

    static const int MAX_LINE_LENGTH;
    static const char _vec[];
    static const char _padding;

    static QByteArray decode(QString ascii);
    static QString encode(const QByteArray& byteArray);

    protected:
    static int convertToNumber(char a_byte);

};
#endif
