/*
** win32-emscripten.h
**
** Win32 API compatibility layer for Emscripten
** Maps Win32 API calls to Emscripten/browser APIs
**
** Copyright (C) 2025 Palcon Team
*/

#ifndef WIN32_EMSCRIPTEN_H
#define WIN32_EMSCRIPTEN_H

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <emscripten/html5.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Win32 types */
typedef void* HANDLE;
typedef int BOOL;
typedef unsigned long DWORD;
typedef void* LPVOID;
typedef const void* LPCVOID;
typedef const char* LPCSTR;
typedef char* LPSTR;
typedef unsigned long ULONG;
typedef long LONG;
typedef struct {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT;

#define INVALID_HANDLE_VALUE ((HANDLE)-1)
#define TRUE 1
#define FALSE 0
#define NULL 0

/* File I/O */
HANDLE CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                   void* lpSecurityAttributes, DWORD dwCreationDisposition,
                   DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

BOOL ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead,
              DWORD* lpNumberOfBytesRead, void* lpOverlapped);

BOOL WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,
               DWORD* lpNumberOfBytesWritten, void* lpOverlapped);

BOOL CloseHandle(HANDLE hObject);

DWORD GetFileSize(HANDLE hFile, DWORD* lpFileSizeHigh);

/* Memory */
LPVOID VirtualAlloc(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
BOOL VirtualFree(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);

LPVOID HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
BOOL HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);

/* Threading (mapped to Emscripten async operations) */
typedef void* HANDLE;
typedef unsigned long (*LPTHREAD_START_ROUTINE)(LPVOID lpThreadParameter);

HANDLE CreateThread(void* lpThreadAttributes, SIZE_T dwStackSize,
                    LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter,
                    DWORD dwCreationFlags, unsigned long* lpThreadId);

DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);

/* Window/Canvas (mapped to HTML5 Canvas) */
BOOL GetWindowRect(HANDLE hWnd, RECT* lpRect);
BOOL SetWindowPos(HANDLE hWnd, HANDLE hWndInsertAfter, int X, int Y,
                  int cx, int cy, UINT uFlags);

/* Error handling */
DWORD GetLastError(void);
void SetLastError(DWORD dwErrCode);

#endif /* __EMSCRIPTEN__ */

#endif /* WIN32_EMSCRIPTEN_H */
