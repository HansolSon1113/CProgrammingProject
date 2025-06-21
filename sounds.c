#include "sounds.h"

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#pragma comment(lib, "winmm.lib")

DWORD WINAPI BgmThread(void *arg)
{
    const char *path = arg; // mp3 · wav 모두 가능
    char cmd[128];

    sprintf(cmd, "open \"%s\" type waveaudio alias bgm", path);
    if (mciSendStringA(cmd, NULL, 0, NULL))
        return 0;

    sprintf(cmd, "setaudio bgm volume to %d", 500);
    mciSendStringA(cmd, NULL, 0, NULL);

    mciSendStringA("play bgm repeat", NULL, 0, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
        DispatchMessage(&msg);

    mciSendStringA("stop bgm", NULL, 0, NULL);
    mciSendStringA("close bgm", NULL, 0, NULL);
    return 0;
}

void SetVolume(int volume)
{
    char command[64];

    sprintf(command, "setaudio bgm volume to %d", volume);
    mciSendStringA(command, NULL, 0, NULL);
}

void StartBgm(const char *path)
{
    if (!hBgmThread)
        hBgmThread = CreateThread(NULL, 0, BgmThread, (void *)path, 0, NULL);
}

void StopBgm(void)
{
    if (hBgmThread)
    {
        PostThreadMessage(GetThreadId(hBgmThread), WM_QUIT, 0, 0);
        WaitForSingleObject(hBgmThread, INFINITE);
        CloseHandle(hBgmThread);
        hBgmThread = NULL;
    }
}

WavData walkSound = {0};
WavData selectionSound = {0}; 

static void LoadWav(const char *path, WavData *out)
{
    HANDLE hFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD sz = GetFileSize(hFile, NULL);
    out->buf = malloc(sz);
    ReadFile(hFile, out->buf, sz, &out->len, NULL);
    CloseHandle(hFile);
}

void InitAudio(void)
{
    LoadWav("sounds/jump.wav", &walkSound);
    LoadWav("sounds/selection.wav", &selectionSound);
}

void PlayWalk(void)
{
    PlaySoundA((LPCSTR)walkSound.buf, NULL, SND_MEMORY | SND_ASYNC | SND_NODEFAULT); //효과음에 프레임 정지 안하고 파일 입출력 안하게
}

void PlaySelection(void)
{
    PlaySoundA((LPCSTR)selectionSound.buf, NULL, SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
}