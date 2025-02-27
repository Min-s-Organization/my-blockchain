find_package(OpenSSL REQUIRED)

aux_source_directory(. SRC_LIST)

include_directories(./include)

add_library(Block SHARED ${SRC_LIST})

target_link_libraries(Block PRIVATE OpenSSL::Crypto)