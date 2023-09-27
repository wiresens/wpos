unix{
  !exists($$DCOPIDL){
    error("dcopidl not found")
  }
  !exists($$DCOPIDL2CPP){
    error("dcopidl2cpp not found")
  }
  message("Using dcopidl and dcopidl2cpp to create productmoduleinterface skel"){
    system(rm -f bslenterthematrixinterface_skel.cpp)
    system(dcopidl productsmodule/productmoduleinterface.h > productsmodule/productmoduleinterface.kidl)
    system(dcopidl2cpp productsmodule/productmoduleinterface.kidl){
      SOURCES += productsmodule/productmoduleinterface_skel.cpp
      QMAKE_CLEAN += productsmodule/productmoduleinterface_skel.cpp \
      productsmodule/productmoduleinterface.kidl \
      productsmodule/productmoduleinterface_stub.h \
      productsmodule/productmoduleinterface_stub.cpp
    }
  }
  message("Using dcopidl and dcopidl2cpp to create productoptionmoduleinterface skel"){
    system(rm -f productsmodule/optionsmodule/productoptionmoduleinterface_skel.cpp)
    system(dcopidl productsmodule/optionsmodule/productoptionmoduleinterface.h > productsmodule/optionsmodule/productoptionmoduleinterface.kidl)
    system(dcopidl2cpp productsmodule/optionsmodule/productoptionmoduleinterface.kidl){
      SOURCES += productsmodule/optionsmodule/productoptionmoduleinterface_skel.cpp
      QMAKE_CLEAN += productsmodule/optionsmodule/productoptionmoduleinterface_skel.cpp \
      productsmodule/optionsmodule/productoptionmoduleinterface.kidl \
      productsmodule/optionsmodule/productoptionmoduleinterface_stub.h \
      productsmodule/optionsmodule/productoptionmoduleinterface_stub.cpp
    }
  }
  message("Using dcopidl and dcopidl2cpp to create productoffermoduleinterface skel"){
    system(rm -f productsmodule/offersmodule/productoffermoduleinterface_skel.cpp)
    system(dcopidl productsmodule/offersmodule/productoffermoduleinterface.h > productsmodule/offersmodule/productoffermoduleinterface.kidl)
    system(dcopidl2cpp productsmodule/offersmodule/productoffermoduleinterface.kidl){
      SOURCES += productsmodule/offersmodule/productoffermoduleinterface_skel.cpp
      QMAKE_CLEAN += productsmodule/offersmodule/productoffermoduleinterface_skel.cpp \
      productsmodule/offersmodule/productoffermoduleinterface.kidl \
      productsmodule/offersmodule/productoffermoduleinterface_stub.h \
      productsmodule/offersmodule/productoffermoduleinterface_stub.cpp
    }
  }
  message("Using dcopidl and dcopidl2cpp to create usermoduleinterface skel"){
    system(rm -f usersmodule/usermoduleinterface_skel.cpp)
    system(dcopidl usersmodule/usermoduleinterface.h > usersmodule/usermoduleinterface.kidl)
    system(dcopidl2cpp usersmodule/usermoduleinterface.kidl){
      SOURCES += usersmodule/usermoduleinterface_skel.cpp
      QMAKE_CLEAN += usersmodule/usermoduleinterface_skel.cpp \
      usersmodule/usermoduleinterface.kidl \
      usersmodule/usermoduleinterface_stub_stub.h \
      usersmodule/usermoduleinterface_stub_stub.cpp
    }
  }
  message("Using dcopidl and dcopidl2cpp to create nprofitreportwidgetinterface skel"){
    system(rm -f menusystem/reports/nprofitreportwidgetinterface_skel.cpp)
    system(dcopidl menusystem/reports/nprofitreportwidgetinterface.h > menusystem/reports/nprofitreportwidgetinterface.kidl)
    system(dcopidl2cpp menusystem/reports/nprofitreportwidgetinterface.kidl){
      SOURCES += menusystem/reports/nprofitreportwidgetinterface_skel.cpp
      QMAKE_CLEAN += menusystem/reports/nprofitreportwidgetinterface_skel.cpp \
      menusystem/reports/nprofitreportwidgetinterface.kidl \
      menusystem/reports/nprofitreportwidgetinterface_stub.h \
      menusystem/reports/nprofitreportwidgetinterface_stub.cpp
    }
  }
  message("Using dcopidl and dcopidl2cpp to create memstickinterface skel"){
    system(rm -f menusystem/system/memstickinterface_skel.cpp)
    system(dcopidl menusystem/system/memstickinterface.h > menusystem/system/memstickinterface.kidl)
    system(dcopidl2cpp menusystem/system/memstickinterface.kidl){
      SOURCES += menusystem/system/memstickinterface_skel.cpp
      QMAKE_CLEAN += menusystem/system/memstickinterface_skel.cpp \
      menusystem/system/memstickinterface.kidl \
      menusystem/system/memstickinterface._stub.h \
      menusystem/system/memstickinterface_stub.cpp
    }
  }
  message("Using dcopidl and dcopidl2cpp to create optioninterface skel"){
    system(rm -f menusystem/offers_options/optioninterface_skel.cpp)
    system(dcopidl menusystem/offers_options/optioninterface.h > menusystem/offers_options/optioninterface.kidl)
    system(dcopidl2cpp menusystem/offers_options/optioninterface.kidl){
      SOURCES += menusystem/offers_options/optioninterface_skel.cpp
      QMAKE_CLEAN += menusystem/offers_options/optioninterface_skel.cpp \
      menusystem/offers_options/optioninterface.kidl \
      menusystem/offers_options/optioninterface_stub.h \
      menusystem/offers_options/optioninterface_stub.cpp
    }
  }
  message("Using dcopidl and dcopidl2cpp to create databasemoduleinterface skel"){
    system(rm -f databasemodule/databasemoduleinterface_skel.cpp)
    system(dcopidl databasemodule/databasemoduleinterface.h > databasemodule/databasemoduleinterface.kidl)
    system(dcopidl2cpp databasemodule/databasemoduleinterface.kidl){
      SOURCES += databasemodule/databasemoduleinterface_skel.cpp
      QMAKE_CLEAN += databasemodule/databasemoduleinterface_skel.cpp \
      databasemodule/databasemoduleinterface.kidl \
      databasemodule/databasemoduleinterface_stub.h \
      databasemodule/databasemoduleinterface_stub.cpp
    }
  }
}