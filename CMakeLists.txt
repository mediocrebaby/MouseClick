cmake_minimum_required(VERSION 3.10)
project(MouseClick)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 添加源文件
set(SOURCES
    src/main.cpp
    src/mouse_click.cpp
)

# 创建可执行文件
add_executable(mouse_click ${SOURCES})

# 根据平台添加所需库
if(WIN32)
    # 添加MSVC编译器的UTF-8编码选项
    if(MSVC)
        target_compile_options(mouse_click PRIVATE "/utf-8")
    elseif(CLANG)
        target_compile_options(mouse_click PRIVATE "-finput-charset=UTF-8")
    endif()
    # Windows平台不需要额外的库
elseif(APPLE)
    find_library(CORE_GRAPHICS CoreGraphics)
    find_library(CORE_FOUNDATION CoreFoundation)
    target_link_libraries(mouse_click ${CORE_GRAPHICS} ${CORE_FOUNDATION})
else()
    # Linux平台
    find_package(X11 REQUIRED)
    target_link_libraries(mouse_click ${X11_LIBRARIES} Xtst)
endif()
