# qqwry2txt
`qqwry2txt` 仅用于解析[纯真 IP 库镜像](https://github.com/wisdomfusion/qqwry.dat) 到 utf-8 编码的文件中, 代码参考[ C 解析 QQwry.dat](https://www.iteye.com/blog/hzy3774-1851364)

```
Options:
-f <file>       纯真 IP 库文件位置
-o <file>       解析后的输出位置, 默认 "./output.txt"
-h, --help      Display this information
```

# iplib_maker
`iplib_maker` 读取 `qqwry2txt` 生成的 txt(单行内容示例:"42.158.0.0|42.158.255.255|<描述内容>"), 构建 IP 库文件, 相较于[原始数据](https://github.com/wisdomfusion/qqwry.dat) 具有性能及通用性优势.
数据结构参考 [ip2region](https://github.com/lionsoul2014/ip2region), 头部长度不再固定, 并删除了 "city_id" 这样的字段, 仅保留 key (IP 区间) - value (描述内容), 复杂内容可转为 json 存入 value, 最大支持长度为 65535 字节

```
iplib 数据结构:

1. 顶部:
  +---------+---------+
  | 4 bytes | 4 bytes |
  +---------+---------+
  索引区开始|索引区结束

2. 头部索引区: 不同于数据索引区, 类似于跳表, 默认每隔 680 个数据索引区元素
记录一次, 当数据不足 680, 也会拥有两个头部索引区元素, 记录头尾
  +---------+---------+
  | 4 bytes | 4 bytes | ...
  +---------+---------+
   start ip | offset

3. 数据区: 仅当数据索引区"数据长度"字段等于 0xFF 时, 数据区的"数据长度" 长
度字段才会存在
  +---------+----------------------+
  | 2 bytes | max 65535 bytes      | ...
  +---------+----------------------+
   数据长度 | 描述字符串

4. 数据索引区:
  +------------+-----------+--------+---------+
  | 4 bytes    | 4 bytes   | 1 byte | 3 bytes | ...
  +------------+-----------+--------+---------+
   start ip    |  end ip   |数据长度| 数据偏移

5. 尾部: 构造日期, "PUNK" 作为魔数结尾
  +---------------+
  | YY/MM/DD.PUNK |
  +---------------+
```
```
Options:
-f <file>	Source text file path
-o <file>	Write output to <file>; default "./iplib.db"
-h, --help	Display this information
```

# iplib_reader
`iplib_reader` 用于读取 `iplib_maker` 生成的 IP 库文件, 同时支持读取 [ip2region 库](https://github.com/lionsoul2014/ip2region), 查询特定 IP 地址的归属地及运营商信息

代码参考[ ip2region C 客户端](https://github.com/lionsoul2014/ip2region/tree/master/binding/c)

```
Options:
-f <file>       IP library file path
-s <ip>         The IP address to search
-h, --help      Display this information
```
