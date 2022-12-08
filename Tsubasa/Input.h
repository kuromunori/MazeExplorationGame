#ifndef DEF_INPUT_H
#include "DxLib.h"
#define DEF_INPUT_H

bool IfMouse(int MOUSE_INPUT);
bool IfDoubleClicked();
void GetMouse();
int gpUpdateKey();
int GetKey(int KeyHandle);
bool LongPushKey(int KeyHandle);
#endif