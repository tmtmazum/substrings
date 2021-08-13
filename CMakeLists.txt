cmake_minimum_required(VERSION 3.18)
project(substrings)

## Build Properties
if (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /WX")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT /Zi /WX")
endif()

## Output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

## -- end of build properties

include_directories(include)

add_subdirectory(src/substrings-lib)
add_subdirectory(src/substrings-client)

if (NOT DEFINED CACHE{skip-tests})
    include_directories(external/googletest/include)
    include_directories(external/googletest)
    add_subdirectory(external/googletest/src)
    add_subdirectory(test/substrings-test)
endif()
