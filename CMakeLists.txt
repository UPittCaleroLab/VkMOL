cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(vkmol VERSION 0.1)

include(GNUInstallDirs)

find_package(Vulkan REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(src/vkmol)
add_subdirectory(src/demo)

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)