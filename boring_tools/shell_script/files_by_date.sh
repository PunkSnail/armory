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

day=${spec_date:0-2}

# option: -print -delete
spec_opt=$2

start_day=$[day - 1]
end_day=$[day + 1]

# align
if [ ${#start_day} -eq 1 ]
then
    start_day="0"$start_day
fi

if [ ${#end_day} -eq 1 ]
then
    start_day="0"$end_day
fi

org_len=${#spec_date}
let org_len-=2

start_date=${spec_date:0:$org_len}$start_day

end_date=${spec_date:0:$org_len}$end_day

if [ $day -gt 27 ] # equal to "(( $day > 27 ))"
then
    echo "This date may be wrong, so you need to deal with it."
    echo find ./ -maxdepth 1  -newermt \"$start_date\" ! -newermt \"$end_date\"
    echo " " 
    exit
fi

list=`find ./ -maxdepth 1 -type f\
    -newermt "$start_date" ! -newermt "$end_date" $spec_opt`

if [ -n "$spec_opt" ]
then
    echo "operation: $spec_opt"
    echo " " 
    echo $list
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


