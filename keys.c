#include "keys.h"

#include <windows.h>
#pragma comment(lib, "user32.lib")

Keys GetKeys(void)
{
    Keys k;
    k.left = GetAsyncKeyState('A') & 0x8000;
    k.right = GetAsyncKeyState('D') & 0x8000;
    k.jump = GetAsyncKeyState(VK_SPACE) & 0x8000;
    
    return k;
}