#-*- coding: utf-8 -*-
import sys
import os
from time import sleep
import threading
import math
import requests
import execjs
import re

url = "https://translate.google.cn/translate_a/single?client=t&sl=ja&tl=zh-CN&hl=zh-CN&dt=at&dt=bd&dt=ex&dt=ld&dt=md&dt=qca&dt=rw&dt=rm&dt=ss&dt=t&ie=UTF-8&oe=UTF-8&otf=1&ssel=4&tsel=0&kc=2"

def hex2chr(x):
	if x<10:
		return chr(x+48)
	else:
		return chr(x+55)
		
def hex2str(x):
	return "%"+hex2chr(math.floor(x/16))+hex2chr(x%16)
	
def TranslateUTF8(s):
	#UTF-8字节流=>%**%**字符串
	gs = ""
	for i in range(0,len(s)):
		gs = gs + hex2str(s[i])
	return gs


	
def MAJO():
	#启动游戏进程
	os.system("D:\\MAJO\\MAJOKOIchs.exe")
	
	
def Control():
	#补丁控制流
	se = requests.Session()
	urlg = "https://translate.google.cn/"
	#hds = {"upgrade-insecure-requests":"1","cache-control":"max-age=0","accept":"text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8","user-agent":"Mozilla/5.0 (Windows NT 6.2; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.181 Safari/537.36"}
	header = {
		'accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8',
		'accept-encoding':'gzip, deflate, br',
		'accept-language':'zh-CN,zh;q=0.9,es;q=0.8',
		'cache-control': 'max-age=0',
		'cookie': '_ga=GA1.3.1606382638.1527305816; _gid=GA1.3.991163329.1527305816; 1P_JAR=2018-5-27-15; NID=131=ZZjFP9OIw4BlfMEqzp8gozSaJu2-wTc-URCww1s1KA-0n-RnneMnxSba-xExoGMUA_lyTan4F8N-gNxO7RAECOEjZutCzMHQN8aRymATe9wcnY7Npw12OwsOHPq_oJIu',
		'upgrade-insecure-requests': '1',
		'user-agent': 'Mozilla/5.0 (Windows NT 6.2; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/66.0.3359.181 Safari/537.36',
		'x-client-data': 'CIu2yQEIo7bJAQjEtskBCKmdygEIqKPKAQ=='
	}
	r = requests.get(urlg,headers=header);
	t = r.text
	a = t.find("TKK")
	t = t[a:a+100]
	#print(t)
	r = re.findall(r"d[-,0-9]*;",t)
	rr = re.findall(r"return [-,0-9]*",t)
	#print(r)
	c  = rr[0][7:len(rr[0])]
	a  = r[0][1:len(r[0])-1]
	b  = r[1][1:len(r[1])-1]
	print(a,b,c)

	a = int(a)
	b = int(b)
	strTKK = c+"."+str(a+b)
	print("TKK = ",strTKK)
	print("=============")
	
	ctx = execjs.compile(""" 
	var b = function (a, b) {
		for (var d = 0; d < b.length - 2; d += 3) {
			var c = b.charAt(d + 2),
				c = "a" <= c ? c.charCodeAt(0) - 87 : Number(c),
				c = "+" == b.charAt(d + 1) ? a >>> c : a << c;
			a = "+" == b.charAt(d) ? a + c & 4294967295 : a ^ c
		}
		return a
	}

	var tk =  function (a,TKK) {
		for (var e = TKK.split("."), h = Number(e[0]) || 0, g = [], d = 0, f = 0; f < a.length; f++) {
			var c = a.charCodeAt(f);
			128 > c ? g[d++] = c : (2048 > c ? g[d++] = c >> 6 | 192 : (55296 == (c & 64512) && f + 1 < a.length && 56320 == (a.charCodeAt(f + 1) & 64512) ? (c = 65536 + ((c & 1023) << 10) + (a.charCodeAt(++f) & 1023), g[d++] = c >> 18 | 240, g[d++] = c >> 12 & 63 | 128) : g[d++] = c >> 12 | 224, g[d++] = c >> 6 & 63 | 128), g[d++] = c & 63 | 128)
		}
		a = h;
		for (d = 0; d < g.length; d++) a += g[d], a = b(a, "+-a^+6");
		a = b(a, "+-3^+b+-f");
		a ^= Number(e[1]) || 0;
		0 > a && (a = (a & 2147483647) + 2147483648);
		a %= 1E6;
		return a.toString() + "." + (a ^ h)
	}""")

	
	while 1:
		fi = open("D:\\MAJO\\record.dat",mode='r',encoding='shiftjis')
		if fi:
			sjis = fi.read()
			fi.close()

			if len(sjis)>0:
				#print(len(sjis))
				fii = open("D:\\MAJO\\record.dat",mode='w+',encoding='shiftjis')
				fii.truncate()
				print(sjis) #日文文本shift-JIS编码
				s = bytes(sjis,encoding = "utf-8")
				fii.close()
				gs = TranslateUTF8(s)
				tk = ctx.call("tk",sjis,strTKK)
				#print("tk = ",tk)
				res = requests.get(url+"&tk="+tk+"&q="+gs)
				#print("status_code = ",res.status_code)
				if res.status_code == "404":
					break
				
				txt = res.text;
				x1 = txt.find('\"')
				x2 = 0
				for i in range(x1+1,len(txt)):
					if txt[i]=="\"":
						x2 = i
						break;
				
				tl = txt[x1+1:x2]
				print(tl+"\n\n")
				
		else:
			fi.close()
		sleep(0.1)
				
			


if __name__ == '__main__':
	
	game = threading.Thread(target=MAJO)
	console = threading.Thread(target=Control)
	game.start()
	console.start()
	game.join()
	console.join()
	
			