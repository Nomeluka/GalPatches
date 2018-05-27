#include <stdio.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <thread>
#include <iostream>
#include <windows.h>
#include <WinInet.h>
using namespace std;

void MAJO(){
	system("D:\\MAJO\\MAJOKOIchs.exe");
} 


char* jis2gbk(string& s){
	const char* strbuf = s.c_str();
	int num = MultiByteToWideChar(932,0,strbuf,-1,NULL,0);
	wchar_t* uni = new wchar_t[num];
	MultiByteToWideChar(932,0,strbuf,-1,uni,num);
	for(int i=0;i<num;i++){
		cout<<uni[i]<<" ";
	}
	cout<<endl;
	int num8 = WideCharToMultiByte(CP_UTF8, NULL, uni, -1, NULL, 0, NULL, NULL);  
    char *utf8;  
    utf8 = new char[num8];  
    WideCharToMultiByte(CP_UTF8,NULL,uni,-1, utf8,num8,NULL,NULL);
    for(int i=0;i<num8;i++){
    	cout<<hex<<short((unsigned char)(utf8[i]))<<" ";
	} 
	cout<<endl;
	delete uni;
	return utf8;
}
void Control(){
	/*
	FILE* f; 
	
	while(1){
		
		while(1){
			f = fopen("D:\\MAJO\\record.dat","r");
			if(f!=NULL){
				break;
			}
		}
		fseek (f , 0 , SEEK_END);
        int l = ftell (f);
        char* buf = new char[l];
        int res = fread(buf,1,l,f);
		if(l>3){
			cout<<buf<<endl;
			fclose(f);
			f = fopen("D:\\MAJO\\record.dat","w+");
			fclose(f);
		}else{
			fclose(f);
		}
	}
	*/
	ifstream fi;
	while(1){
		fi.open("record.dat",ios::in);
		if(fi.is_open()){
			string s;
			getline(fi,s);
			int l = s.length();
	        if(l>0){
	        	//cout<<s<<endl;
	        char* utf8 = jis2gbk(s);
			fi.close();
			FILE* f;
			f = fopen("record.dat","w+");
			fclose(f);
			}
			fi.close();
			Sleep(200);
		}else{
			//cout<<"deny"<<endl;
		}

	}
	
}
int main(){
	
	
	thread game(MAJO);
	thread console(Control); 
	
	game.join();
	console.join();
	return 0;
}
