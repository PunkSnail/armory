#!/bin/bash

# find 参数
# -maxdepth 查找最大层数
# -type     查找类型, 不指定则显示全部匹配文件与文件夹
# -newermt  查找时间 格式: "2019-06-10"

echo " "

if [ $# -lt 1  ]
then
    echo "missing date"
    exit 1
else
    spec_date=$1
fi

# 格式匹配并且日期有效
if echo $1 | grep -Eq "[0-9]{4}-[0-9]{2}-[0-9]{2}" \
    && date -d $1 +%Y%m%d > /dev/null 2>&1
then : 
else
    echo Invalid input date 
    echo Try this format: \"yyyy-mm-dd\"
    exit 1
fi

# option: -print -delete
spec_opt=$2

start_date=$spec_date #:`date -d "yesterday $spec_date" +%Y-%m-%d`
end_date=`date -d "tomorrow $spec_date" +%Y-%m-%d`

list=`find ./ -maxdepth 1 -type f\
    -newermt "$start_date" ! -newermt "$end_date" $spec_opt`

if [ -n "$spec_opt" ]
then
    echo "operation: $spec_opt"
    echo "$list" 
    exit
fi

count=0

for i in $list
do
    ls -l $i
    let count+=1
done

if [ $count -eq 0 ]
then
    echo "Target not found"
fi 


