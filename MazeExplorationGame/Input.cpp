#include "DxLib.h"
#include "Input.h"


int Key[256];

int GetKey(int KeyHandle)
{
	return Key[KeyHandle];
}

int gpUpdateKey(){
        char tmpKey[256];
        GetHitKeyStateAll( tmpKey );
        for( int i=0; i<256; i++ ){ 
                if( tmpKey[i] != 0 ){ 
                        Key[i]++;  
               } else {              
                        Key[i] = 0;   
                }
        }
        return 0;
}

static int Mouse =GetMouseInput();
static int preMouse = 0;
static int Time = 0;

void GetMouse()
{
	preMouse = Mouse;
	Mouse = GetMouseInput();
}
bool IfMouse(int MOUSE_INPUT)
{

	if((Mouse & MOUSE_INPUT) && !(preMouse & MOUSE_INPUT))
		return true;
	else
		return false;
}
bool IfDoubleClicked()
{
	if(Time > 0)
	{
		Time--;
		if(IfMouse(MOUSE_INPUT_LEFT))
			return true;
		else
			return false;
	}
	else
	{
		if(IfMouse(MOUSE_INPUT_LEFT))
			Time = 30;
	}
	return false;
}

bool LongPushKey(int KeyHandle)
{
	if(GetKey(KeyHandle) >= 10 && GetKey(KeyHandle) % 4 == 0)
		return true;
	else
		return false;
}
