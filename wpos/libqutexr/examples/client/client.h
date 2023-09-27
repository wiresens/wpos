/****************************************************************************
** Form interface generated from reading ui file './libqutexr/examples/client/client.ui'
**
** Created: Tue Jul 20 15:57:41 2021
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.8   edited Jan 11 14:47 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CLIENT_H
#define CLIENT_H

#include <QVariant>
#include <QMainWindow>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QLabel;
class QLineEdit;
class QPushButton;
class QTabWidget;
class QWidget;
class QLCDNumber;
class QProgressBar;
class QTextEdit;

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client( QWidget* parent = nullptr);
    ~Client();

    QLabel* textLabel1;
    QLineEdit* lineEdit1;
    QPushButton* pushButton1;
    QTabWidget* tabWidget2;
    QWidget* tab;
    QLabel* textLabel2;
    QLabel* textLabel2_2;
    QLineEdit* lineEdit2;
    QLineEdit* lineEdit3;
    QLCDNumber* lCDNumber2;
    QLCDNumber* lCDNumber1;
    QLabel* textLabel3_2;
    QLabel* textLabel3;
    QWidget* tab_2;
    QLabel* textLabel1_2;
    QLabel* textLabel2_3;
    QProgressBar* progressBar1;
    QLabel* textLabel3_3;
    QTextEdit* textEdit1;
    QTextEdit* textEdit2;

protected:

protected slots:
    virtual void languageChange();

};

#endif // CLIENT_H
