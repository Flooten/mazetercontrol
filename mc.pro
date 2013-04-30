include(libs/qextserialport-1.2rc/src/qextserialport.pri)

QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mc
TEMPLATE = app

INCLUDEPATH += include/

HEADERS += \
    include/utils.h \
    include/userinput.h \
    include/terminal.h \
    include/serialport.h \
    include/control.h \
    include/commandtable.h \
    include/mainwindow.h \
    include/controlsignals.h \
    include/preferencesdialog.h \
    include/mcgraphicsscene.h \
    include/sensordata.h \
    include/aboutdialog.h \
    include/controlsignalsplotscene.h \
    include/sensordataplotscene.h \
    include/plotscene.h \
    include/xmlcontrol.h

SOURCES += \
    src/utils.cc \
    src/userinput.cc \
    src/terminal.cc \
    src/serialport.cc \
    src/main.cc \
    src/control.cc \
    src/mainwindow.cc \
    src/preferencesdialog.cc \
    src/mcgraphicsscene.cc \
    src/aboutdialog.cc \
    src/controlsignalsplotscene.cc \
    src/sensordataplotscene.cc \
    src/plotscene.cc \
    src/xmlcontrol.cc

FORMS += \
    forms/terminal.ui \
    forms/mainwindow.ui \
    forms/preferencesdialog.ui \
    forms/aboutdialog.ui

win32{
QMAKE_CXXFLAGS += -std=c++11
}

unix{
QMAKE_CXXFLAGS += -std=c++11
}

macx{
QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++ -mmacosx-version-min=10.7
LIBS += -stdlib=libc++ -mmacosx-version-min=10.7

APP_RSC_FILES.files = data/mc.ini data/.mc_history
APP_RSC_FILES.path = Contents/MacOS/Data
QMAKE_BUNDLE_DATA += APP_RSC_FILES
}

RC_FILE += icon.rc

RESOURCES += \
    resources.qrc
