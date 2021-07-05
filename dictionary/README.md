
## dictionary
`dictionary`是一个利用金山词霸接口查询单词的自用小工具. 在线查询结果会保存到 "~/.dictionary.sqlite", 下次查询同一内容直接读数据库, 通常情况此功能意义不大, 但提供了"查过就是我的了!"的爽快感, 同时无视了线上的更新, 线下同样丰富多彩

```
Usage of dictionary:
    -e  external 显示外部读音 URL
    -s  sound 在线获取读音 (need sox)
    -w string
        word 指定要查询的单词

对于 '-s' 选项, 需安装 Sox (Sound eXchange), 才能在 terminal 中播放声音

推荐把 wd 和编译后的 dictionary 加入 PATH 环境变量, 使用 wd 查词不用每次都输入 '-w'
```

![image](https://github.com/PunkSnail/armory/blob/master/images/dictionary_demonstration.jpg)
