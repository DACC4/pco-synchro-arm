# PCOSynchro library

## Description
PcoSynchro is a library of classes implementing the classical synchronization mechanisms. It offers:

- PcoThread
- PcoMutex
- PcoSemaphore
- PcoConditionVariable

There classes are wrappers around the objects found in the standard library, offering a subset of the functionalities. Why a subset? Because we use it within a concurrent programming course, and we want to restrict some usage to guide the students.

Moreover, the library offers a mechanism to add random sleeps in the various methods, allowing to test more deeply some synchronization mechanisms such as the producer-consumer or the reader-writer protocols. A class PcoManager can be used to set specific ranges for the random sleeps for all classes or for each specific method.

The library is open source, with a LGPL license.

Some tests based on GoogleTest are implemented, so as to test the correct behavior of the various classes, and can serve as examples of a way to create tests checking for deadlocks for instance.

Author: Yann Thoma

## Installation
Run the following commands in the `lib/pcosynchro` folder :
```bash
export LD_PATH="/usr/local/lib"
export LIBRARY_PATH="/usr/local/lib"
qmake

sed -i .old 's/gnu++11/c++17/g' Makefile
sudo make install
```

## Create a new Clion project
### CMakeLists.txt
**Replace the projet name and source with your project's infos**
```cmake
cmake_minimum_required(VERSION 3.16)
project(PCO_Labo_2 VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

find_package(Qt6 COMPONENTS Core Widgets Core5Compat REQUIRED)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent Gui)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

INCLUDE_DIRECTORIES(/usr/local/include)
LINK_DIRECTORIES(/usr/local/lib)

add_executable(PCO_Labo_2 WIN32 MACOSX_BUNDLE
        src/main.cpp
        src/mainwindow.cpp src/mainwindow.h
        src/mythread.cpp src/mythread.h
        src/threadmanager.cpp src/threadmanager.h
        ui/mainwindow.ui
)
target_link_libraries(PCO_Labo_2 PRIVATE
        Qt::Concurrent
        Qt::Core
        Qt::Gui
        Qt6::Core5Compat
        pcosynchro
)

if ((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(PCO_Labo_2 PRIVATE
            Qt::Widgets
    )
endif ()

install(TARGETS PCO_Labo_2
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
```

### Give Clion the QT path
In the `Settings` menu, go to `Build, Execution, Deployment` and then `CMake`. In the `CMake options` field, add the following line:

**Replace the path with your QT installation path**
```
-DCMAKE_PREFIX_PATH=/Users/christophe/Qt/6.5.2/macos/lib/cmake/
```

### Fixing the code :)
Since the PCO code is made for QT5 and we are using QT6, we'll need to fix some things.

## Tests
A qmake project is used to compile the tests, and can be used if you have Qt installed.
Just run `qmake;make` in the test folder, or use the Qt Creator IDE.