add_library(space "")

target_sources(space
  PRIVATE
    "space.cpp"
  PUBLIC
    "space.h"
  )

# Propagate dependencies
target_include_directories(space
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

# Link libraries
target_link_libraries(space
  PUBLIC
    object
)