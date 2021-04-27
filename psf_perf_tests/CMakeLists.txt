find_package(CapnProto REQUIRED)
capnp_generate_cpp(MSGS_SRCS MSGS_HDRS msgs/PsfPerfTests.capnp)

include_directories(${CMAKE_CURRENT_BINARY_DIR} src)
add_executable(subscriber src/subscriber.cpp ${MSGS_HDRS})
target_link_libraries(subscriber PRIVATE psf)
add_executable(publisher src/publisher.cpp ${MSGS_HDRS})
target_link_libraries(publisher PRIVATE psf)
