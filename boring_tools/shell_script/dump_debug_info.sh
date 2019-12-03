#!/bin/bash

command_arr[0]="show int"
command_arr[1]="show thread"

addr="127.0.0.1"
port="5002"
timeout=1 # wait for login

bin_name=hdp
relative_path=data-plane/hdp

result_file="result_file."$(date +%m-%d) #_%H:%M:%S)

show_help()
{
    echo -e "Options:\n"\
            "  -c <cmd1>, <cmd2>\tSpecified DP command line parameters,"\
            "separated by ','\n"\
            "  -o <file>\t\tPlace the output into <file>\n"\
            "  --help\t\tDisplay this information"
}

set_cmd_arr()
{
    cmd_flag=$1
    if [ $cmd_flag == 1 ]
    then
        unset command_arr
    else
        return
    fi
    OLD_IFS=$IFS # Internal Field Seaprator
    IFS=$',' # set new seaprator

    cmd_idx=0
    for cmd in $command_str
    do
        command_arr[$cmd_idx]=$cmd
        ((cmd_idx++))
    done

    IFS=$OLD_IFS
}

func_handle_input()
{
    cmd_flag=0
    file_flag=0
    for arg in $@
    do
        # set result file path
        if [ "$arg" == "-o" ]
        then
            file_flag=1
            continue
        fi
        if [ $file_flag == 1 ]
        then
            result_file=$arg
            file_flag=0
            continue
        fi
        # set specified DP command 
        if [ "$arg" == "-c" ]
        then
            cmd_flag=1
            continue
        fi
        if [ $cmd_flag == 1 ]
        then
            command_str+=$arg" "
        fi
        # show help
        if [ "$arg" == "--help" ] || [ "$arg" == "-h" ] 
        then
            show_help
            exit 0
        fi
    done

    set_cmd_arr $cmd_flag
}

func_cmd_part()
{
    command_str=''
    command_num=${#command_arr[@]}

    for ((i = 0; i < $command_num; i++))
    do
        echo "${command_arr[$i]}"
        command_str+="${command_arr[$i]}\n"
    done

    exist=`netstat -apn | grep $port | awk '{print $7}' | grep $bin_name`

    if [ ! "$exist" ]
    then
        echo "The specified port couldn't be found" | tee $result_file
        exit 1
    fi

    cmd_result=`(echo -e $command_str; sleep $timeout) |
                telnet $addr $port`
    OLD_IFS=$IFS

    # 原来的换行符是 "\r\n", 这里只保留 "\n"
    # "\r\n" is the original newline, keep only "\n" here
    IFS=$'\r'

    echo -e $cmd_result >> $result_file

    IFS=$OLD_IFS

    echo "Command part done"
}

func_stack_part()
{
    OLD_IFS=$IFS
    IFS=$'' # set empty

    pid=`ps aux | grep $relative_path | grep -v grep | awk '{print $2}'`

    if [ ! "$pid" ]
    then
        echo "The specified process couldn't be found" | tee $result_file
        echo "Relative path: $relative_path"
        exit 1
    fi

    pstack_result=`pstack $pid`
    
    echo -e $pstack_result >> $result_file
    
    IFS=$OLD_IFS

    echo "Pstack part done"
}

func_main()
{
    func_handle_input $@

    echo -e "\nWaiting for execution..."
    
    echo -e "\n\tPART 1: Command line result\n" > $result_file
    func_cmd_part

    echo -e "\n\tPART 2: Pstack result\n" >> $result_file
    func_stack_part

    echo "Saved output info to \"$result_file\""
    echo -e "\nDone."
}

# 入口函数, 传入参数数组
# entry function, passing in parameter array
func_main $@

