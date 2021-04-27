find_package(CapnProto REQUIRED)
capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS include/psf/registry/RegistryRpc.capnp)

# library
add_library(psf SHARED
  src/Node.cpp
  src/registry/Registry.cpp
  src/registry/RegistryClient.cpp
  ${CAPNP_SRCS}
)
target_link_libraries(psf PUBLIC CapnProto::capnp-rpc)
target_include_directories(psf PUBLIC include)
target_include_directories(psf SYSTEM PUBLIC third_party/asio/include ${CMAKE_CURRENT_BINARY_DIR}/include)

# registry
add_executable(psf_registry
  src/registry/registry_server.cpp
  src/registry/Registry.cpp
  src/registry/RegistryServer.cpp
  ${CAPNP_SRCS}
)
target_link_libraries(psf_registry PRIVATE CapnProto::capnp-rpc)
target_include_directories(psf_registry PRIVATE include)
target_include_directories(psf_registry SYSTEM PRIVATE third_party/asio/include ${CMAKE_CURRENT_BINARY_DIR}/include)

# tests
if (BUILD_TESTING)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

  include_directories(SYSTEM third_party/catch2/include)
  add_executable(test_psf test/test_main.cpp test/test_publisher.cpp)
  target_link_libraries(test_psf PRIVATE psf)

  include(Catch)
  catch_discover_tests(test_psf)
endif()
