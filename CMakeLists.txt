cmake_minimum_required(VERSION 3.0.0)
project(leedle VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 26)
set(VCPKG_TARGET_TRIPLET "x86-windows-static")
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

add_subdirectory("thirdparty/PlatformFolders")
add_subdirectory("thirdparty/minhook")

set(LOGURU_INCLUDE_DIRS "thirdparty/loguru")
set(MAGIC_ENUM_INCLUDE_DIRS "thirdparty/magic_enum/include")
set(WIL_INCLUDE_DIRS "thirdparty/wil/include")

add_definitions(-DLOGURU_WITH_STREAMS)
include_directories(${MAGIC_ENUM_INCLUDE_DIRS} ${LOGURU_INCLUDE_DIRS})

add_library(leedle SHARED 
  leedle/leedle.cpp leedle/memory.cpp
  leedle/render.cpp thirdparty/kiero/kiero.cpp
  ${LOGURU_INCLUDE_DIRS}/loguru.cpp thirdparty/imgui_backends/imgui_impl_dx9.cpp
  thirdparty/imgui_backends/imgui_impl_win32.cpp thirdparty/imgui/imgui.cpp thirdparty/imgui/imgui_draw.cpp
  thirdparty/imgui/imgui_tables.cpp thirdparty/imgui/imgui_widgets.cpp thirdparty/imgui/imgui_demo.cpp
  leedle/gui.cpp leedle/input.cpp leedle/hooks/hooks.cpp
)

target_link_libraries(leedle PRIVATE sago::platform_folders minhook)
target_include_directories(leedle PRIVATE "thirdparty/SafeCall" "thirdparty/math/include" 
  "thirdparty/kiero/" ${LOGURU_INCLUDE_DIRS} ${WIL_INCLUDE_DIRS} "thirdparty/utf8.h" "thirdparty/toml11"
  "thirdparty/imgui_backends/" "thirdparty/imgui/" "thirdparty/minhook/include")

set_property(TARGET leedle PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options(leedle PUBLIC "/ZI")
  target_link_options(leedle PUBLIC "/SAFESEH:NO")
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include(CTest)
enable_testing()

add_executable(vmt_test tests/vmt_test.cpp "${CMAKE_SOURCE_DIR}/leedle/memory.cpp")
target_include_directories(vmt_test PRIVATE "${CMAKE_SOURCE_DIR}/leedle/" "thirdparty/minhook/include" ${LOGURU_INCLUDE_DIRS})
target_link_libraries(vmt_test PRIVATE minhook)

add_test( NAME vmt_test COMMAND vmt_test )

add_subdirectory(thirdparty/magic_enum/test)