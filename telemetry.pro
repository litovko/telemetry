TEMPLATE = app

QT += qml quick network
CONFIG += c++11 console

SOURCES += main.cpp \
    tk15.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    tk15.h

DISTFILES += \
    tk15.ico

RC_ICONS = tk15.ico
