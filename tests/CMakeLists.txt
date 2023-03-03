
add_executable(safe_call_test safe_call_test.cpp)
target_include_directories(safe_call_test PRIVATE "${CMAKE_SOURCE_DIR}/thirdparty/SafeCall")
target_link_libraries(safe_call_test PRIVATE user32)

add_test(NAME safe_call_test COMMAND safe_call_test)