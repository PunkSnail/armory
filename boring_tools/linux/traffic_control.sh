#!/bin/bash

usage()
{
    cat << EOF
Usage: $0 [OPTION]...
Limit the bandwidth of an interface, based on Linux tc

OPTIONS:
   -i <interface>   All options require an interface (except "-h")
   -u <rate>        Set maximum upload rate in Kbps (support level set)
   -r <ip1,ip2>     Restrict IPs, Separate items by ','
   -p <port1,port2> Restrict ports, Separate items by ','
   -l <level>       Set the priority level (1-3) under the current rate
                        1 highest priority
                        2 medium priority
                        3 lowest priority
   -a               Append IP or port to an existing class
   -c               Clear the limits from interface
   -s               Display the limits status of interface
   -h               Display this message
EOF
}

TC=/usr/sbin/tc # to avoid environment variables not being found

intfc=""        # name of interface
up_speed=""     # for egress
task_mode=""    # task to be performed
is_append=0     # appending does not create a new queue

set_restrict_arrays()
{
    OLD_IFS=$IFS # Internal Field Seaprator
    IFS=$',' # set new seaprator

    idx=0
    for item in $ips_input
    do
        restrict_ips[$idx]=$item
        ((idx++))
    done
    idx=0
    for item in $ports_input
    do
        restrict_ports[$idx]=$item
        ((idx++))
    done
    IFS=$OLD_IFS
}

# parsing the input parameters related to the speed limit
parse_limit_args()
{
    if [[ -z "$up_speed" ]] || [[ -z "$intfc" ]]; then
        echo -e "No interface or rate limit specified.\n"
        exit 1
    fi

    if [[ -z "$level" ]]; then
        level=3
    fi
    if [[ $level != 1 ]] && [[ $level != 2 ]] && [[ $level != 3 ]]
    then
        echo -e "Invalid input level '$level'.\n"
        exit 1
    fi
    set_restrict_arrays
}

# parse input parameters, possibly run some quick tasks and exit
check_and_run_task()
{
    if [[ -n "$task_mode" ]] && [[ -z "$intfc" ]]; then
        echo -e "Please specify the interface for '$task_mode' mode.\n"
        exit 1
    fi
    if [ "$task_mode" = "status" ]; then
        $TC -s qdisc ls dev "$intfc"
        $TC -s class ls dev "$intfc"
        exit
    fi
    if [ "$task_mode" = "clear" ]; then
        $TC qdisc del dev "$intfc" root    2> /dev/null > /dev/null
        $TC qdisc del dev "$intfc" ingress 2> /dev/null > /dev/null
        exit
    fi
    # task mode is default, so we need to parse the other input parameters
    parse_limit_args
}

create_root_htb()
{
    if [[ -n "$restrict_ips" ]] || [[ -n "$restrict_ports" ]]; then
        # no default class, no bandwidth limits
        $TC qdisc add dev "$intfc" root handle 1: htb r2q 1
    else
        # default 20 indicates all unclassified traffic to class 1:20
        $TC qdisc add dev "$intfc" root handle 1: htb default 20
    fi
}

# $1:conf_limit    $2:classid    $3rate    $4:prio
common_class_set()
{
    if [[ $# < 4 ]]; then
        echo "error getting parameters this should never happen"
        exit
    fi
    conf_limit=$1
    conf_classid=$2
    conf_rate=$3
    conf_prio=$4

    if [ "$conf_rate" -eq 0 ]; then
        conf_rate="1kbit"
    else
        conf_rate="${conf_rate}kbit"
    fi
    # tc 不支持值为 0 的限速, 这里限制到 1bps, 使之几乎不可用
    if [ "$conf_limit" -eq 0 ]; then
        conf_rate="1bps" # 1bps near to packet drop
    fi
    $TC class add dev $intfc parent 1:1 classid $conf_classid htb \
        rate $conf_rate prio $conf_prio # ceil $((90*${up_speed}/100))kbit
}

set_stochastic_fair()
{
    # perturb:
    #   Interval in seconds for queue algorithm perturbation. Defaults to
    # 0, which means that no perturbation occurs. Do not set too low for
    # each perturbation may cause some packet reordering. Advised value 10

    #   队列算法扰动的间隔秒数. 默认值为 0 表示不发生扰动. 不要设置得太低,
    # 每次扰动可能导致包重排序, 建议值为 10

    $TC qdisc add dev "$intfc" parent 1:10 handle 10: sfq perturb 10

    $TC qdisc add dev "$intfc" parent 1:20 handle 20: sfq perturb 10

    $TC qdisc add dev "$intfc" parent 1:30 handle 30: sfq perturb 10

    if [[ $level == 1 ]]; then
        level="1:10"
    elif [[ $level == 2 ]]; then
        level="1:20"
    else
        level="1:30"
    fi
}

# set priority classes for upload
set_upload_classes()
{
    if [ "$up_speed" -eq 0 ]; then
        parent_speed=1 # ensure highest priority class always available
    else
        parent_speed=$up_speed
    fi
    # the sum of all subclasses is less than or equal to the parent class
    $TC class add dev "$intfc" parent 1: classid 1:1 htb \
        rate "${parent_speed}kbit" prio 5
    # highest priority class
    common_class_set $parent_speed 1:10 $((20*${parent_speed}/100)) 1
    # medium priority class
    common_class_set $up_speed 1:20 $((40*${up_speed}/100)) 2
    # lowest priority class
    common_class_set $up_speed 1:30 $((20*${up_speed}/100)) 3

    set_stochastic_fair
}

set_upload_filters()
{
    prefix="$TC filter add dev "$intfc" parent 1: protocol ip "
    # Control the ports, regardless of the source and destination
    for item in "${restrict_ports[@]}"; do
        [ -n "$item" ] || continue
        $prefix prio 11 u32 match ip dport "$item" 0xffff flowid $level

        $prefix prio 12 u32 match ip sport "$item" 0xffff flowid $level
    done

    # Control the IPs, regardless of the source and destination
    for item in "${restrict_ips[@]}"; do
        [ -n "$item" ] || continue
        $prefix prio 13 u32 match ip src "$item" flowid $level

        $prefix prio 14 u32 match ip dst "$item" flowid $level
    done
}

# restrict all traffic flow on the interface
restrict_them_all()
{
    prefix="$TC filter add dev "$intfc" parent 1: protocol ip "
    # TOS minimum delay in 1:10 (ssh)
    $prefix prio 10 u32 match ip tos 0x10 0xff flowid 1:10

    $prefix prio 11 u32 match ip dst 0.0.0.0/0 flowid $level
}

shape_upload_traffic()
{
    if [ $is_append == 1 ]
    then
        set_upload_filters
        exit
    fi
    create_root_htb

    set_upload_classes

    if [[ -n "$restrict_ips" ]] || [[ -n "$restrict_ports" ]]; then
        set_upload_filters
    else
        restrict_them_all
    fi
}

func_main()
{
    while getopts i:u:r:p:l:acsh o; do
        case "$o" in
            i) intfc="$OPTARG";;
            u) up_speed="$OPTARG";;
            r) ips_input="$OPTARG";;
            p) ports_input="$OPTARG";;
            l) level="$OPTARG";;
            a) is_append=1;;
            c) task_mode="clear";;
            s) task_mode="status";;
            h)	usage;
                exit;;
            [?]) usage;
                exit;;
        esac;
    done

    check_and_run_task

    if [[ -n "$up_speed" ]]; then
        shape_upload_traffic
    fi
}

# entry function, pass in the args
func_main $@

