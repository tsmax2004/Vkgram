cmake_minimum_required(VERSION 3.14)

project(concurrency_lib)

add_subdirectory(library)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE LIB_CXX_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE LIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

set(LIBS_LIST "wheels;twist;fmt;function2;expected")
if((CMAKE_BUILD_TYPE MATCHES Release) AND NOT TWIST_FAULTY)
    list(APPEND LIBS_LIST "mimalloc")
endif()

if (LIB_CXX_SOURCES)
    add_library(concurrency_lib STATIC ${LIB_CXX_SOURCES} ${LIB_HEADERS})
    target_include_directories(concurrency_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(concurrency_lib ${LIBS_LIST})
else()
    # header-only library
    add_library(concurrency_lib INTERFACE)
    target_include_directories(concurrency_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(concurrency_lib INTERFACE ${LIBS_LIST})
endif()