/***************************************************************************
                          signals.h  -  description
                             -------------------
    begin                : jue jul 24 2003
    copyright            : (C) 2003 by Carlos Manzanedo
    email                : support@bluesock.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/**
*    this file contains all the signal definition
*    all the signals should be defined with an explanation of the functions that they rise
*    and if they are generic data signals, also an XML scheme of the data that is going to be
*    emited
*/
 
#ifndef H_SIGNALS
#define H_SIGNALS
#include <QString>

/*******************************************************************************************************
**                                 GENERIC SIGNALS
********************************************************************************************************/

struct GSIGNAL{
//used to open the cashbox whenever it's needed
    static const QString OPEN_CASHBOX;

    //used to enable and disable the main stack
    static const QString ENABLE_MAINSTACK;
    static const QString DISABLE_MAINSTACK;
    static const QString X;
    static const QString Z;
    static const QString WRONG_PRODUCT;
    static const QString AUTH_DISABLE_FINGERPRINT;
    static const QString AUTH_ENABLE_FINGERPRINT;

    //used to clear all the extras an options from the extracore
    static const QString XCORE_CLEAR_EXTRAS;
    static const QString BARCORE_EXIT_AND_SAVE_RECEIPT;
    static const QString BARCORE_SET_LAST_RECEIPT;
    static const QString BARCORE_DELETE_ACTUAL_RECEIPT;
    static const QString LOAD_PRODUCTS;
    static const QString LOAD_BUTTONS;
    static const QString LOAD_OFFERS;
    static const QString LOAD_DEVICES;
};

/*******************************************************************************************************
**                                 GENERIC  DATA SIGNALS
********************************************************************************************************/
struct GDATASIGNAL{
    //used to save the admin button state (used at program exit)
    static const QString MAINWIDGET_SAVE_BUTTON_STATE;

    //used to clear the extra core options list
    static const QString XCORE_DEL_OPTIONS;

//#define GDATASIGNAL_
/***********************************************
        GDATASIGNAL_Z    SCHEMA
        GDATASIGNAL_X    SCHEMA
*********************************************
this scheme is deprecated or at least it is too old... this should be
changed to the new scheme...

<?xml version = '1.0' encoding = 'UTF-8'?>
<AUTOMATICALLY_GENERATED_XML>
    <timestamp>24/07/2003 16:27:17</timestamp>
    <employees>
        <employee>
            <name>employee name</name>
            <id>dni</id>
            <num_tickets>int</num_tickets>
            <total_income>double</total_income>
        </employee>
    </employees>
    <main>
        <paytype>
            <name>nombre del tipo de pago</name>
            <num_tickets>int </num_tickets>
            <total_income>double</total_income>
        </paytype>
        <total>674</total>
    </main>
    <products>
        <product>
            <name>nombre del producto</name>
            <units>int </units>
            <total_income>double</total_income>
        </product>
    </products>
</AUTOMATICALLY_GENERATED_XML>
**********************************************/
    static const QString X;
    static const QString Z;

/**
<?xml version = '1.0' encoding = 'UTF-8'?>
<AUTOMATICALLY_GENERATED_XML>
     <name>name of the screen to be accessed</name>
</AUTOMATICALLY_GENERATED_XML>
*/
    static const QString MAINSTACK_SET_PAGE;
    static const QString PRODUCTS_SETPAGE;

/**
set an option to the extra core object, all the options will be added to the new products that the core
receives
<AUTOMATICALLY_GENERATED_XML>
     <type>option name</type>
     <value>option value</value>
</AUTOMATICALLY_GENERATED_XML>
*/
    static const QString XCORE_SET_PRODUCT_OPTION;

/**
set an option to the extra core object, all the options will be added to the new products that the core
receives
<AUTOMATICALLY_GENERATED_XML>
     <type>offer type</type>
     <name>offer name</value>
</AUTOMATICALLY_GENERATED_XML>
*/
    static const QString XCORE_SET_PRODUCT_OFFER;

/**
Set the default offer to the extra core. This offer will be used untill the optional
<AUTOMATICALLY_GENERATED_XML>
     <operation> can one of  "set" or "del" </operation>
     <type>offer type</type>
     <name>offer name</value>
</AUTOMATICALLY_GENERATED_XML>
*/
    static const QString XCORE_SET_DEFAULT_PRODUCT_OFFER;

/**
set price to a product. the core won't take the db price for a product, it will take the customized price from the extra core
<AUTOMATICALLY_GENERATED_XML>
     <price>float</price>
     <tax_type>string with the name of the tax</tax_type>
     <tax>float between 0 and 1</tax>
</AUTOMATICALLY_GENERATED_XML>
*/
    static const QString XCORE_SET_PRODUCT_PRICE;

/**
save an event at the database... most of the events will be processed at the eventlog core, and its
just a simple way to save the logs.
<AUTOMATICALLY_GENERATED_XML>
     <event_type>name of the event type</event_type>
     <cash_quantity> double </cash_quantity>    //optional
     <ticket_number> integer </ticket_number>  //optional
</AUTOMATICALLY_GENERATED_XML>
*/
    static const QString EVENTLOG;

/**
if a id is given it will look at the database to set this id, if there is no id, it look the name to set it
at the authentification module.
<AUTOMATICALLY_GENERATED_XML>
     <id>qstring de identificacion</id> 
     <name>nombre</nombre>    //optional
</AUTOMATICALLY_GENERATED_XML>
*/
    static const QString CHANGE_USER;

/**
if the main user has changed at the authentification module then a signal with this contents
is emited
<AUTOMATICALLY_GENERATED_XML>
     <id>qstring de identificacion</id>
     <name>nombre</nombre>
     <last_name>apellido</last_name>
</AUTOMATICALLY_GENERATED_XML>
*/
    static const QString USER_CHANGED;

/**
when the core receive this signal, it process the ticket and send to the ticket module
all the information.
<AUTOMATICALLY_GENERATED_XML>
     <pay_type>metalico</pay_type>
</AUTOMATICALLY_GENERATED_XML>
*/
    static const QString BARCORE_PROCESS_CORE;

/**
example of what it's needed to set a new product at the barcore...
<AUTOMATICALLY_GENERATED_XML>
        <product>
            <options>
                <option>
                    <type>servido en</type>
                    <value>tubo</value>
                </option>
            </options>
            <articles>
                <article>
                    <name>bacardi</name>
                </article>
                <article>
                    <name>fanta_naranja</name>
                </article>
            </articles>
        </product>
</AUTOMATICALLY_GENERATED_XML>
*/
    static const QString BARCORE_SEND_PRODUCT;

/**
<AUTOMATICALLY_GENERATED_XML>
     <mode>mode</mode> //the mode can be (normal, special,last or toggle) at now
                                        //the last color
                                        //the normal o special mode set the color to this predefined colors
                                        //the toggle will change the actual color to the other color
</AUTOMATICALLY_GENERATED_XML>
*/
    static const QString PRODSELECT_COLOR_MODE;

/**
<AUTOMATICALLY_GENERATED_XML>
     <mode>mode</mode> //the mode can be (normal, product_anulation, default_offer) at now
                                        //the product anulation set negative price for each product
                                        //that reaches to the core (this function is at the extra_core)
                                        //the default_offer set the color when a default offer is set.
</AUTOMATICALLY_GENERATED_XML>
*/
    static const QString SETCORE_MODE;

/**
Used to change the main XML at the BARCORE .... the scheme is a tipicall
------------------------------------------------------------------------
<AUTOMATICALLY_GENERATED_XML>
    <location>
        <lounge>barra</lounge>
        <table>0</table>
    </location>
    <timestamps>
        <start>06/10/2003 15:11:38</start>
        <last>06/10/2003 15:11:50</last>
    </timestamps>
    <employee>
        <name>usuario de sistema</name>
        <dni>1</dni>
    </employee>
    <products>
        <product>
            <timestamps>
                <ordertime>06/10/2003 15:11:46</ordertime>
            </timestamps>
            <name>fanta limon</name>
            <options/>
            <billing>
                <price>3</price>
                <currency>euro</currency>
                <tax type="iva7" >0.07</tax>
            </billing>
            <articles>
                <article>
                    <name>fanta_limon</name>
                </article>
            </articles>
        </product>
    <paytype>metalico</paytype>
</AUTOMATICALLY_GENERATED_XML>
----------------------------------------------------------------------
*/
    static const QString BARCORE_CHANGE_XML;

/**
Used to toggle the status of the printing section at the ticket core...
This is done at ticketcore because the printing system is going to be removed
from this project soon.
------------------------------------------------------------------------
<AUTOMATICALLY_GENERATED_XML>
        <state> state </state> //it could be on or off
</AUTOMATICALLY_GENERATED_XML>
----------------------------------------------------------------------
*/
    static const QString TICKETCORE_PRINTING_STATE;


/**
Used to print a number of copies of a ticket or the Z and X report
This is done at ticketcore because the printing system is going to be removed
from this project soon.
------------------------------------------------------------------------
<AUTOMATICALLY_GENERATED_XML>
        <copies>INT</copies> //an integer with the number of copies
</AUTOMATICALLY_GENERATED_XML>
----------------------------------------------------------------------
*/
    static const QString TICKETCORE_PRINTING_COPIES;

/**
Used to print a number of copies of a ticket or the Z and X report
This is done at ticketcore because the printing system is going to be removed
from this project soon.
------------------------------------------------------------------------
<AUTOMATICALLY_GENERATED_XML>
        <enabled>boolean</enabled> //true if default has been selected or false if lateral widgets must be disabled
</AUTOMATICALLY_GENERATED_XML>
----------------------------------------------------------------------
*/
    static const QString MAINWIDGET_SETENABLED;
    static const QString LATERALWIDGET_SET_VISIBLE;

/**
Used to activate or deactivate the button at the receipt widget that enable or disable the
actions that permit waiters to see and user receipts that they not own.
------------------------------------------------------------------------
<AUTOMATICALLY_GENERATED_XML>
        <enabled>boolean</enabled> //true if waiters can see other  waiters receipts.
</AUTOMATICALLY_GENERATED_XML>
----------------------------------------------------------------------
*/
    static const QString RECEIPTWIDGET_ALL_SETENABLED;


/**
Used to activate or deactivate the short mode of getting cash
if the state is on, the program should not go out to waiters menu...
------------------------------------------------------------------------
<AUTOMATICALLY_GENERATED_XML>
        <state> state </state> //it could be on or off
</AUTOMATICALLY_GENERATED_XML>
----------------------------------------------------------------------
*/
    static const QString CASH_MENU_SPEED;


/**
Used to set progress process and labels at the ReadConfigWidget... it should be
shown with GDATASIGNAL_MAINSTACK_SETPAGE to show the progressbar
------------------------------------------------------------------------
<AUTOMATICALLY_GENERATED_XML>
        <type>object that is doing reconfig ( products ) </type>
        <first_id>first_identifier</first_id>
        <last_id>last_identifier</last_id>
</AUTOMATICALLY_GENERATED_XML>
----------------------------------------------------------------------
*/
    static const QString REREAD_CONFIG;


/**
Used to set the description to the current order at the barcore to the
description at the XML
------------------------------------------------------------------------
<AUTOMATICALLY_GENERATED_XML>
        <description> the description </description>
</AUTOMATICALLY_GENERATED_XML>
----------------------------------------------------------------------
*/
    static const QString BARCORE_CHANGE_ORDER_DESCRIPTION;

/**
 * used to send to special printers the current order (and also mark products already sent)
Used to set the description to the current order at the barcore to the description at the XML
------------------------------------------------------------------------
<AUTOMATICALLY_GENERATED_XML>
        <type>printer_type</type>
</AUTOMATICALLY_GENERATED_XML>
----------------------------------------------------------------------
*/
    static const QString BARCORE_PRINT_ORDER_AT_SPECIAL_PRINTER;

};
#endif
