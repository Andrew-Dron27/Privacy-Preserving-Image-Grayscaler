cmake_minimum_required(VERSION 3.10)
project(Image_Greyscaler)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)

# Find and link to HElib
find_package(helib REQUIRED)

# Link HElib and its dependencies

# Add source files
add_executable(client client.cpp foo.cpp)

# Optional: Add compilation flags
target_compile_options(my_program PRIVATE -Wall -g)

# Link libraries if needed
# target_link_libraries(my_program PRIVATE some_library)

# Custom command for cleaning
add_custom_target(clean
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeClean.cmake
)

