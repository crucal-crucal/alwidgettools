# alwidgettools

# Usage method
You need to copy all directories in the install directory to your project

Suppose you copy to the `3rdparty` directory in the root directory of your project

example
```angular2html
set(ALWIDGETTOOLS_PATH ${CMAKE_SOURCE_DIR}/3rdparty/alwidgettools)

find_package(alwidgettools REQUIRED)
```
The following statements must be executed after the executable file or library generates the statement
```angular2html
if (alwidgettools_FOUND)
    message(STATUS "Found alwidgettools: ${alwidgettools_LIBRARIES}")
    message(STATUS ${alwidgettools_INCLUDE_DIRS})
    include_directories(${alwidgettools_INCLUDE_DIRS})
    target_link_directories(${PROJECT_NAME} PUBLIC ${alwidgettools_LIBRARY_DIRS})
    target_include_directories(${PROJECT_NAME} PUBLIC ${alwidgettools_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${alwidgettools_LIBRARIES})
endif ()
```
add copy the dll file to your execute directory
```angular2html
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${ALWIDGETTOOLS_PATH}/bin/libalwidgettoolsresource.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>

        COMMAND ${CMAKE_COMMAND} -E copy
        ${ALWIDGETTOOLS_PATH}/bin/alwidgettools.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
```
or
```angular2html
FILE(GLOB BIN_FILES  ${ALWIDGETTOOLS_BIN_DIRS}/*)
FILE(COPY ${BIN_FILES} DESTINATION ${CMAKE_BINARY_DIR})
```

# Complete example
```angular2html
cmake_minimum_required(VERSION 3.16)
project(example)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)

if (WIN32 OR UNIX)
    set(QT_DIR $ENV{QTDIR})
    message(STATUS "QT_DIR=${QT_DIR}")
    find_program(QMAKE_EXECUTABLENAMES qmake HINTS $ENV{QT_DIR} PATH_SUFFIXES bin)
    set(CMAKE_PREFIX_PATH ${QT_DIR})
endif ()

# Query the Qt version and store it in QT_VERSION
execute_process(COMMAND ${QMAKE_EXECUTABLENAMES} -query QT_VERSION OUTPUT_VARIABLE QT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
if (QT_VERSION)
    message(STATUS "Qt version: " ${QT_VERSION})
    string(REPLACE ".." ";" QT_VERSION_LIST ${QT_VERSION})
    list(GET QT_VERSION_LIST 0 QT_VERSION_MAJOR)
else ()
    message(FATAL_ERROR "Qt version not found. Please check if the QTDIR environment variable is set correctly.")
endif ()

include_directories(${QT_DIR}/include)

if (QT_VERSION_MAJOR VERSION_GREATER_EQUAL 5)
    message(STATUS "Qt5 found.")
    find_package(Qt5
            COMPONENTS
            Widgets
            REQUIRED
    )
else ()
    message(FATAL_ERROR "this project requires Qt5. Please install Qt5 and try again.")
endif ()

#add_definitions(-DQT_NO_DEBUG_OUTPUT)
#add_definitions(-DQT_NO_INFO_OUTPUT)
#add_definitions(-DQT_NO_WARNING_OUTPUT)
#add_definitions(-DQT_NO_CRITICAL_OUTPUT)
#add_definitions(-DQT_NO_FATAL_OUTPUT)

set(ALWIDGETTOOLS_PATH ${CMAKE_SOURCE_DIR}/3rdparty/alwidgettools)

list(APPEND CMAKE_PREFIX_PATH ${ALWIDGETTOOLS_PATH}/cmake)
find_package(alwidgettools REQUIRED)

set(examples_SOURCES
    mainwidget.hpp
    mainwidget.cc
)

add_executable(${TARGET_NAME} main.cc ${examples_SOURCES})

if (alwidgettools_FOUND)
    message(STATUS "alwidgettools version: ${ALWIDGETTOOLS_VERSION}")
    include_directories(${ALWIDGETTOOLS_INCLUDE_DIRS})
    target_link_directories(${PROJECT_NAME} PUBLIC ${ALWIDGETTOOLS_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${ALWIDGETTOOLS_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${ALWIDGETTOOLS_LIBRARIES})
endif ()


target_link_libraries(${TARGET_NAME} Qt5::Widgets)

install(TARGETS ${TARGET_NAME}
    RUNTIME DESTINATION bin
)

#copy rcc file to execute directory
file(GLOB NECESSARY_FILES ${ALWIDGETTOOLS_BIN_DIRS}/* ${ALWIDGETTOOLS_LIBRARY_DIRS})
file(COPY ${NECESSARY_FILES} DESTINATION ${CMAKE_BINARY_DIR})

set_target_properties(${TARGET_NAME} PROPERTIES
    WIN32_EXECUTABLE FALSE
)
```