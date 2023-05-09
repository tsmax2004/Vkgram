file(GLOB_RECURSE CLIENT_CXX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/client/*.cpp)
file(GLOB_RECURSE SERVER_CXX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/server/*.cpp)
file(GLOB_RECURSE BUFFERS_CXX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/buffers/*.cpp)
file(GLOB_RECURSE INTERFACE_CXX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/interface/*.cpp)

add_executable(client ${INTERFACE_CXX_SOURCES} ${CLIENT_CXX_SOURCES} ${BUFFERS_CXX_SOURCES} client_main.cpp)
add_executable(server ${SERVER_CXX_SOURCES} ${BUFFERS_CXX_SOURCES} server_main.cpp)

include_directories("../lib")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(client concurrency_lib)
find_package(Qt6Widgets REQUIRED)
target_link_libraries(client Qt6::Widgets)


target_link_libraries(server concurrency_lib)
target_link_libraries(server pqxx)
