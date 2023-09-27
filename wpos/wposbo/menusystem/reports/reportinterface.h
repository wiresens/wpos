/***************************************************************************
                          nprofitreportwidgetinterface.h  -  description
                             -------------------
    begin                : Mon may 5 2004
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/
#ifndef REPORTINTERFACE_H
#define REPORTINTERFACE_H

class ReportInterface{
    virtual void reportSlot(QString)=0; //ASYNC
};
#endif
