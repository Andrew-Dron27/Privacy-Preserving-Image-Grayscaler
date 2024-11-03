cmake_minimum_required(VERSION 3.10)
project(Image_Greyscaler)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)

# Find and link to HElib
find_package(helib REQUIRED)

# Link HElib and its dependencies

# Add source files
add_executable(client src/client/client.cpp src/image/network.cpp src/image/image.cpp)
add_executable(server src/server/server.cpp src/image/network.cpp src/image/image.cpp)

target_link_libraries(client helib)
target_link_libraries(server helib)

# Optional: Add compilation flags
target_compile_options(client PRIVATE -Wall -Wextra -std=c++17 -g)
target_compile_options(server PRIVATE -Wall -Wextra -std=c++17 -g)

# Link libraries if needed
# target_link_libraries(my_program PRIVATE some_library)

set_target_properties(client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

set_target_properties(server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

