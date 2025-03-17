add_executable(testServer testServer.cpp)
add_executable(testClient testClient.cpp)

include_directories(
  ${CMAKE_SOURCE_DIR}/src/include
)

target_link_libraries(testServer reactor)

target_include_directories(testServer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../src/include)