cmake_minimum_required(VERSION 3.24)
project(gametest C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_MACOSX_BUNDLE TRUE)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(gametest main.c arial.ttf AppIcon.icns)
set(RESOURCE_FILES arial.ttf AppIcon.icns)
target_link_libraries(gametest ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf)

set(MACOSX_BUNDLE_ICON_FILE AppIcon.icns)
set_target_properties(gametest PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_FRAMEWORK_IDENTIFIER ch.yiu.Counter
        RESOURCE "${RESOURCE_FILES}")