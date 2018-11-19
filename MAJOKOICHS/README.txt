PAL.dll中存在两种字符画图机制
1. 从内置DEFAULT_FONT.dat中提取字符位图并画图
2. 创建字体并调用GetGlyphOutlineA取得位图并画图

[机制一补丁方法]
1.将中文编码映射到日文编码
2.在PAL.0x10008010中，将从DEFAULT_FONT读出来的位图gm信息修改为对应汉字的GetGlyphOutline的信息（6个UINT）
3.在PAL.0x1001BED0中将buf中的位图信息改为对应汉字的位图信息。

[机制二补丁方法(这个简单，采用这个)]
1.在PalFontBegin中修改全局句柄中的字体编码值为0/1，使程序进入机制2
2.修改PAL.0x10008380中的字体结构体信息，charset=0x86
3.修改Pal.dll中的字体名称字符串。

=======================================
//首先调用PalFontBegin做准备
PalFontBegin@0x10009150

获取全局设备句柄
.text:10009153                 mov     edx, dword_15117124 

设备句柄偏移 0x1C8处记录了字体编号0/1/4 
0&1代表两种可自定义的字体，使用机制2
4代表使用DEFAULT_FONT，使用引擎内置画图机制，机制1
.text:10009159                 movzx   ecx, word ptr [edx+1C8h]

如果是0&1，调用PAL.0x10008380，
PAL.0x10008380填写字体结构体信息，并调用CreateFontA创建字体
打印一句话创建一次字体
.text:100091AE                 call    sub_10008380 @PalFontBegin

如果是4，不调用PAL.0x10008380

======================================

PalFontBegin返回后调用PalFontDrawText画图
PalFontDrawText@0x10009020

提取字符位图和画图功能由PAL.0x10008AD0实现
.text:10009070                 call    sub_10008AD0

======================================
PAL.0x10008AD0

如果为0&1，调用GetGlyphOutlineA得到位图
.text:10008B80                 call    esi ; GetGlyphOutlineA

如果为4，调用PAL.0x10008010从DEFAULT_FONT中提取位图并将位图放入新的buf中
并更新GLYPHMETRICS(gm)结构体
.text:10008C00                 call    sub_10008010

字符编码边界检查(0x10008C0E-0x10008C50)

画图
======================================
PAL.0x10008010

从DEFAULT_FONT的buf中算出字符位图的偏移offset
EAX = charcode 
offset1 =[EBX + (AH-0x80)*0xFF+AL]
offset = [EBX + offset1]

每个字符位图包含header和数据本体，header中有6个uint
uint a;//最小包络矩形宽
uint b;//最小包络矩形高度（实际字体高度）
uint c;//originX 字符左上角距原点水平距离
uint d;//originY 字符左上角距原点垂直距离
uint e = 0x1C;//到下一个字符水平距离
uint f;//数据本体字节数 

f/b =实际字体矩形包络宽度a' >= a
比较一下PAL.0x1001BED0的二重循环a'&b'和a&b的关系
二重循环中的宽（内层循环）= f/b = a' >= a
高度（外重循环）= b - 1

调用PAL.0x1001BED0将位图信息放入新的buf中

=====================================
PAL.0x1001BED0

二重循环将位图信息放入新的buf，新的buf大小有0x1000字节可以使用
0x1001C02B: mov byte ptr ds:[eax], bl 

从0x1001C03D前最后一个sub推断内重循环（宽）的控制变量在SS:[EBP-4]
从0x1001C052前最后一个sub推断外重循环（高）的控制变量在SS:[EBP-10]

=====================================





							
