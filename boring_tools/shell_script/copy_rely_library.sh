#!/bin/bash

if [ $# -lt 1 ] 
then
    echo "missing args"
    exit 1
else
    target_lib=$1
fi

loop_limit=100

for ((i = 1; i <= $loop_limit; i++))
do
current_lib=`ldd $target_lib | awk -v line=$i 'NR==line{print $3}'`

if [ -z "$current_lib"  ]
then
    break
fi

if [ -f "$current_lib" ] 
then
    echo "now copy "$current_lib
    cp  $current_lib . > /dev/null 2>&1 
fi

done

