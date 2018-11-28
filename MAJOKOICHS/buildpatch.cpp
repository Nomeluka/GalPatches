#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;

int main() {
	
	FILE* dst = fopen("patch.h","wb+");
	
	FILE* p1 = fopen("TEXT.dat","rb");
	fseek(p1,0,SEEK_END);
	int len1 = ftell(p1);
	fseek(p1,0,SEEK_SET);
	uint8_t* buf1 = new uint8_t[len1];
	fread(buf1,len1,1,p1);
	fclose(p1);
	
	char uc[2];
	fwrite("static unsigned char ordermap[2*1024*1024]={",1,44,dst);
	for(int i = 0; i<len1 ;i++){
		itoa(buf1[i],uc,16);
		if(uc[1]==0x00){
			uc[1] = uc[0];
			uc[0] = '0';
		}
		if(i%1000!=0){
			fwrite(",",1,1,dst);
		}
		
		fwrite("0x",1,2,dst);
		fwrite(uc,1,2,dst);
		
		if(i%1000 == 999){
			fwrite(",",1,1,dst);
			fwrite("\n",1,1,dst);
		}
	}
	fwrite("};\n",1,3,dst);
	
//writing chinese text entry
	uint8_t* buf2 = new uint8_t[2*1024*1024];
	uint32_t endp = 0;
	ifstream fin("TEXTchs.dat");
	string str;
	
	fwrite("static unsigned char chstext[80000][40]={",1,41,dst);
	fwrite("\n",1,1,dst);
	
	while(getline(fin,str)){
			
		const char* cs = str.c_str();
		int sl = strlen(cs);

		if(sl<40){
				
			for(int j=0;j<sl;j++){
					
				itoa((short)(unsigned char)cs[j],uc,16);
				if(uc[1]==0x00){
					uc[1] = uc[0];
					uc[0] = '0';
				}
				fwrite("0x",1,2,dst);
				fwrite(uc,1,2,dst);
				fwrite(",",1,1,dst);
			}	
				
			for(int j=0;j<40-sl;j++){
				fwrite("0x0,",1,4,dst);
			}
			fwrite("\n",1,1,dst);
		}else{
			
			fwrite("0x0,",1,4,dst);
			
			itoa(endp&0xff,uc,16);
			if(uc[1]==0x00){
				uc[1] = uc[0];
				uc[0] = '0';
			}
			fwrite("0x",1,2,dst);
			fwrite(uc,1,2,dst);
			fwrite(",0x",1,3,dst);
			itoa((endp>>8)&0xff,uc,16);
			if(uc[1]==0x00){
				uc[1] = uc[0];
				uc[0] = '0';
			}
			fwrite(uc,1,2,dst);
			fwrite(",0x",1,3,dst);
			itoa((endp>>24)&0xff,uc,16);
			if(uc[1]==0x00){
				uc[1] = uc[0];
				uc[0] = '0';
			}
			fwrite(uc,1,2,dst);
			fwrite(",0x",1,3,dst);
			itoa((endp>>24)&0xff,uc,16);
			if(uc[1]==0x00){
				uc[1] = uc[0];
				uc[0] = '0';
			}
			fwrite(uc,1,2,dst);
			fwrite(",",1,1,dst);
			
			for(int j=0;j<35;j++){
				fwrite("0x0,",1,4,dst);
			}
			fwrite("\n",1,1,dst);
			memcpy(buf2+endp,cs,sl);
			endp = endp + sl; 
			*(buf2+endp+1) = 0x00; 
			endp = endp + 1;
		}	
	}
	
	for(int j=0;j<39;j++){
		fwrite("0x0,",1,4,dst);
	}
	fwrite("0x0};\n",1,6,dst);
	
	
// write long text indexing buf
	fwrite("static unsigned char chstextbuf[2*1024*1024]={",1,46,dst);
	fwrite("\n",1,1,dst);
	
	for(int i=0;i<endp;i++){
		itoa((short)buf2[i],uc,16);
		if(uc[1]==0x00){
			uc[1] = uc[0];
			uc[0] = '0';
		}
		fwrite("0x",1,2,dst);
		fwrite(uc,1,2,dst);
		fwrite(",",1,1,dst);
		if(i%500 == 499){
			fwrite("\n",1,1,dst);
		}
	}
	
	fwrite("0x0};",1,5,dst);
	
	fin.close();
	delete buf2;
	delete buf1;
	fclose(dst);
}
