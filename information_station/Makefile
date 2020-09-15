
TOP=$(shell pwd)/

# create lib directory
TMP:=$(shell mkdir -p lib)

CFLAGS:= -Wall -Wconversion -g -D_DEBUG

export TOP CFLAGS

EXCLUDE_DIRS=inc lib

tail:all
	@/bin/echo -e "\033[32mDone. \033[0m" #show green

include $(TOP)make.env

