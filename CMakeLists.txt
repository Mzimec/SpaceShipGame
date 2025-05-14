cmake_minimum_required(VERSION 3.20)
project(gravity_lib)
# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



add_executable(GravityLib program.cpp)

# Include also other directories recursively
add_subdirectory(object)
add_subdirectory(space)
add_subdirectory(display)

file(COPY "${CMAKE_CURRENT_LIST_DIR}/begok.ttf" DESTINATION "${CMAKE_BINARY_DIR}")


# Propagate dependencies
target_include_directories(GravityLib
  PRIVATE
    $<TARGET_PROPERTY:display,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(GravityLib
  PRIVATE
    display
)


