cmake_minimum_required(VERSION 2.6)
project(jnipp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(JNI)

if (JNI_FOUND)
  include_directories(${JNI_INCLUDE_DIRS})
else()
  message(FATAL_ERROR "JNI not supported.")
endif()

find_package(Boost 1.55 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/ ${Boost_INCLUDE_DIR})

set(JNIPP_LIBRARY_SOURCE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/environment
  ${CMAKE_CURRENT_SOURCE_DIR}/src/listener
)

add_library(jnipp SHARED ${JNIPP_LIBRARY_SOURCE})
target_link_libraries(jnipp ${Boost_LIBRARIES})
