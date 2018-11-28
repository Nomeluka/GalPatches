#include <windows.h>
#include <iostream>
#include <stdio.h>
#include "patch.h"

static unsigned int a;//text start
static unsigned int b;//text end 
					 //Narrator 0x0fffffff

static unsigned int script_pos;

INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved) {

    switch(Reason) {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void SetOriginOrder(unsigned int ay){
	script_pos = *(unsigned int*)(ordermap+ay);
}

unsigned char* GetChsName(){
	return chstext[script_pos+1];
}

unsigned char* GetChsText(){
	if(chstext[script_pos][0]!=0x00){
		return chstext[script_pos];
	}
	else{
		return chstextbuf + (*(unsigned int*)(chstext[script_pos]+1));
	}
}

extern "C" __declspec(dllexport) void signal(unsigned int pos1, unsigned int pos2){
	b = pos1;
	a = pos2;
	SetOriginOrder(a);
}

extern "C" __declspec(dllexport) void rewriteTXT(unsigned int text_end) {
	unsigned int tail;
	unsigned int base;
	unsigned int pos_relocate2;
	__asm{
		mov eax, dword ptr ss:[text_end]; 
		mov tail,eax;
		mov dl, byte ptr ds:[eax - 1];
		cmp dl, 0x00;
		jz f
l:
		dec eax;
		mov dl, byte ptr ds:[eax];
		cmp dl,0x00;
		jnz l
f:
		mov base, eax;

		inc eax;
		mov edx, dword ptr ds:[eax-0x174];
		mov pos_relocate2, edx;
		mov dl, byte ptr 0x00;
	}


	SetOriginOrder(pos_relocate2);

	unsigned char* pstr = GetChsText();

	int len = 0;
	for(len=0;;len++){
		if(pstr[len]== 0x00){
			len += 1 ;
			break;
		}
	}

	memcpy((unsigned char*)(base+1),pstr,len);
}

extern "C" __declspec(dllexport) void rewriteNM(unsigned int name_start) {
	if(b == 0xfffffff){
		return;
	}
	else{

		unsigned int base;
		unsigned int pos_relocate1;

		__asm{
			mov eax, dword ptr ss:[name_start];
			mov dword ptr base, eax;

			mov edx, dword ptr ds:[eax - 0x70];
			mov pos_relocate1, edx;
		}

		SetOriginOrder(pos_relocate1);

		unsigned char* pstr = GetChsName();

		int len = 0;
		for(len=0;;len++){
			if(pstr[len]== 0x00){
				len += 1 ;
				break;
			}
		}
		
		memcpy((unsigned char*)base,pstr,len);
	}
}