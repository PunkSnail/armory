
# ARMORY

关于这个库存在的意义, 可以概括为"无聊时间里的产物", 而总有一天, 我会把它带出无聊的范畴

## common
常用功能库

- `utils_daemon.h` 成为守护进程

- `utils_library.h` 用符号加载动态库

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

## classic_sort

排序算法入门学习

- `-s <size>` 指定乱序数组大小, 默认 20

- `-a <algorithm>` 指定排序算法, 详见`--help`的输出, 默认全开

- `-h, --help` 更多其它功能说明

