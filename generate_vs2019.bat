REM clearing existing project files
rmdir /S build/
REM generating new project files
cmake -G "Visual Studio 16 2019" -B build/
