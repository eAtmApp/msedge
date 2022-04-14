# msedge
让小娜搜索网页时打开默认浏览器

```
//配置文件config.json
{

    //edge原路径.
    "edge_path":"C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe"
    
    //需要替换的注册表值.
    ,"reg_paths":[
        {
            "path":"计算机\\HKEY_CLASSES_ROOT\\MSEdgeHTM\\shell\\open\\command",
            "name":"",
            "value":"\"[EXE]\" --single-argument %1"
        }
        ,{
            "path":"计算机\\HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\EdgeUpdate\\Clients\\{56EB18F8-B008-4CBD-B6D2-8C97FE7E9062}",
            "name":"location",
            "value":"[DIR]"
        }
    ]

    //搜索引擎设置
    ,"urls":{
        
        //默认搜索引擎
        "default":"https://www.google.com.hk/search?q=[KEY]",

        //百度, 搜索 bd:上海天气 将使用百度搜索上海天气.
        "bd":"https://www.baidu.com/s?ie=UTF-8&wd=[KEY]",

        //淘宝
        "tb":"https://s.taobao.com/search?q=[KEY]&imgfile=&commend=all&ssid=s5-e&search_type=item&sourceId=tb.index&ie=utf8"
    }
}
```
#代码仅供参考,自用库源代码并未上传.
