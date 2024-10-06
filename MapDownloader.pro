QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/FileDownloader.cpp \
    src/UDownloadProcess.cpp \
    src/UTileCalculate.cpp \
    src/main.cpp \
    src/MainWindow.cpp

HEADERS += \
    src/FileDownloader.h \
    src/MainWindow.h \
    src/UDownloadProcess.h \
    src/UTileCalculate.h

FORMS += \
    src/MainWindow.ui

INCLUDEPATH += \
    src

#LIBS += -lcurl

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
