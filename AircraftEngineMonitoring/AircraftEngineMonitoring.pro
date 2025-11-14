QT       += core gui serialport 

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bufferprocessor.cpp \
    circulargauge.cpp \
    main.cpp \
    mainwindow.cpp \
    message.cpp \
    monitoring.cpp \
    qgaugewidget.cpp

HEADERS += \
    bufferprocessor.h \
    circulargauge.h \
    mainwindow.h \
    message.h \
    monitoring.h \
    qgaugewidget.h

FORMS += \
    mainwindow.ui \
    monitoring.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# QXlsx code for Application Qt project
QXLSX_PARENTPATH=./         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./source/  # current QXlsx source path is ./source/
include(./QXlsx.pri)