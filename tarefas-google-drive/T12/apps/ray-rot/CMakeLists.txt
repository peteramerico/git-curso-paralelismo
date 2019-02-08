cmake_minimum_required(VERSION 3.4)

project(ray-rot)

include (../CMakeCommon.cmake)

set (LANGUAGE "cxx")

set (SOURCES
  program.cpp
  image.cpp
  ray_engine.cpp
  rotation_engine.cpp
)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -fno-inline-functions")

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (NOT CHECK)
add_executable(bdx ${SOURCES})
set_target_properties(bdx
  PROPERTIES
  COMPILE_FLAGS "-fopenmp -fopenmp-use-dependence-analysis"
  COMPILE_DEFINITIONS "__USE_BDX__"
  LINK_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -fopenmp-use-dependence-analysis"
)

add_executable(tls ${SOURCES})
set_target_properties(tls
  PROPERTIES
  COMPILE_FLAGS "-fopenmp -mrtm"
  COMPILE_DEFINITIONS "__USE_TLS__"
  LINK_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -mrtm"
)

add_executable(doax ${SOURCES})
set_target_properties(doax
  PROPERTIES
  COMPILE_FLAGS "-fopenmp"
  LINK_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp"
)

target_link_libraries(bdx m)
target_link_libraries(tls m)
target_link_libraries(doax m)
endif()

add_executable(serial ${SOURCES})

if (NOT CHECK)
  target_link_libraries(serial m)
else()
  target_link_libraries(serial m)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
  add_check_target(serial ${SOURCES})
endif()
