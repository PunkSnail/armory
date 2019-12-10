#!/usr/bin/expect

# interpret this script using expect

set param "root@127.0.0.1"
set password "snail"

set timeout 7

spawn ssh ${param}

expect "password:"

send "${password}\n"

interact
