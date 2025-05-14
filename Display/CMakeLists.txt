add_library(display "")

target_sources(display
  PRIVATE
    "display.cpp"
  PUBLIC
    "display.h"
  )

# Propagate dependencies
target_include_directories(display
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

# Link libraries
target_link_libraries(display
  PUBLIC
    space
  )