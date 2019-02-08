cmake_minimum_required (VERSION 3.4)

set (LANGUAGE "cxx")
set (TARGET "bfs")

include (../../common/src/CMakeLists.txt)

# To use slow (but precise) math routines
if (slowmath)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fp-model precise")
else ()
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffast-math")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")
endif ()

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (SOURCES
  main.cc
  ../../common/src/parboil.c
)

add_executable(bfs ${SOURCES})

target_link_libraries (${TARGET}
  m
  stdc++
  gomp
)

# To use the MKL library
if (DEFINED mkl)
  link_directories(/opt/intel/mkl/8.0/lib/em64t)
  target_link_libraries (${TARGET}
    mkl
    vml
    guide
    pthread
  )
  target_include_directories(
    /opt/intel/mkl/9.1.023/include
    ${INCLUDES}
  )
  add_definitions(MKL)
endif()

add_check_target(${TARGET} ${SOURCES})
