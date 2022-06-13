#!/bin/bash

# find 参数
# -maxdepth 查找最大层数
# -type     查找类型, 不指定则显示全部匹配文件与文件夹

echo " "

if [ $# -lt 1  ]
then
    echo "missing args"
    exit 1
fi

show_help()
{
    echo -e "Options:\n"\
            "  <yyyymmdd>\tFind files by date\n"\
            "  --delete\tDelete found files\n"\
            "  --help\tDisplay this information"
}

assign_opt="-print"
date_input=0

func_handle_input()
{
    for arg in $@
    do
        if [ "$arg" == "--delete" ]
        then
            assign_opt="-print -delete"
            continue
        fi
        if echo $arg | grep -Eq "[0-9]{4}[0-9]{2}[0-9]{2}"
        then
            date_input=$arg
            continue
        fi
        if [ "$arg" == "--help" ] || [ "$arg" == "-h" ]
        then
            show_help
            exit 0
        fi
        echo Unknown input: $arg
        exit 0
    done
}

func_handle_input $@

if [ 0 == $date_input ]
then
    show_help
    exit 1
fi

# If the time of file is 00:00:00, then it belongs to the previous day.
start_date=$date_input
# This doesn't result in an extra day of checking.
end_date=`date -d "tomorrow $date_input" +%Y%m%d`

list=`find ./ -maxdepth 1 -type f\
    -newermt "$start_date" ! -newermt "$end_date" $assign_opt`

count=0

for i in $list
do
    if [ "$assign_opt" == "-print" ]
    then
        ls -l $i
    fi
    let count+=1
done

if [ $count -eq 0 ]
then
    echo "Target not found"
elif
    [ "$assign_opt" != "-print" ]
then
    echo "Total: $count"
fi

