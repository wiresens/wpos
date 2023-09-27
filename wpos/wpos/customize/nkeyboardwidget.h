/****************************************************************************
** Form interface generated from reading ui file './ntpv/customize/nkeyboardwidget.ui'
**
** Created: Tue Jul 20 15:57:41 2021
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef NKEYBOARDWIDGET_H
#define NKEYBOARDWIDGET_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLineEdit;
class QFrame;
class QWidgetStack;
class QPushButton;

class NKeyboardWidget : public QWidget
{
    Q_OBJECT

public:
    NKeyboardWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~NKeyboardWidget();

    QLineEdit* output_line;
    QFrame* line1;
    QWidgetStack* stack;
    QWidget* kb_page;
    QPushButton* bdel;
    QPushButton* b7;
    QPushButton* benter;
    QPushButton* bn;
    QPushButton* bt;
    QPushButton* bh;
    QPushButton* bb;
    QPushButton* bd;
    QPushButton* ba;
    QPushButton* b0;
    QPushButton* bs;
    QPushButton* bl;
    QPushButton* br;
    QPushButton* bu;
    QPushButton* by;
    QPushButton* b2;
    QPushButton* bdot;
    QPushButton* b5;
    QPushButton* bp;
    QPushButton* b8;
    QPushButton* bcut;
    QPushButton* b4;
    QPushButton* bz;
    QPushButton* bf;
    QPushButton* bmsg;
    QPushButton* blalt;
    QPushButton* brctrl;
    QPushButton* bx;
    QPushButton* bm;
    QPushButton* bk;
    QPushButton* bq;
    QPushButton* bspace;
    QPushButton* blsh;
    QPushButton* b3;
    QPushButton* b1;
    QPushButton* bc;
    QPushButton* bg;
    QPushButton* bv;
    QPushButton* b9;
    QPushButton* b6;
    QPushButton* bi;
    QPushButton* brsh;
    QPushButton* bo;
    QPushButton* be;
    QPushButton* bw;
    QPushButton* blmi;
    QPushButton* bralt;
    QPushButton* bj;
    QPushButton* btab;
    QPushButton* blctrl;
    QPushButton* bmay;
    QPushButton* bint;
    QPushButton* br2;
    QPushButton* br4;
    QPushButton* br1;
    QPushButton* brplus;
    QPushButton* br9;
    QPushButton* brdot;
    QPushButton* brenter;
    QPushButton* br3;
    QPushButton* br6;
    QPushButton* br0;
    QPushButton* br5;
    QPushButton* br7;
    QPushButton* br8;
    QPushButton* bbloq;
    QPushButton* bdiv;
    QPushButton* bprod;
    QPushButton* brmin;
    QPushButton* esc;
    QPushButton* f1;
    QPushButton* f2;
    QPushButton* f3;
    QPushButton* f4;
    QPushButton* f5;
    QPushButton* f6;
    QPushButton* f7;
    QPushButton* f8;
    QPushButton* f9;
    QPushButton* f10;
    QPushButton* f11;
    QPushButton* f12;
    QPushButton* clear_button;
    QPushButton* exit_button;
    QPushButton* lounge_button;
    QPushButton* ben;
    QPushButton* oa;
    QWidget* lounge_page;
    QFrame* button_frame;
    QPushButton* kb_button;
    QPushButton* tables_exit_button;
    QFrame* line2;
    QWidgetStack* lounge_stack;
    QWidget* WStackPage;

protected:
    QVBoxLayout* NKeyboardWidgetLayout;
    QVBoxLayout* lounge_pageLayout;
    QHBoxLayout* layout2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;

};

#endif // NKEYBOARDWIDGET_H
