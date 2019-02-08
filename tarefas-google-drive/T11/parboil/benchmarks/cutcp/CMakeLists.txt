cmake_minimum_required (VERSION 3.4)

set (LANGUAGE "c")
set (TARGET "cutcp")

include (../../common/src/CMakeLists.txt)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fopenmp")

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (SOURCES
  cutcpu.c
  excl.c
  main.c
  output.c
  readatom.c
  ../../common/src/parboil.c
)

add_executable(${TARGET} ${SOURCES})

target_link_libraries (${TARGET}
  m
  gomp
)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_check_target(${TARGET} ${SOURCES})
