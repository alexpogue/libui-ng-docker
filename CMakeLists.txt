cmake_minimum_required(VERSION 3.16)

project(my_app C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

# If libui-ng is a subdirectory
#add_subdirectory(/app/libui-ng)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

add_executable(app src/main.c)

target_compile_options(app PRIVATE
    -Os
    -ffunction-sections
    -fdata-sections
)

target_link_options(app PRIVATE
    -Wl,--gc-sections
    -Wl,--strip-all
)

#target_link_libraries(app PRIVATE ui)
target_include_directories(app PRIVATE
    /app/libui-ng
    ${GTK_INCLUDE_DIRS}
)

target_link_directories(app PRIVATE /app/libui-ng/build/meson-out)

target_link_libraries(app PRIVATE
    ui
    ${GTK_LIBRARIES}
)
