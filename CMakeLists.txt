cmake_minimum_required(VERSION 3.0.0)
project(leedle VERSION 0.1.0)

add_compile_options(-m32)

set(CMAKE_CXX_STANDARD 26)
set(VCPKG_TARGET_TRIPLET "x86-windows-static")
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

add_subdirectory(tests)

find_package(imgui CONFIG REQUIRED)

add_library(leedle SHARED leedle/leedle.cpp leedle/memory.cpp)
target_link_libraries(leedle PRIVATE imgui::imgui)
target_include_directories(leedle PRIVATE "thirdparty/SafeCall" "thirdparty/math/include")

set_property(TARGET leedle PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options(leedle PUBLIC "/ZI")
  target_link_options(leedle PUBLIC "/SAFESEH:NO")
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

