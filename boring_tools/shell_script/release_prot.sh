#!/bin/sh

if [ $# -lt 1 ] 
then
    echo "missing args"
    exit 1
else
    echo port = $1
fi

raw_str=`netstat -apn| grep $1 | awk '{print $7}'`

proc_pid=`echo $raw_str | cut -d "/" -f1`

echo "proc_pid = " $proc_pid

kill -9 $proc_pid

ps -fe| grep "{$proc_pid}"| grep -v grep

if [ $? -ne 0  ]
then
    echo " $1 release success"
    echo "  "
else
    echo "$1 release failed"
    echo "  "
fi

