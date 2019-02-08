cmake_minimum_required(VERSION 3.4)

project(Loop_A)

include (../CMakeCommon.cmake)

set (LANGUAGE "c")

set (SOURCES
  bitcnts.c
  bitarray.c
  bitcnt_1.c
  bitcnt_2.c
  bitcnt_3.c
  bitcnt_4.c
  bitfiles.c
  bitstrng.c
  bstr_i.c
  loop-wrap.c
)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -fno-inline-functions")

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (NOT CHECK)
add_executable(bdx ${SOURCES})
set_target_properties(bdx
  PROPERTIES
  COMPILE_FLAGS "-fopenmp -fopenmp-use-dependence-analysis"
  COMPILE_DEFINITIONS "__USE_BDX__")

add_executable(tls ${SOURCES})
set_target_properties(tls
  PROPERTIES
  COMPILE_FLAGS "-fopenmp -mrtm"
  COMPILE_DEFINITIONS "__USE_TLS__")

add_executable(doax ${SOURCES})
set_target_properties(doax
  PROPERTIES
  COMPILE_FLAGS "-fopenmp")

target_link_libraries(bdx m omp)
target_link_libraries(tls m omp)
target_link_libraries(doax m omp)
endif()

add_executable(serial ${SOURCES})

if (NOT CHECK)
  target_link_libraries(serial m)
else()
  target_link_libraries(serial m omp)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
  add_check_target(serial ${SOURCES})
endif()
