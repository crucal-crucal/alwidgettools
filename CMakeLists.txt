cmake_minimum_required(VERSION 3.16)
project(alwidgettools VERSION 0.1 LANGUAGES CXX)

string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

include(${CMAKE_SOURCE_DIR}/cmake/alsetup.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/alconfigmacros.cmake)

al_initialize_build_flags()
al_setup_Qt()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(example)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.alwidgettools)
endif ()
