QT       += quick qml network positioning
QT       += core gui charts sql webkit webkitwidgets quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#QML_IMPORT_PATH += usr/lib/x86_64-linux-gnu/

TARGET = chart

CONFIG += c++11

QTPLUGIN += QPSQL
QT_DEBUG_PLUGINS=1

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calcwindow.cpp \
    changedatawindow.cpp \
    filterandgraphics.cpp \
    main.cpp \
    mainwindow.cpp \
    mapworld.cpp \
    myiostreamfile.cpp \
    outputwindow.cpp \
    requestSql.cpp \
    settingdbwindow.cpp \
    storage.cpp

HEADERS += \
    MarkerModel.h \
    calcwindow.h \
    changedatawindow.h \
    connection.h \
    filterandgraphics.h \
    mainwindow.h \
    mapworld.h \
    myiostreamfile.h \
    outputwindow.h \
    requestSql.h \
    settingdbwindow.h \
    storage.h

FORMS += \
    avalancheRiskCalc.ui \
    changedatawindow.ui \
    filterandgraphics.ui \
    mainwindow.ui \
    mapworld.ui \
    outputwindow.ui \
    settingdbwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    textRead


