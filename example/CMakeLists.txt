set(TARGET_NAME alexample)

file(GLOB ORIGIN *.hpp *.cpp)
file(GLOB PAGEORIGIN page/*.hpp page/*.cpp)
file(GLOB MODELVIEWORIGIN modelview/*.hpp modelview/*.cpp)

set(EXAMPLE_SOURCE ${ORIGIN} ${PAGEORIGIN} ${MODELVIEWORIGIN})

include_directories(page)
include_directories(modelview)
include_directories(${CMAKE_SOURCE_DIR}/src/include/public/al)
include_directories(${CMAKE_SOURCE_DIR}/src/include/public/core)
include_directories(${CMAKE_SOURCE_DIR}/src/include/public/dialogs)
include_directories(${CMAKE_SOURCE_DIR}/src/include/public/theme)
include_directories(${CMAKE_SOURCE_DIR}/src/include/public/widgets)

add_executable(${TARGET_NAME} ${EXAMPLE_SOURCE})
target_link_libraries(${TARGET_NAME} Qt5::Widgets)
target_link_libraries(${TARGET_NAME} alwidgettools)
# Qt Release版本中如果需要定位到日志文件，需要添加此宏
add_definitions(-DQT_MESSAGELOGCONTEXT)

set_target_properties(${TARGET_NAME} PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE FALSE
)

if (WIN32)
    set(RESOURCE_SUFFIX ".dll")
else ()
    set(RESOURCE_SUFFIX ".so")
endif ()

set(RESOURCE_FILES
        "${CMAKE_SOURCE_DIR}/resource/libalwidgettoolsresource${RESOURCE_SUFFIX}"
        "${CMAKE_SOURCE_DIR}/example/resource/example.rcc"
)

foreach(RESOURCE_FILE ${RESOURCE_FILES})
    get_filename_component(FILENAME ${RESOURCE_FILE} NAME)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Copying ${FILENAME} to $<TARGET_FILE_DIR:${TARGET_NAME}>/${FILENAME}"
            COMMAND ${CMAKE_COMMAND} -E copy ${RESOURCE_FILE} $<TARGET_FILE_DIR:${TARGET_NAME}>/${FILENAME}
    )
endforeach()

