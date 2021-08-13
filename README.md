# substrings
## Solution Algorithm
The solution uses a multi-threaded divide-and-conquer approach with all threads sharing a trie to find substring matches across threads. The trie is constructed with the set of common words. Once constructed the trie is kept constant to allow it to be shared across all threads without any possibility of data races.

Once the trie is constructed, space is allocated in a vector for each of the words in allwords.txt. The words are then divided evenly across the number of threads. (e.g. for 7 total words and 2 threads, thread 0 gets words 0-3, thread 1 gets words 4-6). This is implemented in 

Each thread loops through its set of assigned words and finds all possible matches within the trie. The algorithm for finding all matches within a single word uses a doubly-linked list of trie nodes to keep track of all running matches. This is implemented in `trie.cpp` in `trie_impl::find_matches`.

### Usage
```
./substrings-client.exe <all-words-file> <common-words-file> [num_threads]
```
num\_threads:
    optional argument for specifying number of additional threads to use in computation. If no value is specified `std::thread::hardware_concurrency` is used, which returns the number of available hardware threads.

### Build Dependencies
- Tools
    - [CMake (3.18 or higher)](https://cmake.org/download/)
    - [Visual Studio 16 2019](https://visualstudio.microsoft.com/downloads/) or similar IDE/compiler that supports C++17
- Libraries
    - googletest (included in source, for building unit test executable)

### Building on Windows w/ Visual Studio 2019
The easiest way to build on Windows is to install CMake and then invoke the pre-configured batch file `generate_vs2019.bat`. The script should invoke CMake to generate the appropriate Visual Studio solution files in the `build/` folder.

Alternatively, you can run `generate_vs2019_skip_tests.bat` to generate build files for only the core solution executable without any unit tests. This mode does not consume googletest as a dependency.

### Building on Windows w/ older Visual Studio
If you have a different VS version installed, you can check supported VS generators on cmake by running `cmake --help`. Once you have the appropriate generator, invoke the following cmake command from the root directory:
```
cmake -G <generator-name> -B build/

e.g.:

cmake -G "Visual Studio 16 2019" -B build/
```
Once cmake has generated the solution files, you can then go to the `build/` folder and open the generate solution (.sln) file using Visual Studio and build `substrings-client` to generate the main substrings solution executable.

All compiled binaries will be put in the ROOT/bin/Debug folder for debug bulds and ROOT/bin/Release folder for release.

### Build Targets
- substrings-lib
    - static library containing implementation of substring matching
- substrings-client
    - command-line executable for file I/O and invoking substring matching

- gtest
    - googletest framework compiled statically and consumed by substrings-test
- substrings-test
    - unit test executable

### File Structure
#### Source Code
            
- ROOT/include
    - public headers for substring matching implementation
    - substrings.hpp : public interface containing the `find_matches` function and return type
    - substrings-common.hpp : common logging macros
- ROOT/src/substrings-lib
    - code for substring matching implementation
    - trie.{hpp,cpp} : implementation of trie for matching substrings
    - substrings.cpp : implements `find_matches` function
- ROOT/src/substrings-client
    - code for frontend command-line executable
- ROOT/test/substrings-test
    - unit tests

#### Generated Artifacts
- ROOT/bin/{Debug|Release}
    - output binaries (executables and libs) are put here

- ROOT/build
    - solution files are generated here after invoking CMake

#### External Deps
- ROOT/external/googletest
    - source copy of google test library

```
ROOT
|   CMakeLists.txt
|   generate_vs2019.bat
|   generate_vs2019_skip_tests.bat
|   LICENSE
|   README.md
+---external
|   \---googletest
|               
+---include
|       substrings-common.hpp
|       substrings.hpp
|       
+---src
|   +---substrings-client
|   |       CMakeLists.txt
|   |       main.cpp
|   |       
|   \---substrings-lib
|           CMakeLists.txt
|           substrings.cpp
|           trie.cpp
|           trie.hpp
|           
\---test
    \---substrings-test
            CMakeLists.txt
            unit-tests.cpp
```
