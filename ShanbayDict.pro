TARGET = ShanbayDict
TEMPLATE = app

QT += widgets qml multimedia
CONFIG += c++14

RESOURCES += qml.qrc icons.qrc
RC_FILE = app.rc

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
}else{
    DEFINES += NDEBUG
}
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

INCLUDEPATH += src
SOURCES += \
    src/app/main.cpp \
    src/net/shanbaynet.cpp \
    src/app/application.cpp \
    src/gui/Gui.cpp \
    src/app/config.cpp
HEADERS  += \
    src/net/shanbaynet.h \
    src/app/application.h \
    src/gui/Gui.h \
    src/app/config.h

win32{

}
macx{
    ICON = logo.icns
}
#DEFINES += QT_NO_WARNING_OUTPUT\
#           QT_NO_DEBUG_OUTPUT




































