include(libs/qextserialport-1.2rc/src/qextserialport.pri)

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mc
TEMPLATE = app

HEADERS += \
    utils.h \
    userinput.h \
    terminal.h \
    serialport.h \
    control.h \
    commandtable.h \
    mainwindow.h \
    controlsignals.h \
    preferencesdialog.h \
    mcgraphicsscene.h \
    sensordata.h \
    aboutdialog.h \
    controlsignalsplotscene.h \
    sensordataplotscene.h

SOURCES += \
    utils.cc \
    userinput.cc \
    terminal.cc \
    serialport.cc \
    main.cc \
    control.cc \
    mainwindow.cc \
    preferencesdialog.cc \
    mcgraphicsscene.cc \
    aboutdialog.cc \
    controlsignalsplotscene.cc \
    sensordataplotscene.cc

FORMS += \
    terminal.ui \
    mainwindow.ui \
    preferencesdialog.ui \
    aboutdialog.ui

win32{
QMAKE_CXXFLAGS += -std=c++11
}

unix{
QMAKE_CXXFLAGS += -std=c++11
}

RC_FILE += icon.rc

RESOURCES += \
    resources.qrc
