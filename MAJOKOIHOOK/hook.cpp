#include <windows.h>
#include <iostream>
#include <stdio.h>

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved) {

    switch(Reason) {
    case DLL_PROCESS_ATTACH:
               std::cout<<"attached"<<std::endl;
        break;
    case DLL_PROCESS_DETACH:
		std::cout<<"detached"<<std::endl;
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) int entry(unsigned int edx,unsigned int ecx) {

	char* exp = "Unexcepted Character.\0";
	char buf[256];
	int cnum = 0;

	if(edx <= 0x100){
		//特殊字符情况
		strncpy(buf,exp,23);
		cnum = 23;
		//return 0; //直接返回，不输出错误信息
		
	}
	else{
		try{
			char* ptr = ((char*)(ecx-2));
			while(*ptr!=0){
				buf[cnum]=*ptr;
				cnum = cnum +1;
				ptr = ptr + 1;
				if(cnum>=255){
					break;
					//实际应该动态扩大缓冲区
				}
			}

			buf[cnum]='\0';

		}catch(...){
			strncpy(buf,exp,25);
		}
	}


	FILE* file;
	file = fopen("record.dat", "a+");
	while(file==NULL){
		Sleep(10);
		file=fopen("record.dat", "a+");
	}

	fprintf(file,buf);
	fflush(file);
	fclose(file);
	
	int len = cnum;

	while(len>0){
		Sleep(10);
		FILE* fpr;
		if(fpr = fopen("record.dat","r")){
			fseek (fpr,0,SEEK_END);
			len = ftell (fpr);
			fclose(fpr);
		}
		else{
			continue;
		}
	}
	
	return 0;
}