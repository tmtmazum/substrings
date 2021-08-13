# substrings
### Build Dependencies
- Tools
    - [CMake (3.18 or higher)](https://cmake.org/download/)
    - [Visual Studio 16 2019](https://visualstudio.microsoft.com/downloads/)
- Libraries
    - googletest (included in source, for building unit test executable)

### Building on Windows w/ Visual Studio 2019
The easiest way to build on Windows is to install CMake and then invoke the pre-configured batch file `generate_vs2019.bat`. The script should invoke CMake to generate the appropriate Visual Studio solution files in the `build/` folder. You can then go to the `build/` folder and open the generate solution (.sln) file using Visual Studio and build.

As an alternate option, you can run `generate_vs2019_skip_tests.bat` to generate build files for only the core solution executable without any unit tests. This mode does not consume googletest as a dependency.

You can also manually invoke CMake to generate build files as follows:
```
cmake -G "Visual Studio 16 2019" -B build/
```

All compiled binaries will be put in the ROOT/bin/Debug folder for debug bulds and ROOT/bin/Release folder for release.


### Usage
```
./substrings-client <all-words-file> <common-words-file> [num_threads]
```
num_threads:
    optional argument for specifying number of additional threads to use in computation. If no value is specified `std::thread::hardware_concurrency` is used, which returns the number of available hardware threads.


### Build Targets
- substrings-lib
    - static library containing implementation of substring matching
- substrings-client
    - command-line executable for file I/O and invoking substring matching

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

### Solution Algorithm
The solution uses a multi-threaded divide-and-conquer approach with all threads sharing a trie to find substring matches across threads. The trie is constructed with the set of common words. Once constructed the trie is kept constant to allow it to be shared across all threads without any possibility of data races.

Once the trie is constructed, space is allocated in a vector for each of the words in allwords.txt. The words are then divided evenly across the number of threads. (e.g. for 7 total words and 2 threads, thread 0 gets words 0-3, thread 1 gets words 4-6).

Each thread loops through its set of assigned words and finds all possible matches within the trie. The algorithm for finding all matches within a single word uses a doubly-linked list of trie nodes to keep track of all running matches.
