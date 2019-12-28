
CONFIG += c++14 console
CONFIG -= app_bundle
CONFIG -= qt

unix {
    LIBS += -lpthread
}

LIBS += -lgtest

SOURCES += \
        ../src/pcomanager.cpp \
        ../src/pcothread.cpp \
        ../src/pcomutex.cpp \
        ../src/pcosemaphore.cpp \
        ../src/pcoconditionvariable.cpp \
        main.cpp

HEADERS += \
    ../src/pcomanager.h \
    ../src/pcothread.h \
    ../src/pcomutex.h \
    ../src/pcosemaphore.h \
    ../src/pcoconditionvariable.h \
    ../src/pcotest.h
