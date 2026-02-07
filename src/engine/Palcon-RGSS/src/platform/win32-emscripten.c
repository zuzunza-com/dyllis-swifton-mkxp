/*
** win32-emscripten.c
**
** Win32 API compatibility layer implementation for Emscripten
**
** Copyright (C) 2025 Palcon Team
*/

#ifdef __EMSCRIPTEN__

#include "win32-emscripten.h"
#include <emscripten.h>
#include <emscripten/fetch.h>
#include <errno.h>

static DWORD g_lastError = 0;

DWORD GetLastError(void)
{
    return g_lastError;
}

void SetLastError(DWORD dwErrCode)
{
    g_lastError = dwErrCode;
}

/* File I/O using Emscripten FS API */
HANDLE CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
                   void* lpSecurityAttributes, DWORD dwCreationDisposition,
                   DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
    int flags = 0;
    int mode = 0666;

    if (dwDesiredAccess & 0x80000000) { /* GENERIC_READ */
        flags |= O_RDONLY;
    }
    if (dwDesiredAccess & 0x40000000) { /* GENERIC_WRITE */
        flags |= O_WRONLY | O_CREAT;
    }

    switch (dwCreationDisposition) {
        case 1: /* CREATE_NEW */
            flags |= O_CREAT | O_EXCL;
            break;
        case 2: /* CREATE_ALWAYS */
            flags |= O_CREAT | O_TRUNC;
            break;
        case 3: /* OPEN_EXISTING */
            break;
        case 4: /* OPEN_ALWAYS */
            flags |= O_CREAT;
            break;
        case 5: /* TRUNCATE_EXISTING */
            flags |= O_TRUNC;
            break;
    }

    int fd = open(lpFileName, flags, mode);
    if (fd < 0) {
        SetLastError(errno);
        return INVALID_HANDLE_VALUE;
    }

    return (HANDLE)(intptr_t)fd;
}

BOOL ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead,
              DWORD* lpNumberOfBytesRead, void* lpOverlapped)
{
    int fd = (int)(intptr_t)hFile;
    ssize_t bytesRead = read(fd, lpBuffer, nNumberOfBytesToRead);

    if (bytesRead < 0) {
        SetLastError(errno);
        if (lpNumberOfBytesRead) *lpNumberOfBytesRead = 0;
        return FALSE;
    }

    if (lpNumberOfBytesRead) *lpNumberOfBytesRead = bytesRead;
    return TRUE;
}

BOOL WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,
               DWORD* lpNumberOfBytesWritten, void* lpOverlapped)
{
    int fd = (int)(intptr_t)hFile;
    ssize_t bytesWritten = write(fd, lpBuffer, nNumberOfBytesToWrite);

    if (bytesWritten < 0) {
        SetLastError(errno);
        if (lpNumberOfBytesWritten) *lpNumberOfBytesWritten = 0;
        return FALSE;
    }

    if (lpNumberOfBytesWritten) *lpNumberOfBytesWritten = bytesWritten;
    return TRUE;
}

BOOL CloseHandle(HANDLE hObject)
{
    int fd = (int)(intptr_t)hObject;
    if (close(fd) < 0) {
        SetLastError(errno);
        return FALSE;
    }
    return TRUE;
}

DWORD GetFileSize(HANDLE hFile, DWORD* lpFileSizeHigh)
{
    int fd = (int)(intptr_t)hFile;
    struct stat st;

    if (fstat(fd, &st) < 0) {
        SetLastError(errno);
        return 0;
    }

    if (lpFileSizeHigh) *lpFileSizeHigh = (st.st_size >> 32) & 0xFFFFFFFF;
    return (DWORD)(st.st_size & 0xFFFFFFFF);
}

/* Memory allocation using Emscripten memory API */
LPVOID VirtualAlloc(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect)
{
    (void)lpAddress; /* Emscripten doesn't support specific address allocation */
    (void)flAllocationType;
    (void)flProtect;
    return malloc(dwSize);
}

BOOL VirtualFree(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType)
{
    (void)dwSize;
    (void)dwFreeType;
    free(lpAddress);
    return TRUE;
}

LPVOID HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes)
{
    (void)hHeap;
    (void)dwFlags;
    return malloc(dwBytes);
}

BOOL HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem)
{
    (void)hHeap;
    (void)dwFlags;
    free(lpMem);
    return TRUE;
}

/* Threading - mapped to async operations */
HANDLE CreateThread(void* lpThreadAttributes, SIZE_T dwStackSize,
                    LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter,
                    DWORD dwCreationFlags, unsigned long* lpThreadId)
{
    (void)lpThreadAttributes;
    (void)dwStackSize;
    (void)dwCreationFlags;
    (void)lpThreadId;

    /* In Emscripten, we use async operations instead of threads */
    /* For now, execute synchronously */
    if (lpStartAddress) {
        lpStartAddress(lpParameter);
    }
    return (HANDLE)1;
}

DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds)
{
    (void)hHandle;
    (void)dwMilliseconds;
    /* In Emscripten, async operations complete immediately */
    return 0; /* WAIT_OBJECT_0 */
}

/* Window/Canvas operations */
BOOL GetWindowRect(HANDLE hWnd, RECT* lpRect)
{
    (void)hWnd;
    if (!lpRect) return FALSE;

    /* Get canvas dimensions */
    int width, height;
    emscripten_get_canvas_element_size("#canvas", &width, &height);

    lpRect->left = 0;
    lpRect->top = 0;
    lpRect->right = width;
    lpRect->bottom = height;

    return TRUE;
}

BOOL SetWindowPos(HANDLE hWnd, HANDLE hWndInsertAfter, int X, int Y,
                  int cx, int cy, UINT uFlags)
{
    (void)hWnd;
    (void)hWndInsertAfter;
    (void)X;
    (void)Y;
    (void)uFlags;

    /* Set canvas size */
    if (cx > 0 && cy > 0) {
        emscripten_set_canvas_element_size("#canvas", cx, cy);
    }

    return TRUE;
}

#endif /* __EMSCRIPTEN__ */
