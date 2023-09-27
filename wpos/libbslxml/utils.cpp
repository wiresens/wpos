/***************************************************************************
                          utils.cpp  -  description
                             -------------------
    begin                : mar oct 8 2002
    copyright            : (C) 2002 by Napsis S.L.
    email                : support@napsis.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "comp.h"   //to use the iostream classes
#include "utils.h"

Sector::Sector(){}
Sector::~Sector(){}

QString Sector::toString(const sector bytes)
{
	QString lower;
	QString upper;
    long low=0;
	long up=0;

	//there's a problem here, when we divide it can happen that low delete all the ceros at the left
	//that are not significant as it's a number but significant as to a string
	//for instance 20033 if factor were 1000 low will be 33 but it must be 033 or the string will
	// become 2033 instead of 20033, that's why we do that check

	up= bytes/FACTOR;
	low=bytes%FACTOR;

	upper.setNum(up); //there's no problem with the upper number

//	temp=(sector) up*FACTOR;
//	temp+=low;
	//if (((low*10)>bytes)&&(temp==bytes)) //ok
	//if((low*10)>bytes)
	if (up==0)
		lower.setNum(low);
	else{
//		if (temp!=bytes)
	               //something happens, there's no enought zeroes
			lower=correctZeroes(low,FACTOR);
	}	
	if (up==0)  //because, in any other way we'll have an extra 0
		return(lower);
	else
		return(upper+lower);
}

sector Sector::toSector(const QString text)
{
	QString lower;
	QString upper;
	long low;
	long up;
	sector temp;
  //we split the text in 2 and pass it to a long
	//then we just multiply and add to make the original number
  //note that we are losing numbers but 64 bits is big enought
	if (text.length()>=9)
		upper=text.left(text.length()-9);
	lower=text.right(9);
	low=lower.toLong();
	up=upper.toLong();
	temp=(long long)up*(long long)FACTOR;
	temp+=low;
	return(temp);
}

QString Sector::convenient(const sector bytes)
{
	int level=0;
	sector big;
	int small=0;
	long t=0;
	QString number;
	QString dotstring;
	int truncate=0;

	big=bytes;
	while(big> 1024)
	{
		level++;
		small=big%1024;
		big/=1024;
	}
	t=big;
	number.setNum(t);

	if (number.length()==1)
		truncate=2;
	else if (number.length()==2)
		truncate=1;
	else
		truncate=0;
	if (small>=1000)
			small=999;
  if(small)	
  	dotstring=correctZeroes(small,1000);//it's not very acurate, but as 1024 is almost 1000 don't seem to bother
	else
    dotstring=QString("0");

	dotstring.truncate(truncate);
	dotstring.prepend('.');
//	while (dotstring.at(dotstring.length()-1)=='0')
//		dotstring.truncate(dotstring.length()-1);
	if (dotstring.at(dotstring.length()-1)=='.')
		dotstring.truncate(0);		

	number+=dotstring;
	switch(level)
	{
	case 0:
		number+=" By";
		break;
	case 1:
		number+=" KB";
		break;
	case 2:
		number+=" MB";
		break;
	case 3:
		number+=" GB";
		break;
	case 4:
		number+=" TB";
		break;
	case 5:
		number+=" PB";
		break;
	}

	return(number);
}

QString Sector::correctZeroes(const long number_to_fix, const long limit )
{
	sector temp;
	QString result;
	int how_many_zeros=0;

	if(!number_to_fix) //it's 0, we know the result, this is safer and faster
	{
		result.setNum(limit);
		result=result.right(result.length()-1); //we eliminate the first number and leave the 0s
    return result;
	}
	temp=(sector)number_to_fix*10;
	while(temp<limit){// till we reach the number of numbers of FACTOR we do it
    if (temp==0)
      temp=limit;
		temp*=10;
		how_many_zeros++; 
	}
	result.setNum(number_to_fix);  
	for (int i=0;i<how_many_zeros;i++) //we add all that 0 to the begin of the string
		result.prepend("0"); 

	return(result);
}

bool String::compare(const QString& str, const QString& other)
{
  if (str.compare(other, Qt::CaseInsensitive)) return false;
  else return true;
}


QString String::stringOff(const QString& src)
{
  if (src.contains('.')) //we go one step back
  {
    return (src.left(src.lastIndexOf('.')));
  }
  else                   //we have no more steps to go back, we are nothing
    return (QString(""));
}

QString String::splitNumber(const QString& string,int *number, Align align)
{
  int length=string.length();
  int temp=0,i=0;
  QString result;

  if (align==END)
  {
    for (i=length-1; i>=0; i--)
    {
      if (!string.at(i).isNumber())
        break;
    }
    result=string.left(i+1);
    temp=string.right(length-i-1).toInt();
  }

  else if(align==BEGIN) //TODO: test this case
  {
    for (i=0; i<length; i++)
    {
      if (!string.at(i).isNumber())
        break;
    }
    result=string.right(length-i);
    temp=string.left(i).toInt();
  }
  if (number)
    *number=temp;
  return result;
}


