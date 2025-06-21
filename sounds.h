#ifndef SOUNDS_H
#define SOUNDS_H

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

HANDLE hBgmThread;

typedef struct
{
    BYTE *buf;
    DWORD len;
} WavData;

void SetVolume(int volume);
void StartBgm(const char *path);
void StopBgm(void);
void InitAudio(void);
void CloseAudio(void);

#endif