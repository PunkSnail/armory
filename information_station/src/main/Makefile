
TARGET:=run_test

# If the target isn't found in the current directory,
# it will look in the directory specified by vpath.
vpath %.so $(TOP)lib

EXEPATH:=$(TOP)

TARGET_LIBS:=libbaseinfo_collect.so

CC=gcc
CFLAGS+= -std=c11 -I $(TOP)inc -Wl,-rpath=$(TOP)lib

LDFLAGS:= -L $(TOP)lib 

LDFLAGS+= -lbaseinfo_collect

include $(TOP)make.env
