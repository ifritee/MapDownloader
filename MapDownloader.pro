QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    src/FileDownloader.cpp \
    src/MapAdapterLocal.cpp \
    src/UDownloadProcess.cpp \
    src/UTileCalculate.cpp \
    src/WGetCoordinates.cpp \
    src/main.cpp \
    src/MainWindow.cpp

HEADERS += \
    src/FileDownloader.h \
    src/MainWindow.h \
    src/MapAdapterLocal.h \
    src/UDownloadProcess.h \
    src/UTileCalculate.h \
    src/WGetCoordinates.h

FORMS += \
    src/MainWindow.ui

INCLUDEPATH += \
    src \
    third_party

CONFIG(debug, debug|release) {
LIBS += \
    -L$$PWD/lib -lqmapcontrold
}

CONFIG(release, debug|release) {
LIBS += \
    -L$$PWD/lib -lqmapcontrol
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
  res/icons.qrc
