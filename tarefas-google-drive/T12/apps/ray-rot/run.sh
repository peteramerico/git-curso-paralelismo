#! /usr/bin/env bash

if [ $# -le 0 ]; then
  echo "usage: $0 <serial | bdx | doax | check>"
  exit -1
fi

version=$1
prefix='../output/scene_out'

./${version} ../ifiles.txt ../ofiles.txt 90 200 200 5 1>/dev/null
for j in $(seq -w 1 24); do
  mv ${prefix}.${j} ${prefix}-$version-${j}
  diff -q -s ${prefix}-serial-${j} ${prefix}-${version}-${j}
done

