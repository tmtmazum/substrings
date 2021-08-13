project(substrings-client)

file(GLOB ss_client_src *.cpp)

add_executable(substrings-client ${ss_client_src})
target_link_libraries(substrings-client substrings-lib)

# use static CRT
set_property(TARGET substrings-client PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
