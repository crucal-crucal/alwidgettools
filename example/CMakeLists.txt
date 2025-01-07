set(TARGET_NAME alexample)

file(GLOB ORIGIN *.hpp *.cpp)
file(GLOB PAGEORIGIN page/*.hpp page/*.cpp)

set(EXAMPLE_SOURCE ${ORIGIN} ${PAGEORIGIN})

include_directories(${CMAKE_SOURCE_DIR}/src/include/public)

add_executable(${TARGET_NAME} ${EXAMPLE_SOURCE})
target_link_libraries(${TARGET_NAME} Qt5::Widgets Qt5::Svg)
target_link_libraries(${TARGET_NAME} alwidgettools)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying alwidgettools.rcc file from ${CMAKE_SOURCE_DIR}/resource/alwidgettools.rcc to $<TARGET_FILE_DIR:${TARGET_NAME}>/alwidgettools.rcc"
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resource/alwidgettools.rcc
        $<TARGET_FILE_DIR:${TARGET_NAME}>/alwidgettools.rcc
)
