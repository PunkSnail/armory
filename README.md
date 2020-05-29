
# ARMORY

关于这个库存在的意义, 可以概括为"无聊时间里的产物", 而总有一天, 我会把它带出无聊的范畴


## formatted_text

格式化文本文件

- `-e <file>` 删除文本文件中行末的空格与制表符, 多用于格式化代码
  - 之前: "void func(void); &nbsp; &nbsp; "
  - 之后: "void func(void);"

- `-f <file>` 为在文本中出现的 ASCII 字符添加空格, 具体指 32~127 之间, 其他符号未处理
  - 之前: "我的Raspberry Pi计算圆周率 10000 位用了221秒"
  - 之后: "我的 Raspberry Pi 计算圆周率 10000 位用了 221 秒"

- `-b, --backup` 修改文件`xxx`之前会备份到`xxx.bak`

- `-h, --help` 功能说明

- 对齐与长度限制
  - 在做了
