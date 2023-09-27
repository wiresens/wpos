/***************************************************************************
                          utils.h  -  description
                             -------------------
    begin                : mar oct 8 2002
    copyright            : (C) 2003 by NAPSIS s.L.
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
/*
In this file we'll have several utils
*/
#ifndef UTILS_H
#define UTILS_H


#include <QString>

//#if __GNUC__ ==2 && __GNUC_MINOR__ < 96
//#define __builtin_expect(x,expected_value) (x)
//#define __builtin_prefetch(address, read_write, locality) (x)
//#endif
//
//#define likely(x)    __builtin_expect((x),1)
//#define unlikely(x)  __builtin_expect((x),0)
//
//#define addToWriteCache(x) __builtin_prefetch(x,1,1)
//#define addToReadCache(x) __builtin_prefetch(x,1,1)

typedef long long sector;


 /******************************************************************************
	* @short This class will implement the string -> long long --> string conversions
	* That's necesary because the disk's size is returned
	* in long long and we want to read, and write to XML
	* This is part of libbslxml right now
	* @version 0.1
  ******************************************************************************/

class Sector {
public:
	Sector();
	~Sector();
 /*
	* Converts a sector (long long right now) to a string, it's used by the xmlconfig
	* @param bytes the number to be converted
	* @return the string with the number converted
	*/	
	QString toString(const sector bytes);
	
 /*
	* Converts the number contained in a string to a sector type
	* @param text the string to be converted
	* @result the number converted
	*/
	sector toSector(const QString text);
	
 /*
	* Converts a number to the convenient KB,MB, GB.. representation
	* @param bytes the number to be converted
	* @result the string with the number + sufix (KB,MB,GB,TB)
	*/
	 QString convenient(const sector bytes);

private:
	/* it takes a number and a limit and add zeroes to it till the number has
	* as many zeroes as the limit especifies. in the end of the function limit and
	* number_to_fix will have the same amount of numbers, so it can be put in 
	* string without losing data.
	* @param number_to_fix the number to add zeroes to
	* @param limit this number indicate the number of zeroes to add
	* @result the string with the number + zeroes, we can't return a number as
	* zeroes at the left are not significative 
	*/ 
	QString correctZeroes(const long number_to_fix, const long limit );

	static const long  FACTOR=1000000000; //the number we use to split the long long
};


class String{
public:

  enum Align
  {
    BEGIN=1,
    END=2
  };

 /**
  * Compare two strings without checking lowercase/uppercase issues, hello==Hello==heLLo ...
  * @param str the first string to compara
  * @param str2 the string to compare with
  * @return true if the string are equal or false otherwhise
  */
  static bool compare(const QString& str, const QString& other);

 /**
  * Go one step backwards in the string x[].y[].z[] --> x[].y[]
  * @param the string we look the parent for
  * @return the parent of the string
  */
  static QString stringOff(const QString& src);

 /**
  * get a string "strin392" and returns  strin  and  392 if the align is END
  * @param string the string to be divided
  * @param number the number we return, any better way of doing this?
  * @param align align change the behaivour if the align is END it assumes the
  * number is in the end of the string, if the align is BEGIN it assumes it's in
  * the beginning of the string
  * @returns the string without number
  */
  static QString splitNumber(const QString& string,int *number, Align align=END);

};




#endif


