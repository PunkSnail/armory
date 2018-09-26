#!/bin/bash

s_name=aa.bb.cc.dd

echo "  original name:    " $s_name

name=${s_name%%.*}
echo "  name%%.*:         " $name

name=${s_name%.*}
echo "  name%.*:          " $name

