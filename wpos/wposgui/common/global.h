#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QSize>

struct Colors{
    static const  QString PRODUCT_SCREEN_BG_COLOR;
    static const  QString PRODUCT_SCREEN_SPECIAL_COLOR;
    static const  QString PRODUCT_SCREEN_DEFAULT_COLOR;
    static const  QString PRODUCT_SCREEN_ANULATION_COLOR;

    static const  QString PRODUCT_BG_COLOR;

    //static const  QString SPECIAL_BUTTON_POPUP_BG_COLOR;
    static const  QString SPECIAL_BUTTON_POPUP_BG_COLOR;

    static const  QString LOUNGE_TABLE_BUTTON_ON_COLOR;
    static const  QString LOUNGE_TABLE_BUTTON_OFF_COLOR;
    static const  QString LOUNGE_TABLE_BUTTON_LOCKED_COLOR;

    static const  QString MONEY_BUTTON_UP_COLOR;
    static const  QString MONEY_BUTTON_DWN_COLOR;

    //static const  QString MENU_PAGE_BG_COLOR;
    static const  QString MENU_PAGE_BG_COLOR;
    static const  QString MENU_STACK_BG_COLOR;

    static const  QString ORDER_WARNING_COLOR;
    static const  QString ORDER_NORMAL_COLOR;
    static const  QString ORDER_BUTTONS_CONTAINER_BG_COLOR;
    static const  QString ORDER_ANULATION_COLOR;
    static const  QString ORDER_DEFAULT_COLOR;

    static const  QString ADVANCED_ORDER_BG_COLOR;
    static const  QString ADVANCED_ORDER_BUTTONS_BG_COLOR;

};

/***
MENUSTACK NAMES
this should be at an XML with all the definitions
***/

struct Menus{
    static const  QString MAIN_MENU;
    static const  QString MAIN_SCREEN_MENU;
    static const  QString MAIN_MENU_ITEMS;
    static const  QString SUB_MENU_TREE;
    static const  QString AUTH_MENU;
    static const  QString SELECT_USER_MENU;
    static const  QString ADD_USER_MENU;
    static const  QString DDBB_MENU;
};

struct Sizes{
    static constexpr QSize PRODUCT_SIZE{ QSize(100,100) };
    static constexpr uint  FONT_SIZE_11{11};
    static constexpr uint  FONT_SIZE_8{8};
    static constexpr uint  ROW_HEIGTH{25};
};

#endif // COLORS_H
