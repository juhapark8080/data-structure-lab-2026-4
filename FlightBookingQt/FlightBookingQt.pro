QT += widgets
CONFIG += c++17

TARGET = FlightBookingQt
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/flightgraph.cpp \
    src/mapwidget.cpp

HEADERS += \
    src/mainwindow.h \
    src/flightgraph.h \
    src/mapwidget.h
