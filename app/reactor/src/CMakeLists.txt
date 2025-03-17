include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_library(
  reactor
  SHARED
  ${SRC_FILES}
)