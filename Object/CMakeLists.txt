add_library(object "")

target_sources(object
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/object.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/object.h
  )


# Find required packages
# set(SFML_DIR "C:/Users/stepa/source/repos/vcpkg/installed/x64-windows/share/sfml")
find_package(SFML COMPONENTS system window graphics REQUIRED)
add_compile_options(-D_WIN32_WINNT=0x0601)

target_include_directories(object
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
    ${SFML_INCLUDE_DIRS}  # Include SFML directories
  )

# Link libraries
target_link_libraries(object PUBLIC sfml-system sfml-network sfml-graphics sfml-window)


