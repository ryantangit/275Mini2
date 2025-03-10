cmake_minimum_required(VERSION 3.13)
project(Count)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-g)

## Finding pcakage on Protobuf and gRPC
option(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
   find_program(_PROTOBUF_PROTOC protoc)
else()
   set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif() 


find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
   find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
   set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif() 
#####

# the protobuf definitions (absolute path needed)
set(BASE_DIR ../${CMAKE_CURRENT_SOURCE_DIR})
set(PROTOS_BASE ${BASE_DIR}/proto_def)
message("CCurrent PROTOS_BASE directory: ${PROTOS_BASE}")
set(PROTOS ${PROTOS_BASE}/count.proto)

set(PROTO_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto-src)
file(MAKE_DIRECTORY ${PROTO_SRC_DIR})
include_directories(${PROTO_SRC_DIR})

# note each file must be listed separately!
set(PROTO_SRCS "${PROTO_SRC_DIR}/count.pb.cc")
set(GRPC_SRCS "${PROTO_SRC_DIR}/count.grpc.pb.cc")
set(PROTO_HDRS "${PROTO_SRC_DIR}/count.pb.h")
set(GRPC_HDRS "${PROTO_SRC_DIR}/count.grpc.pb.h")

message(STATUS "grpc plugin: ${_GRPC_CPP_PLUGIN_EXECUTABLE}")

add_custom_command(
   OUTPUT "${PROTO_SRCS}" "${GRPC_SRCS}" "${PROTO_HDRS}" "${GRPC_HDRS}"
   COMMAND ${_PROTOBUF_PROTOC}
   ARGS --grpc_out "${PROTO_SRC_DIR}"
        --cpp_out "${PROTO_SRC_DIR}"
        -I "${PROTOS_BASE}"
        --plugin=protoc-gen-grpc=${_GRPC_CPP_PLUGIN_EXECUTABLE}
        "${PROTOS}"
   DEPENDS "${PROTOS}"
   COMMENT "creating generated protobuf/grpc files"
   VERBATIM
)

include_directories("PROTO_SRC_DIR")

add_executable(count-main
   count.cpp
)

