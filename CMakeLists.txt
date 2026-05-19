cmake_minimum_required(VERSION 3.16)
project(my_app C)

set(CMAKE_C_STANDARD 11)

add_executable(app src/main.c)

# ----------------------------
# Common compile options
# ----------------------------
target_compile_options(app PRIVATE
    -Os
    -ffunction-sections
    -fdata-sections
)

# ----------------------------
# macOS
# ----------------------------
if(APPLE)

    find_library(COCOA_LIBRARY Cocoa)

    target_include_directories(app PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libui-ng
    )

    target_link_directories(app PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libui-ng/build/meson-out
    )

    target_link_libraries(app PRIVATE
        ui
        ${COCOA_LIBRARY}
    )

endif()

# ----------------------------
# Linux
# ----------------------------
if(UNIX AND NOT APPLE)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtk+-3.0)

    target_include_directories(app PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libui-ng
        ${GTK_INCLUDE_DIRS}
    )

    target_link_directories(app PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libui-ng/build/meson-out
    )

    target_link_libraries(app PRIVATE
        ui
        ${GTK_LIBRARIES}
    )

    target_link_options(app PRIVATE
        -Wl,--gc-sections
        -Wl,--strip-all
    )

endif()
