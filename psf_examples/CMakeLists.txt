find_package(CapnProto REQUIRED)
capnp_generate_cpp(MSGS_SRCS MSGS_HDRS msgs/PsfExamples.capnp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(ping_server src/ping_server.cpp ${MSGS_HDRS})
target_link_libraries(ping_server PRIVATE psf)
add_executable(ping_client src/ping_client.cpp ${MSGS_HDRS})
target_link_libraries(ping_client PRIVATE psf)
