#include <stdint.h> 
#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
using namespace std;

void extract(uint8_t* buf,int len){
	
	FILE* pfile = fopen("TEXTjp.dat","wb+");
	
	uint32_t base = 0x10;
	uint32_t* porder = (uint32_t*)(buf+base);
	uint32_t order = *porder;
	
	int i = base + 4;
	int j = 0;
	

	cout<<hex<<len<<endl;;
	int num = 0; 
	while(i < len){
		j = 0;
		while(j<len){
			if(buf[i+j] == 0){
				
				if((i+j)!= len){
					porder = (uint32_t*)(buf + i + j + 1);
					if((*porder - order != 0) && (*porder - order != 1) ){
						cout<<"i+j"<<endl;
					}
					if(j > 14){
						num ++ ;
					}
					fwrite(buf+i,1,j,pfile);
					fwrite("\n",1,1,pfile);
					order = *porder;
					i = i + j + 5;
					break;
				}else{
					i = i+j;
					i++;
					break;
				}
			}else{
				//if(i+j>len-10)
				//	cout<<hex<<(short)buf[i+j]<<" ";
				j ++ ;
			}
		}
		
	}
	
	fclose(pfile);
	cout<<dec<<num;
}

uint8_t ror(uint8_t val, uint8_t count)
{
	int bitcount =   8;
	count %= bitcount;
	return (val >> count) | (val << (bitcount - count));
}

void encryptscript(uint8_t* buf, int len){
	
	uint32_t key1 = 0x084DF873;
	uint32_t key2 = 0xFF987DEE;
	uint32_t key = key1^key2;
	
	uint32_t base = 0x10;
	uint32_t* p;
	uint32_t pp;
	uint32_t count = (len-0x10)/4 + 1;
	int c = 0x04;
	
	for(int i= 0; i < count; i++){
		p = (uint32_t*)(buf + base + 4*i);
		pp = *p;
		pp ^= key;
		pp = ror(pp,c)+(pp&0xffffff00);
		*(int32_t*)(buf+base+4*i) = pp;
		c++;
		c&=0xFF;
	}
	
}

uint8_t* buildscript(uint8_t* buf,int len, uint32_t* clen){
	
	ifstream fin("TEXTchs.dat");//汉化文本 
	
	uint8_t* bufchs = new uint8_t[len];//重建脚本buf 
	
	uint32_t bufchsend = 0;
	
	strcat((char*)bufchs, "$TEXT_LIST__");
	bufchsend += 12;
	
	*(uint32_t*)(bufchs+bufchsend) = 0x00011816;
	bufchsend += 4;
	
	/*
	for(int i=0;i<0x20;i++){
		cout<<hex<<(short)(bufchs[i])<<" ";
	}
	*/
	
	string s;
	uint32_t base = 0x10;
	uint32_t* porder = (uint32_t*)(buf+base);
	uint32_t order = *porder;
	
	int i = base + 4;
	int j = 0;
	
	while(i < len){
		j = 0;
		while(j<len){
			if(buf[i+j] == 0){
				if((i+j)!= len){
					porder = (uint32_t*)(buf + i + j + 1);
					
					*(uint32_t*)(bufchs + bufchsend) = order;
					bufchsend += 4;
					
					getline(fin,s);
					const char* cs = s.c_str();
					memcpy(bufchs+bufchsend,cs,strlen(cs));
					bufchsend += strlen(cs);
					
					*(bufchs+bufchsend) = 0x00;
					bufchsend ++;
					
					order = *porder;
					i = i + j + 5;
					break;
				}else{
					i = i+j;
					i++;
					break;
				}
			}else{
				j ++ ;
			}
		}
		
	}
	
	cout<<hex<<"len"<<bufchsend<<endl;
	*clen = bufchsend;
	fin.close();
	return bufchs;
	//encrypt(buf,len);
	
}

void writescript(uint8_t* bufchs,int len){
	FILE* pdata = fopen("data_jp.pac","rb");
	fseek(pdata,0,SEEK_END);
	int datalen = ftell(pdata);
	uint8_t* databuf = new uint8_t[datalen];
	fseek(pdata,0,SEEK_SET);
	fread(databuf,datalen,1,pdata);
	cout<<hex<<datalen<<endl;
	fclose(pdata);
	
	memcpy(databuf+0xbb8663,bufchs,len);
	int diff = datalen - 0xda4974;
	memcpy(databuf+0xbb8663+len,databuf+0xda4974,diff);
	
	*(uint32_t*)(databuf+0x2354) = len;
	
	(uint8_t*)(databuf+0xbb8663+len-1) = 0x00;
	
	for(int i=0; i<29;i++){
		uint32_t start = *(uint32_t*)(databuf+0x2380+0x50*i);
		cout<<hex<<start<<endl;
		*(uint32_t*)(databuf+0x2380+0x50*i) = start - (0x1ec311 - len);
	}
	
	FILE* data = fopen("data.pac","wb+");
	
	fwrite(databuf,datalen,1,data);
	fclose(data);
	delete databuf;
}

void buildpac(uint8_t* buf, int len){
	uint8_t* bufchs; 
	uint32_t chslen;
	bufchs = buildscript(buf,len,&chslen);
	//cout<<hex<<chslen;
	encryptscript(bufchs,len);
	writescript(bufchs,chslen);
	delete bufchs;
}
 
int main(){
	FILE* pfile = fopen("TEXT.dat","rb");
	fseek(pfile,0,SEEK_END);
	int textlen = ftell(pfile);
	fseek(pfile,0,SEEK_SET);
	uint8_t* buf = new uint8_t[textlen];
	fread(buf,textlen,1,pfile);
	fclose(pfile);
	buildpac(buf,textlen);
	delete buf;
	return 0;
} 
