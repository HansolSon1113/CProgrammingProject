#include "sounds.h"

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#pragma comment(lib, "winmm.lib")
#define WM_APP_SETVOLUME (WM_APP + 1)

static HANDLE hReady;
static DWORD bgmTid;

DWORD WINAPI BgmThread(void *arg)
{
    MSG dummy;
    PeekMessage(&dummy, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    SetEvent(hReady);

    const char *path = arg;
    char cmd[128];

    sprintf(cmd, "open \"%s\" type mpegvideo alias bgm", path);
    if (mciSendStringA(cmd, NULL, 0, NULL))
        return 0;

    mciSendStringA("play bgm repeat", NULL, 0, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (msg.message == WM_APP_SETVOLUME)
        {
            sprintf(cmd, "setaudio bgm volume to %d", (int)msg.wParam);
            mciSendStringA(cmd, NULL, 0, NULL);
        }
        else if (msg.message == WM_QUIT)
        {
            break;
        }
        else
        {
            DispatchMessage(&msg);
        }
    }

    mciSendStringA("stop bgm", NULL, 0, NULL);
    mciSendStringA("close bgm", NULL, 0, NULL);
    return 0;
}

void SetVolume(int volume)
{
    PostThreadMessage(GetThreadId(hBgmThread), WM_APP_SETVOLUME, (WPARAM)volume, 0);
}

void StartBgm(const char *path)
{
    hReady = CreateEvent(NULL, TRUE, FALSE, NULL);
    hBgmThread = CreateThread(NULL, 0, BgmThread, (void *)path, 0, &bgmTid);
    WaitForSingleObject(hReady, INFINITE);
}

void StopBgm(void)
{
    if (hBgmThread)
    {
        PostThreadMessage(bgmTid, WM_QUIT, 0, 0);
        WaitForSingleObject(hBgmThread, INFINITE);
        CloseHandle(hBgmThread);
        hBgmThread = NULL;
    }
}

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
    LoadWav("sounds/selection.wav", &selectionSound);
}

void PlaySelection(void)
{
    PlaySoundA((LPCSTR)selectionSound.buf, NULL, SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
}

void CloseAudio(void)
{
    if (selectionSound.buf)
    {
        free(selectionSound.buf);
        selectionSound.buf = NULL;
        selectionSound.len = 0;
    }
}