project(substrings-lib)

file(GLOB ss_lib_src *.cpp)
file(GLOB ss_lib_h *.hpp ../../include/*.hpp)

add_library(substrings-lib STATIC ${ss_lib_src} ${ss_lib_h})
target_compile_features(substrings-lib PUBLIC cxx_std_17)
set_property(TARGET substrings-lib PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
