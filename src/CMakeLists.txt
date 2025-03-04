set(TARGET_NAME alwidgettools)

# Enable exporting all symbols on Windows (useful for DLLs)
# 在 Windows 上启用导出所有符号
if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif ()

al_initialize_build_flags()

# Gather all source and header files
# 收集所有源文件和头文件
file(GLOB ORIGIN
        core/*.cpp
        dialogs/*.cpp
        theme/*.cpp
        widgets/*.cpp
)
file(GLOB PUBLIC_INCLUDE
        include/public/al/*.hpp
        include/public/core/*.hpp
        include/public/dialogs/*.hpp
        include/public/theme/*.hpp
        include/public/widgets/*.hpp
)
file(GLOB PRIVATE_INCLUDE include/private/*.hpp)
file(GLOB COMPONENTS components/*.hpp components/*.cpp)

# Organize the files in Visual Studio source groups (optional for IDE users)
# 在 Visual Studio 中将文件分组（这对使用 IDE 的开发者有帮助）
source_group(include FILES ${PUBLIC_INCLUDE})
source_group(private FILES ${PRIVATE_INCLUDE})
source_group(components FILES ${COMPONENTS})

# Define the installation prefix (where the package will be installed)
# 定义安装路径（安装包将会放在这个路径下）
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "Install path" FORCE)
# Option to choose between static or shared library
# 选择静态库或动态库
option(CALWIDGETTOOLS_BUILD_STATIC_LIB "Build static library." OFF)

# Set the library type based on the option
# 根据选项设置库类型
if (CALWIDGETTOOLS_BUILD_STATIC_LIB)
    set(LIB_TYPE STATIC)
else ()
    set(LIB_TYPE SHARED
            include/private/alicon_p.hpp)
endif ()

# Add include directories for public, private, and component headers
# 设置包含目录（公共、私有和组件头文件）
include_directories(include/public/al)
include_directories(include/public/core)
include_directories(include/public/dialogs)
include_directories(include/public/theme)
include_directories(include/public/widgets)
include_directories(include/private)
include_directories(components)

# Define the library target (either static or shared based on the LIB_TYPE variable)
# 定义库目标（根据 LIB_TYPE 选择是静态库还是动态库）
add_library(${TARGET_NAME} ${LIB_TYPE}
        include/public/al/alwidgettoolsdef.hpp
        ${ORIGIN}
        ${PUBLIC_INCLUDE}
        ${PRIVATE_INCLUDE}
        ${COMPONENTS}
)

# Platform-specific adjustments
if (MINGW)
    set_target_properties(${TARGET_NAME} PROPERTIES PREFIX "")
elseif (MSVC)
    set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "d")
endif ()

# Link necessary libraries based on platform
target_link_libraries(${TARGET_NAME} PUBLIC Qt5::Widgets)
if (WIN32)
    target_link_libraries(${TARGET_NAME} PUBLIC
            D3D11
            DXGI
    )
endif ()
# Define compile-time library definition for the target
target_compile_definitions(${TARGET_NAME} PRIVATE CALWIDGETTOOLS_LIB)

#######  Installation commands  #######
set(EXPORT_RESOURCE_FILE ${CMAKE_SOURCE_DIR}/resource/libalwidgettoolsresource${CMAKE_SHARED_LIBRARY_SUFFIX})

# Set the include directories for the target, with proper interface handling for build and installation
# 设置目标的包含目录，对于安装时和构建时的路径使用不同的变量
target_include_directories(${TARGET_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/public/al>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/public/core>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/public/dialogs>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/public/theme>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/public/widgets>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/private>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/components>
)

# Install the compiled library and resources
# 安装编译后的库和资源文件
install(TARGETS ${TARGET_NAME}
        EXPORT ${TARGET_NAME}-targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

if (WIN32)
    install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}> DESTINATION pdb OPTIONAL)
endif ()

# Install the public headers
# 安装公共头文件
install(FILES ${PUBLIC_INCLUDE} DESTINATION include)

# Install the resource file (e.g., Qt .rcc file)
# 安装资源文件（例如 Qt 的 .rcc 文件
install(FILES ${EXPORT_RESOURCE_FILE}
        DESTINATION bin
)

####### Use CMake's helpers to write package version and config files #######
# 使用 CMake 的帮助工具生成包版本文件和配置文件
include(CMakePackageConfigHelpers)

# Set up package configuration variables for package installation
# 为包安装配置生成必要的变量
set(ALWIDGETTOOLS_INCLUDE_DIRS include CACHE PATH "Include directories")
set(ALWIDGETTOOLS_LIBRARIES ${TARGET_NAME} CACHE STRING "Library target")
set(ALWIDGETTOOLS_LIB_DIR lib CACHE PATH "Library directory")
set(ALWIDGETTOOLS_CMAKE_DIR cmake CACHE PATH "CMake directory")

# Write the basic version file for the package
# 写入包版本文件
write_basic_package_version_file(
        ${CMAKE_BINARY_DIR}/${TARGET_NAME}configversion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

message(STATUS "ALWIDGETTOOLS_INCLUDE_DIRS: ${ALWIDGETTOOLS_INCLUDE_DIRS}")
message(STATUS "ALWIDGETTOOLS_LIBRARIES: ${ALWIDGETTOOLS_LIBRARIES}")
message(STATUS "ALWIDGETTOOLS_LIB_DIR: ${ALWIDGETTOOLS_LIB_DIR}")
message(STATUS "ALWIDGETTOOLS_CMAKE_DIR: ${ALWIDGETTOOLS_CMAKE_DIR}")

# Configure the package config file with paths to the necessary directories
# 配置包的配置文件，传递需要的变量路径
configure_package_config_file(
        ${CMAKE_SOURCE_DIR}/cmake/${TARGET_NAME}config.cmake.in
        ${CMAKE_BINARY_DIR}/${TARGET_NAME}config.cmake
        INSTALL_DESTINATION cmake
        PATH_VARS ALWIDGETTOOLS_INCLUDE_DIRS ALWIDGETTOOLS_LIBRARIES ALWIDGETTOOLS_LIB_DIR ALWIDGETTOOLS_CMAKE_DIR
        INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}
)

# Install the package config files
# 安装包配置文件
install(FILES ${CMAKE_BINARY_DIR}/${TARGET_NAME}config.cmake ${CMAKE_BINARY_DIR}/${TARGET_NAME}configversion.cmake DESTINATION cmake)
