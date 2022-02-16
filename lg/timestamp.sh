#!/bin/bash

s=$1
shift

t="00000000"

checktime()
{
  m=`date -r $1 +%Y%m%d`
  if [ $m -gt $t ]; then
    t=$m
  fi
}

while [ $# -gt 0 ]; do
  if [ -d $1 ]; then
    for f in $1/* ; do
      case $f in
        $1/stamp-*)
          ;;
        *)
          checktime $f
          ;;
      esac
    done
  else
    checktime $1
  fi
  shift
done

echo -n > $s/stamp-$t
