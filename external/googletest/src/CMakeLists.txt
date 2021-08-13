project(gtest)

file(GLOB gtest_src *.cpp *.cc)

add_library(gtest STATIC ${gtest_src})
set_property(TARGET gtest PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
