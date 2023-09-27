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

#include "signals.h"
#include <QString>

//GENERIC SIGNALS
    const QString GSIGNAL::AUTH_DISABLE_FINGERPRINT                     {"GSIGNAL_AUTH_DISABLE_FINGERPRINT"};
    const QString GSIGNAL::AUTH_ENABLE_FINGERPRINT                      {"GSIGNAL_AUTH_ENABLE_FINGERPRINT"};
    const QString GSIGNAL::BARCORE_EXIT_AND_SAVE_RECEIPT                {"GSIGNAL_BARCORE_EXIT_AND_SAVE_RECEIPT"};
    const QString GSIGNAL::BARCORE_SET_LAST_RECEIPT                     {"GSIGNAL_BARCORE_SET_LAST_RECEIPT"};
    const QString GSIGNAL::BARCORE_DELETE_ACTUAL_RECEIPT                {"GSIGNAL_BARCORE_DELETE_ACTUAL_RECEIPT"};
    const QString GSIGNAL::DISABLE_MAINSTACK                            {"GSIGNAL_DISABLE_MAINSTACK"};
    const QString GSIGNAL::ENABLE_MAINSTACK                             {"GSIGNAL_ENABLE_MAINSTACK"};
    const QString GSIGNAL::LOAD_PRODUCTS                                {"GSIGNAL_LOAD_PRODUCTS"};
    const QString GSIGNAL::LOAD_BUTTONS                                 {"GSIGNAL_LOAD_BUTTONS"};
    const QString GSIGNAL::LOAD_OFFERS                                  {"GSIGNAL_LOAD_OFFERS"};
    const QString GSIGNAL::LOAD_DEVICES                                 {"GSIGNAL_LOAD_DEVICES"};
    const QString GSIGNAL::OPEN_CASHBOX                                 {"GSIGNAL_OPEN_CASHBOX"};
    const QString GSIGNAL::WRONG_PRODUCT                                {"GSIGNAL_WRONG_PRODUCT"};
    const QString GSIGNAL::X                                            {"GSIGNAL_X"};
    const QString GSIGNAL::XCORE_CLEAR_EXTRAS                           {"GSIGNAL_XCORE_CLEAR_EXTRAS"};
    const QString GSIGNAL::Z                                            {"GSIGNAL_Z"};

//GENERIC  DATA SIGNALS
    const QString GDATASIGNAL::BARCORE_CHANGE_ORDER_DESCRIPTION         {"GDATASIGNAL_BARCORE_CHANGE_ORDER_DESCRIPTION"};
    const QString GDATASIGNAL::BARCORE_CHANGE_XML                       {"GDATASIGNAL_BARCORE_CHANGE_XML"};
    const QString GDATASIGNAL::BARCORE_PRINT_ORDER_AT_SPECIAL_PRINTER   {"GDATASIGNAL_BARCORE_PRINT_ORDER_AT_SPECIAL_PRINTER"};
    const QString GDATASIGNAL::BARCORE_PROCESS_CORE                     {"GDATASIGNAL_BARCORE_PROCESS_CORE"};
    const QString GDATASIGNAL::BARCORE_SEND_PRODUCT                     {"GDATASIGNAL_BARCORE_SEND_PRODUCT"};
    const QString GDATASIGNAL::CHANGE_USER                              {"GDATASIGNAL_CHANGE_USER"};
    const QString GDATASIGNAL::CASH_MENU_SPEED                          {"GDATASIGNAL_CASH_MENU_SPEED"};
    const QString GDATASIGNAL::EVENTLOG                                 {"GDATASIGNAL_EVENTLOG"};
    const QString GDATASIGNAL::LATERALWIDGET_SET_VISIBLE                {"GDATASIGNAL_LATERALWIDGET_SET_VISIBLE"};
    const QString GDATASIGNAL::MAINSTACK_SETPAGE                        {"GDATASIGNAL_MAINSTACK_SETPAGE"};
    const QString GDATASIGNAL::MAINWIDGET_SAVE_BUTTON_STATE             {"GDATASIGNAL_MAINWIDGET_SAVE_BUTTON_STATE"};
    const QString GDATASIGNAL::MAINWIDGET_SETENABLED                    {"GDATASIGNAL_MAINWIDGET_SETENABLED"};
    const QString GDATASIGNAL::PRODSELECT_COLOR_MODE                    {"GDATASIGNAL_PRODSELECT_COLOR_MODE"};
    const QString GDATASIGNAL::PRODUCTS_SETPAGE                         {"GDATASIGNAL_PRODUCTS_SETPAGE"};
    const QString GDATASIGNAL::RECEIPTWIDGET_ALL_SETENABLED             {"GDATASIGNAL_RECEIPTWIDGET_ALL_SETENABLED"};
    const QString GDATASIGNAL::REREAD_CONFIG                            {"GDATASIGNAL_REREAD_CONFIG"};
    const QString GDATASIGNAL::SETCORE_MODE                             {"GDATASIGNAL_SETCORE_MODE"};
    const QString GDATASIGNAL::TICKETCORE_PRINTING_STATE                {"GDATASIGNAL_TICKETCORE_PRINTING_STATE"};
    const QString GDATASIGNAL::TICKETCORE_PRINTING_COPIES               {"GDATASIGNAL_TICKETCORE_PRINTING_COPIES"};
    const QString GDATASIGNAL::USER_CHANGED                             {"GDATASIGNAL_USER_CHANGED"};
    const QString GDATASIGNAL::XCORE_DEL_OPTIONS                        {"GDATASIGNAL_XCORE_DEL_OPTIONS"};
    const QString GDATASIGNAL::X                                        {"GDATASIGNAL_X"};
    const QString GDATASIGNAL::Z                                        {"GDATASIGNAL_Z"};
    const QString GDATASIGNAL::XCORE_SET_PRODUCT_OPTION                 {"GDATASIGNAL_XCORE_SET_PRODUCT_OPTION"};
    const QString GDATASIGNAL::XCORE_SET_PRODUCT_OFFER                  {"GDATASIGNAL_XCORE_SET_PRODUCT_OFFER"};
    const QString GDATASIGNAL::XCORE_SET_DEFAULT_PRODUCT_OFFER          {"GDATASIGNAL_XCORE_SET_DEFAULT_PRODUCT_OFFER"};
    const QString GDATASIGNAL::XCORE_SET_PRODUCT_PRICE                  {"GDATASIGNAL_XCORE_SET_PRODUCT_PRICE"};
