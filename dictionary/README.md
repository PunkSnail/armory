
## dictionary
`dictionary`是一个利用金山词霸接口查询单词的自用小工具. 在线查询结果会保存到 "~/.dictionary.sqlite", 下次查询同一内容直接读数据库, 通常情况此功能意义不大, 但提供了"查过就是我的了!"的爽快感, 同时无视了线上的更新, 线下同样丰富多彩

```
Usage of dictionary:
    -e  显示外部读音 URL
    -s string
        指定要查询的单词

为方便使用, 推荐在 ~/.bashrc 中加入 "alias wd='dictionary -s'"
```

![image](https://github.com/PunkSnail/armory/blob/master/images/dictionary_demonstration.jpg)
