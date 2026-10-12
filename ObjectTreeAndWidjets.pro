greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += core gui widgets

CONFIG   += console c++17
CONFIG   -= app_bundle

TARGET = ObjectTreeDemo
TEMPLATE = app

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h \
    widgets.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
