编译MAJO_text.cpp生成MAJO_text.exe
编译TEXTHOOK.cpp生成ch.dll
将MAJO_text.exe & ch.dll & MAJOKOIchs.exe放到游戏根目录下
运行MAJO_text.exe（只实现日文文本提取）
运行MAJO.py(可翻译) 依赖库 --- requests execjs 

--- 正常运行MAJO.py需要在ipv6环境下



补丁过程
1.MAJOKOI.exe 关闭ASLR动态基址
2.MAJOKOI.exe 字符读入ECX后JMP到末尾NULLPADDING处注入代码，调用DLL，返回时JMP到程序控制流
3.ch.dll中将字符写入文件
4.MAJO.py线程读取输出文件数据
5.从translate.google.cn的返回内容中拿到TKK的生成函数
6.根据TKK和字符内容生成TK参数
7.google发包获得翻译内容


--- 还需解决问题 1.同步 2.特殊编码异常 3.JSON提取不全