[补丁编译方法]
1. buildpatch.cpp 与拆包后的TEXT.dat以及汉化后的文本TEXTchs.dat放在一个目录下编译运行生成patch.h
2. patch.h和patch.cpp放在一个工程下编译生成patch.dll
3. patch.dll和MAJOKOIchs.exe放在原游戏目录下; pal.dll替换原来的pal.dll

[待解决问题]
画面震动时不能更新文本
