find_package(OpenSSL REQUIRED)

aux_source_directory(. SRC_LIST)

include_directories(./include)

add_library(Block SHARED ${SRC_LIST})
add_library(Blockchain SHARED ${SRC_LIST})

target_link_libraries(Block PUBLIC OpenSSL::Crypto)
target_link_libraries(Blockchain PUBLIC Block)