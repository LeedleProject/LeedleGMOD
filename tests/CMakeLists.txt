include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 26)

add_executable(safe_call_test safe_call_test.cpp)
target_include_directories(safe_call_test PRIVATE "${CMAKE_SOURCE_DIR}/thirdparty/SafeCall")
target_link_libraries(safe_call_test PRIVATE user32)

add_executable(vmt_test vmt_test.cpp "${CMAKE_SOURCE_DIR}/leedle/memory.cpp")
target_include_directories(vmt_test PRIVATE "${CMAKE_SOURCE_DIR}/leedle/")

add_test( safe_call_test safe_call_test )
add_test( vmt_test vmt_test )