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

extern "C" __declspec(dllexport) int entry(unsigned int ecx) {
	FILE *file=fopen("record.dat", "a+");
	char p[3];
	p[0]=(char)(ecx&0xff);
	p[1]=(char)((ecx>>8)&0xff);
	p[2]='\0';
	fprintf(file,p);
	fflush(file);
	fclose(file);
	
	unsigned short ecx_low = ecx&0xffff;

	if(ecx_low == 0x7681){
		int l=2;
		/*
		FILE *fpr =fopen("record.dat","r");
	    fseek (fpr , 0 , SEEK_END);
        l = ftell (fpr);
		fclose(fpr);
		*/
		while(l>0){
			Sleep(10);
			FILE *fpr =fopen("record.dat","r");
			fseek (fpr , 0 , SEEK_END);
			l = ftell (fpr);
			fclose(fpr);
		}
	}
    
	return 0;
}