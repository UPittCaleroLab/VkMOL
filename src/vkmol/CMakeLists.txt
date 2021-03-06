project(libvkmol VERSION 0.1)

if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-fcolor-diagnostics)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        add_compile_options(-fcolor-diagnostics)
    endif ()
endif ()

add_library(vkmol SHARED
    src/renderer/Buffer.cpp
    src/renderer/Debug.cpp
    src/renderer/Renderer.cpp
    src/renderer/Resource.cpp
    src/renderer/Allocator.cpp include/vkmol/renderer/UploadOp.h src/renderer/UploadOp.cpp)

target_include_directories(vkmol
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:include>
    PRIVATE src)

target_compile_features(vkmol
    PUBLIC cxx_std_17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

# The following are set due to vk_mem_alloc.h:
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-zero-variadic-macro-arguments")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable -Wno-unused-parameter")

include_directories(${CCP4_INCLUDE_DIRS})

target_link_libraries(vkmol
    Vulkan::Vulkan
    ${CCP4_LIBRARIES})

if (APPLE)
    target_link_libraries(vkmol MoltenVK)
endif ()

install(TARGETS vkmol EXPORT VkMOLConfig
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY share/ DESTINATION ${CMAKE_INSTALL_DATAROOTDIR})

install(EXPORT VkMOLConfig DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake)

export(TARGETS vkmol FILE VkMOLConfig.cmake)

# TODO : Add tests.

# add_executable(testlib
# test / testlib.cpp)

# target_link_libraries(testlib
# lib)

# add_test(testlib testlib)