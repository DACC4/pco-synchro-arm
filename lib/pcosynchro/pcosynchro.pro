CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++14

SOURCES += \
    ../../src/pcoconditionvariable.cpp \
    ../../src/pcohoaremonitor.cpp \
    ../../src/pcomanager.cpp \
    ../../src/pcomutex.cpp \
    ../../src/pcosemaphore.cpp \
    ../../src/pcothread.cpp

HEADERS += \
    ../../src/pcoconditionvariable.h \
    ../../src/pcohoaremonitor.h \
    ../../src/pcomanager.h \
    ../../src/pcomutex.h \
    ../../src/pcosemaphore.h \
    ../../src/pcothread.h

# Default rules for deployment.
unix {
    includes.path = /usr/local/include/pcosynchro
    includes.files = ../../src/*.h
    INSTALLS    += includes
}
