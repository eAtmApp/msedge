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
}
```
