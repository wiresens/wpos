unix{
  !exists($$DCOPIDL){
    error("dcopidl not found")
  }
  !exists($$DCOPIDL2CPP){
    error("dcopidl2cpp not found")
  }
  message("Using dcopidl and dcopidl2cpp to create bslenterthematrixinterface skel"){
    system(rm -f bslenterthematrixinterface_skel.cpp)
    system(dcopidl bslenterthematrixinterface.h > bslenterthematrixinterface.kidl)
    system(dcopidl2cpp bslenterthematrixinterface.kidl){
      SOURCES += bslenterthematrixinterface_skel.cpp
      QMAKE_CLEAN += bslenterthematrixinterface_skel.cpp \
      bslenterthematrixinterface.kidl \
      bslenterthematrixinterface_stub.h \
      bslenterthematrixinterface_stub.cpp
    }
  }
  message("Using dcopidl and dcopidl2cpp to create filealterationwatcherinterface skel"){
    system(rm -f barcore/filealterationwatcherinterface_skel.cpp)
    system(dcopidl barcore/filealterationwatcherinterface.h > barcore/filealterationwatcherinterface.kidl)
    system(dcopidl2cpp barcore/filealterationwatcherinterface.kidl ){
      SOURCES += barcore/filealterationwatcherinterface_skel.cpp
      QMAKE_CLEAN += barcore/filealterationwatcherinterface_skel.cpp \
      barcore/filealterationwatcherinterface.kidl \
      barcore/filealterationwatcherinterface_stub.h \
      barcore/filealterationwatcherinterface_stub.cpp
    }
  }
  message("Using dcopidl and dcopidl2cpp to create linubarreceiptinterface skel"){
    system(rm -f menusystem/menus/linubarreceiptinterface_skel.cpp)
    system(dcopidl menusystem/menus/linubarreceiptinterface.h > menusystem/menus/linubarreceiptinterface.kidl)
    system(dcopidl2cpp menusystem/menus/linubarreceiptinterface.kidl){
      SOURCES += menusystem/menus/linubarreceiptinterface_skel.cpp
      QMAKE_CLEAN += menusystem/menus/linubarreceiptinterface_skel.cpp \
      menusystem/menus/linubarreceiptinterface_kidl \
      menusystem/menus/linubarreceiptinterface_stub.cpp \
      menusystem/menus/linubarreceiptinterface_stub.h
    }
  }
}
