set(TARGET_NAME alwidgettools)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
al_initialize_build_flags()

option(CALWIDGETTOOLS_BUILD_STATIC_LIB "Build static library." OFF)

file(GLOB ORIGIN *.hpp *.cpp)
file(GLOB INCLUDE include/public/*.hpp)
file(GLOB PRIVATE_INCLUDE include/private/*.hpp)
file(GLOB COMPONENTS components/*.hpp components/*.cpp)

source_group(include FILES ${INCLUDE})
source_group(private FILES ${PRIVATE_INCLUDE})
source_group(components FILES ${COMPONENTS})

set(SOURCES ${ORIGIN} ${INCLUDE} ${PRIVATE_INCLUDE} ${COMPONENTS})

if (CALWIDGETTOOLS_BUILD_STATIC_LIB)
    set(LIB_TYPE STATIC)
else ()
    set(LIB_TYPE SHARED)
endif ()

include_directories(include/public)
include_directories(include/private)
include_directories(components)

add_library(${TARGET_NAME} ${LIB_TYPE}
        include/public/alwidgettoolsdef.hpp
        ${SOURCES}
)

file(GLOB EXPORT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/public/*.hpp)
set(EXPORT_RESOURCE_FILE ${CMAKE_SOURCE_DIR}/resource/alwidgettools.rcc)

target_include_directories(${TARGET_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/components>
        $<INSTALL_INTERFACE:include>
)

if (MINGW)
    set_target_properties(${TARGET_NAME} PROPERTIES PREFIX "")
elseif (MSVC)
    set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "d")
endif ()

if (WIN32)
    target_link_libraries(${TARGET_NAME} PUBLIC
            Qt5::Widgets
            D3D11
            DXGI
    )
else ()
    target_link_libraries(${TARGET_NAME} PUBLIC
            Qt5::Widgets
    )
endif ()

target_compile_definitions(${TARGET_NAME} PRIVATE CALWIDGETTOOLS_LIB)

install(TARGETS ${TARGET_NAME}
        EXPORT ${TARGET_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

install(FILES ${EXPORT_HEADERS}
        DESTINATION include
)

install(FILES ${EXPORT_RESOURCE_FILE}
        DESTINATION bin
)

install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION ${CMAKE_BINARY_DIR}
)