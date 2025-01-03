set(TARGET_NAME uvexample)

file(GLOB ORIGIN *.hpp *.cpp)

set(EXAMPLE_SOURCE ${ORIGIN})

include_directories(${CMAKE_SOURCE_DIR}/src/include/public)

add_executable(${TARGET_NAME} ${EXAMPLE_SOURCE})
target_link_libraries(${TARGET_NAME} uvwidgettools)

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying uvresource.rcc file from ${CMAKE_SOURCE_DIR}/resource/uvresource.rcc to $<TARGET_FILE_DIR:${TARGET_NAME}>/uvresource.rcc"
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resource/uvresource.rcc
        $<TARGET_FILE_DIR:${TARGET_NAME}>/uvresource.rcc
        COMMAND ${CMAKE_COMMAND} -E echo "Copying translation file from ${CMAKE_SOURCE_DIR}/translation/zh_CN.qm to $<TARGET_FILE_DIR:${TARGET_NAME}>/zh_CN.qm"
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/translation/zh_CN.qm
        $<TARGET_FILE_DIR:${TARGET_NAME}>/zh_CN.qm
)
