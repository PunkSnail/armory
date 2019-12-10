#!/bin/bash

param=root@172.17.21.18
password=123456

# need to install sshpass
SSHPASS=${password} sshpass -e ssh ${param}

