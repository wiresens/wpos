# wPOS
**C++ based Point of Sale System with Qt5 and DBus**

----------------------------------------------------------------
## Background History

**nTPV** was a point of sale (POS) system designed and developed by **Napsis Corp in 2003**,
implemented in **C++** with **Qt3**.

After Napsis closure, nTPV was mantained by Napsis founders and partners 
as a Open Source Project on sourceforge until **December 2008**.

It was initially designed to manage restaurants, bars, cafes, and similar businesses, 
but now, it's evolving into a general purpose POS. It uses **DCOP** and **nkxmlrpcd** for 
distributed comunications among POS, and uses **PostgreSQL** as its database backend. 
It features a fast and easy-to-use interface. Its high modularity and scalability 
makes it easy to enhance and mantain.

		[DCOPFX2000]  Biometric fingerprint autentication of waiters and users.
	 	[DCOPRECEIPT] Easy use of orders, tables, and invoice saving... no more ordering notebooks are needed
		[DCOPREPORTS] Reports generated with this plugin allows to obtain information to increase benefits.
	 	[DCOPSTICK]   Manage your backups and store your reports with USB storage drives.

But nTPV as developped by napsis had some problems which made it's maintenance difficults:
* No use of the **MVC design patern** to separate GUI from Data
* Used mostly **Transaction Script** and sometime **Table Module** to model the Domain
  
The system was working only on old Linux systems supporting KDE and Qt3 which both are legacy technology abandonned
for Years. But since software had some potential and was widelly used in those days, at Wiresens we undertook 
to port it the recent technologies and refactor its design the overcome the two problems mentioned above.

In this regard, so far we have currently done the following successfully:
* Ported form **Qt3** to **Qt5**
* Replaced **DCOP** with **DBus**
* Repalced the Gosview Document viewer by an internal PDF Reader

So presently, it has the same functionality as legacy nTPV. We are working hard to make it more general
to be used in all retail systems:
The system is now called **wPOS**

## TODO:

### We are currently working on :
* Adopting an MVC approach to sepparate the GUI from Data
* Transition from Transaction Script to **Domain Model**
* Transition from **QWidget** to **QM**L to be supported on Mobile Patforms
* Transition from **qmake** build system  to **cmake**

## wPOS Dependecies on Ubuntu 22.04( Ubuntu packages):

	- qt5 (https://www.qt.io)	
	- qtpdf5-dev libqt5pdf5 libqt5pdfwidgets5 libqt5webenginewidgets5 libqt5sql5-psql
	- libgdchart (http://www.fred.net/brv/chart/) : libgdchart-gd2-xpm, libgdchart-gd2-xpm-dev
	- libxml2 (http://xmlsoft.org/)
	
## Building wPOS.  (Quick method.)

**wPOS** now use qmake package generation. It's very simple and quick to compile and install. 
	qmake and make must be installed.

  	> git clone https://github.com/wiresens/wpos.git
  	> cd wpos/wpos
  	> qmake wpos.pro -spec linux-g++-64
  	> make -j4
  	> sudo make install

And that's all.

## Cleaning all sources.

  	> make clean


## Uninstalling.

	> sudo make uninstall

Or you can use the latest **Qtcreator IDE**. At the time of this writting, it was QtCreator 11 on **Kubuntu 22.04**
