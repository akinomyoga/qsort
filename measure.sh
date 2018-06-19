#!/bin/sh

compare_qsorts () {
  #"$measure" qsort   "$@"   0   0   0 0
  "$measure" qs9e17  "$@" 140 900   0 0
  "$measure" qs10a5  "$@" 140 900 400 0
  "$measure" qs10a5m "$@" 140 900 400 0
  "$measure" glibc   "$@"   0   0   0 0
  "$measure" newlib  "$@"   0   0   0 0
}

for measure in build/L*/measure.exe; do
  echo "=== $measure ==="
  compare_qsorts -3 10000 8    1000
  compare_qsorts -3 10000 1000 1000
done 2>/dev/null | tee -a measure.log
