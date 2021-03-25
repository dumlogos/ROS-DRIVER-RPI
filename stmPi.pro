QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += qwt


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    UNISTD_CLOSE_OVERRIDE.cpp \
    can_handler.cpp \
    can_receiver.cpp \
    can_transmitter.cpp \
    drivercontroller.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    CAN_ENUM.h \
    UNISTD_CLOSE_OVERRIDE.h \
    can_handler.h \
    can_receiver.h \
    can_struct.h \
    can_transmitter.h \
    drivercontroller.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /home/pi/Qt_Projects/$${TARGET}
!isEmpty(target.path): INSTALLS += target

#unix:!macx: LIBS += -L$$PWD/../../../usr/local/qwt-6.1.6/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../usr/local/qwt-6.1.6/include
DEPENDPATH += $$PWD/../../../usr/local/qwt-6.1.6/include
