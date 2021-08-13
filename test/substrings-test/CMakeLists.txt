project(substrings-test)

file(GLOB ss_ut_src *.cpp)

add_executable(substrings-test ${ss_ut_src})
target_link_libraries(substrings-test substrings-lib gtest)

# use static CRT
set_property(TARGET substrings-test PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
