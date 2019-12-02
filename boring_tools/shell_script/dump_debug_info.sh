#!/bin/bash

#TODO: support clear

command_arr[0]="show int"
command_arr[1]="show thread"

login_addr="127.0.0.1 5002"
login_time=1 # wait for login

result_file="result_file."$(date +%m-%d_%H:%M:%S)
bin_relative_path="data-plane/hdp"

echo -e "\nwaiting for execution..."

func_cmd_part()
{
    command_num=${#command_arr[@]}

    for ((i = 0; i < $command_num; i++))
    do
        command_str+="${command_arr[$i]}\n"
    done

    cmd_result=`(echo -e $command_str; sleep $login_time) |
                telnet $login_addr`

    OLD_IFS=$IFS # backup Internal Field Seprator

    # 原来的换行符是 "\r\n", 这里只保留 "\n"
    # "\r\n" is the original newline, keep only "\n" here
    IFS=$'\r'

    echo -e $cmd_result >> $result_file

    IFS=$OLD_IFS # rest

    echo "command part done"
}

func_stack_part()
{
    OLD_IFS=$IFS # backup
    IFS=$'' # set empty

    pid=`ps aux | grep $bin_relative_path |grep -v grep | awk '{print $2}'`

    pstack_result=`pstack $pid`
    
    echo -e $pstack_result >> $result_file
    
    IFS=$OLD_IFS # rest

    echo "pstack part done"
}

echo -e "\n\tPART 1: command line result\n" > $result_file
func_cmd_part


echo -e "\n\tPART 2: pstack result\n" >> $result_file
func_stack_part

echo -e "\ndone."

