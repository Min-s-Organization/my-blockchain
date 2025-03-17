add_executable(receiver userservice.cpp ../user.pb.cc)
target_link_libraries(receiver rpc protobuf)