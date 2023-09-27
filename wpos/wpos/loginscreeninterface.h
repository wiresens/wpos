/***************************************************************************
                          loginscreeninterface.h  -  description
                             -------------------
    begin                 : mon Jun 2 2003
    copyright          : (C) 2003 by Napsis S.L.
    email                 : carlos@napsis.com

@author Carlos Manzanedo Rueda

%LICENCIA%
 ***************************************************************************/

#ifndef LOGIN_SCREEN_INTERFACE_H
#define LOGIN_SCREEN_INTERFACE_H

//this should be readed from the dcopfx2000 lib...
//class FX2000MatchData{
//public:
//     QString identity;
//     int finger;
//     //fingerprint
//     float similarity;
//     QString fingerprint_path;
//     QString model_path;
//};

//@BENES
class LoginScreenInterface {
public:
    virtual void getMatchResults(QString) = 0;
};
#endif
