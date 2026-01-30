cmake_minimum_required(VERSION 3.10)
project(MoteurMorphologiqueArabe)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/AVL.cpp
    src/TableSchemes.cpp
    src/Morphologie.cpp
)

# Include directories
include_directories(src)

# Create executable
add_executable(morphologie ${SOURCES})

# Optional: Add tests subdirectory if CMake tests are needed
# enable_testing()
# add_subdirectory(tests)
