
# ARMORY

## common
常用功能库

- `utils_daemon.h` 成为守护进程

- `utils_library.h` 用符号加载动态库

- `snail_queue` go 环形队列

## information_station
功能不重要, 重点是作为参考, 支持多级目录 Makefile 的一种写法 :)

## formatted_text

格式化文本文件

- `-f <file>` 指定要处理的文件

- `-e, --eat` 删除文本文件中行末的空格与制表符 (相当于在 vim 末行模式中输入 :%s/\s\+$//g)
  - 之前: "void func(void); &nbsp; &nbsp; "
  - 之后: "void func(void);"

- `-i, --insert` 为在文本中出现的 ASCII 字符添加空格, 具体指 32~127 之间, 其他符号未处理
  - 之前: "我的Raspberry Pi计算圆周率 10000 位用了221秒"
  - 之后: "我的 Raspberry Pi 计算圆周率 10000 位用了 221 秒"

- `-c, --check` 与格式化类型配合使用, 判断是否需要格式化

- `-h, --help` 更多其它功能说明

- 对齐与长度限制
  - 在做了

## commands_loop
`boring_tools/shell_script/commands_loop.sh`是一个可以灵活设定单条命令执行时间和次数的 shell 脚本

```
Options:
  -c '"<cmd>"'  Commands to execute, content needs to
                be enclosed in quotation marks: (\") or ('").
  -s <num>      Seconds between each execution.
  -t <num>      Variable number of loops.
  --help        Display this information.
```

## dictionary
`dictionary`是一个利用金山词霸接口查询单词的自用小工具. 在线查询结果会保存到 "~/.dictionary.sqlite", 下次查询同一内容直接读数据库, 通常情况此功能意义不大, 但提供了"查过就是我的了!"的爽快感, 同时无视了线上的更新, 线下同样丰富多彩

```
Usage of dictionary:
    -e  显示外部读音 URL
    -s string
        指定要查询的单词

为方便使用, 推荐在 ~/.bashrc 中加入 "alias wd='dictionary -s'"
```
