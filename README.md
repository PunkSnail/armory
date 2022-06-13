
# ARMORY

## common
常用功能库

- `utils_daemon.h` 成为守护进程

- `utils_library.h` 用符号加载动态库

- `utils_singleton` 辅助实现单例进程

- `snail_queue` go 环形队列

- `easy_log` 线程安全的简单日志
```
easy_log 输出示例:

  2021/07/19 14:51:23 [DEBUG] 😅
  2021/07/19 14:51:24 [ERROR] laugh
```

## formatted_text

格式化文本文件

- `-f <file>` 指定要处理的文件

- `-e, --eat` 删除文本文件中行末的空格与制表符 (相当于在 vim 末行模式中输入 `%s/\s\+$//g`)
  - 之前: "void func(void); &nbsp; &nbsp; "
  - 之后: "void func(void);"

- `-i, --insert` 为在文本中出现的 ASCII 字符添加空格, 具体指 32~127 之间, 其他符号未处理
  - 之前: "我的Raspberry Pi计算圆周率 10000 位用了221秒"
  - 之后: "我的 Raspberry Pi 计算圆周率 10000 位用了 221 秒"

- `-c, --check` 与格式化类型配合使用, 判断是否需要格式化

- `-h, --help` 更多其它功能说明

## commands_loop
`boring_tools/commands_loop.sh`是一个可以灵活设定单条命令执行时间和次数的 shell 脚本

```
Options:
  -c '"<cmd>"'  Commands to execute, content needs to
                be enclosed in quotation marks: (\") or ('").
  -s <num>      Seconds between each execution.
  -t <num>      Variable number of loops.
  --help        Display this information.
```

## byte_convert
`boring_tools/byte_convert`字节转换工具

```
Options:
   <number>		Specify the number of bytes to be converted
                Example: "./byte_convert 1048576" or "./byte_convert 0x100000"
   --help		Display this information
```

## dictionary
`dictionary`是一个利用金山词霸接口查询单词的自用小工具. 在线查询结果会保存到 "~/.dictionary.sqlite", 下次查询同一内容直接读数据库, 通常情况此功能意义不大, 但提供了"查过就是我的了!"的爽快感, 同时无视了线上的更新(-o 强制在线查询, 并更新本地内容), 线下同样丰富多彩

```
Usage of dictionary:
    -e  external 显示外部读音 URL
    -s  sound 在线获取读音 (need sox)
    -o  online 获取线上非本地结果
    -w string
        word 指定要查询的单词

对于 "-s" 选项, 需安装 Sox (Sound eXchange), 才能在 terminal 中播放声音

推荐把 wd 和编译后的 dictionary 加入 PATH 环境变量, 使用 wd 查词不用每次都输入 '-w'
```

![image](https://github.com/PunkSnail/armory/blob/master/images/dictionary_demonstration.jpg)
