#!/bin/sh

compare_qsorts1 () {
  #"$measure" qsort   "$@"   0   0   0 0
  "$measure" qs9e17   "$@" 140 900   0 0
  "$measure" qs10a6   "$@" 140 900 400 0
  "$measure" qs10a6m  "$@" 140 900 400 0
  "$measure" qs226ms2 "$@"   0   0   0 0
  "$measure" glibc    "$@"   0   0   0 0
  "$measure" newlib   "$@"   0   0   0 0
  "$measure" stdsort  "$@"   0   0   0 0
}

# original benchmark.sh
measure1 () {
  Z=1
  measure=build/L3N/measure.exe
  {
    compare_qsorts1  -3 10000    8 $((1000*Z))
    compare_qsorts1  -3 10000   20 $((1000*Z))
    compare_qsorts1  -3 10000   24 $((1000*Z))
    compare_qsorts1  -3 10000  500 $(( 200*Z))
    compare_qsorts1  -3 10000 1000 $(( 200*Z))
    compare_qsorts1 100 10000    8 $((2000*Z))
    compare_qsorts1 100 10000   20 $((2000*Z))
    compare_qsorts1 100 10000   24 $((2000*Z))
    compare_qsorts1 100 10000  500 $(( 200*Z))
    compare_qsorts1 100 10000 1000 $(( 200*Z))
    compare_qsorts1   2 10000    8 $((4000*Z))
    compare_qsorts1   2 10000   20 $((4000*Z))
    compare_qsorts1   2 10000   24 $((4000*Z))
    compare_qsorts1   2 10000  500 $(( 400*Z))
    compare_qsorts1   2 10000 1000 $(( 400*Z))
  } 2>/dev/null | tee -a benchmark.txt
}

# compare optimization options
measure2 () {
  for measure in build/L*/measure.exe; do
    echo "=== $measure ==="
    compare_qsorts1 -3 10000 8    1000
    compare_qsorts1 -3 10000 1000 1000
  done 2>/dev/null | tee -a measure.log
}

# キャッシュの影響を見るために要素数を減らす
measure3() {
  for measure in build/L{2,3,3N}/measure.exe; do
    echo "=== $measure ==="
    compare_qsorts1 -3 1000 8 100000
  done 2>/dev/null | tee -a measure.log
}

# -DDEBUG と -DNDEBUG を比較するために使用
measure4() {
  for measure in build/L{2,3,3N}/measure.exe; do
    echo "=== $measure ==="
    compare_qsorts1 -3 10000 8 1000
  done 2>/dev/null | tee -a measure.log
}

[[ $1 ]] && "$1"
