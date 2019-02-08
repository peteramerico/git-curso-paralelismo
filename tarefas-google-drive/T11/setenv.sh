#!/bin/bash

BASE_PATH=/opt/aclang/install

if [[ -z $1 ]]; then 
  export CC=clang
  export CXX=clang++
  export PATH=$BASE_PATH/bin:$PATH
  export C_INCLUDE_PATH=$BASE_PATH/include:$C_INCLUDE_PATH
  export CXX_INCLUDE_PATH=$BASE_PATH/include:$CXX_INCLUDE_PATH
  export LIBRARY_PATH=$BASE_PATH/lib:$LIBRARY_PATH
  export LD_LIBRARY_PATH=$BASE_PATH/lib:$BASE_PATH/libexec:$LD_LIBRARY_PATH
else
  binpath=$BASE_PATH/bin
  export PATH=$(sed 's|'$binpath'\:||g' <<<$PATH)
  unset C_INCLUDE_PATH
  unset CXX_INCLUDE_PATH
  unset LIBRARY_PATH
  unset LD_LIBRARY_PATH
  unset CC
  unset CXX
fi
