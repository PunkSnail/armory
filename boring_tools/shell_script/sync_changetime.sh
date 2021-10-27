#!/bin/bash

# 把文件的“修改时间" 变更为 "创建时间" for Darwin
# Change the "Modify" time of the file to the "Birth" time
# 批量修改示例/Batch modification example:
# for file in `ls *.jpg`; do sync_changetime $file; done

TARGET=$1
STAT=gstat; SYNC=gtouch # always love GNU
#if [ `uname` == "Linux" ]; then STAT=stat; SYNC=touch; fi

if ! command -v $STAT &> /dev/null; then
    echo "command not found:" $STAT && exit
fi

if ! command -v $SYNC &> /dev/null; then
    echo "command not found:" $SYNC && exit
fi

if [ -z $TARGET ] || [ ! -f $TARGET ]; then
    echo "cannot access" \'$TARGET\' "no such file" && exit
fi

RAW=`$STAT $TARGET | grep "Birth:"`
DATE=`echo $RAW | awk '{print $2}'`
TIME=`echo $RAW | awk '{print $3}' | awk '{split($0,a,"."); print a[1]}'`

if [ -z $DATE ] || [ -z $TIME ]; then
    echo "unable to get the creation time of" \'$TARGET\' && exit
fi

year=${DATE%%-*} # YYYY-MM-DD -> YYYY
month=${DATE%-*}; month=${month##*-} # YYYY-MM-DD -> YYYY-MM -> MM
day=${DATE##*-} # YYYY-MM-DD -> DD

hour=${TIME%%:*} # HH:MM:SS -> HH
minute=${TIME%:*}; minute=${minute##*:} # HH:MM:SS -> HH:MM -> MM
second=${TIME##*:} # HH:MM:SS -> SS

# Update its "Modify" time
$SYNC -mt $year$month$day$hour$minute.$second $TARGET
# format_time=`echo $month/$day/$year $TIME` # for setfile -m
