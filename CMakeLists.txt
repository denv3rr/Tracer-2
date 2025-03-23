cmake_minimum_required(VERSION 3.10)

# Export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project name
project(Tracer-2 VERSION 1.0)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Executable target and source files
add_executable(Tracer-2
        src/main.cpp
        src/PathAlgorithms.cpp
        src/menu.cpp
        src/Object.cpp
        src/scenario.cpp
        src/Tracker.cpp
)

# Directories for headers
target_include_directories(Tracer-2 PUBLIC include)
