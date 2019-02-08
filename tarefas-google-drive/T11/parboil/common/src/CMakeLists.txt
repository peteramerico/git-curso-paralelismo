# (c) 2007 The Board of Trustees of the University of Illinois.

#set (SRCDIR "${PARBOIL_ROOT}/common/src")
#set (BUILD_DIR "${PARBOIL_ROOT}/common/lib")

##include $(PARBOIL_ROOT)/common/mk/common.mk
##include $(PARBOIL_ROOT)/common/mk/cuda.mk

#if (NOT DEFINED PARBOIL_ROOT)
#  message (FATAL_ERROR "PARBOIL_ROOT varibale not defined!")
#endif ()

#include_directories(${PARBOIL_ROOT}/common/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../common/include)

function(add_check_target _TARGET)
  set(_SOURCES "${ARGN}")
  #message (STATUS "_SOURCES: ${_SOURCES}")
  set (_FULL_PATH_SOURCES "")
  set (LL_FILES "")
  set (prefix "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_TARGET}.dir")
  foreach(src IN LISTS _SOURCES)
    set (src "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    get_filename_component (realpath_src ${src} REALPATH)
    list (APPEND _FULL_PATH_SOURCES ${realpath_src})
    if (src STREQUAL realpath_src)
      string (REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "${prefix}" src "${src}")
    else()
      set (src "${prefix}/${realpath_src}")
    endif()
    string (CONCAT src "${src}" ".ll")
    #message (STATUS "src: ${src}")
    list (APPEND LL_FILES ${src})
  endforeach()
  #message (STATUS "LL_FILES: ${LL_FILES}")
  if (LANGUAGE STREQUAL "c")
    set (COMPILER "clang")
    string (REPLACE " " ";" COMPILER_FLAGS "${CMAKE_C_FLAGS}")
  else()
    set (COMPILER "clang++")
    string (REPLACE " " ";" COMPILER_FLAGS "${CMAKE_CXX_FLAGS}")
  endif()
  get_target_property(LIBRARIES ${_TARGET} LINK_LIBRARIES)
  set (LINK_FLAGS "")
  foreach (library IN LISTS LIBRARIES)
    list (APPEND LINK_FLAGS "-l${library}")
  endforeach() 
  #message (STATUS "Link flags: ${LINK_FLAGS}")
  add_custom_target (${_TARGET}-check
    COMMAND dp-checker -p . ${_FULL_PATH_SOURCES}
    COMMAND ${COMPILER} ${COMPILER_FLAGS} -O3 ${LL_FILES} -o ${_TARGET}-check ${LINK_FLAGS} -lcheck-rt
  )
  add_custom_target (check-clean
    COMMAND rm -rf ${_TARGET}-check ${LL_FILES})
endfunction(add_check_target)

#OBJS=$(BUILD_DIR)/parboil.o
#OBJS_TWO=$(BUILD_DIR)/parboil_cuda.o
#OBJS_THREE=$(BUILD_DIR)/parboil_opencl.o
#TARGET=$(BUILD_DIR)/libparboil.a 
#TARGET_TWO=$(BUILD_DIR)/libparboil_cuda.a
#TARGET_THREE=$(BUILD_DIR)/libparboil_opencl.a

if (NOT DEFINED CUDA_PATH)
  set (TARGET_CUDA "no_cuda")
else()
  set (TARGET_CUDA "${BUILD_DIR}/libparboil_cuda.a")
endif()

if (NOT DEFINED OPENCL_PATH)
  set (TARGET_OPENCL "no_opencl")
else()
  set (TARGET_OPENCL "${BUILD_DIR}/libparboil_opencl.a")
endif()

#TARGETS=$(TARGET) $(TARGET_CUDA) $(TARGET_OPENCL)

#.PHONY: all clean

#all : $(TARGETS)

#no_cuda:
#	@echo
#	@echo CUDA_PATH is not set in $$PARBOIL_ROOT/common/Makefile.conf. CUDA ignored. | grep --color CUDA_PATH
#	@echo

if (TARGET_CUDA EQUAL "no_cuda")
  message(WARNING "CUDA_PATH is not set in ${PARBOIL_ROOT}/common/CMakeLists.conf. CUDA ignored.")
endif()

if (TARGET_OPENCL EQUAL "no_opencl")
  message(WARNING "OPENCL_PATH is not set in ${PARBOIL_ROOT}/common/Makefile.conf. OPENCL ignored.")
endif()

#add_library(parboil STATIC ${PARBOIL_ROOT}/common/src/parboil.c)

#clean:
#	rm -f $(OBJS) $(TARGETS)

#$(BUILD_DIR)/parboil.o : parboil.c
#	$(CC) -c -g $(CFLAGS) $^ -o $@

#$(BUILD_DIR)/parboil_cuda.o : parboil_cuda.c
#	$(CC) -c -g $(CFLAGS) -I$(CUDA_PATH)/include $^ -o $@

#$(BUILD_DIR)/parboil_opencl.o : parboil_opencl.c
#	$(CC) -c -g $(CFLAGS) -I$(OPENCL_PATH)/include $^ -o $@

#$(TARGET) : $(OBJS)
#	$(AR) rc $@ $?
#	$(RANLIB) $@

#$(TARGET_TWO) : $(OBJS_TWO)
#	$(AR) rc $@ $?
#	$(RANLIB) $@
	
#$(TARGET_THREE) : $(OBJS_THREE)
#	$(AR) rc $@ $?
#	$(RANLIB) $@	
