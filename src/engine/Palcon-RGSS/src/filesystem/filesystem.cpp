/*
** filesystem.cpp
**
** Filesystem implementation for Palcon-RGSS (WASM)
** Uses Emscripten FS API
**
** Copyright (C) 2025 Palcon Team
*/

#include "filesystem.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/fetch.h>
#include <sys/stat.h>
#endif
#include <string.h>
#include <stdio.h>

Filesystem::Filesystem()
	: initialized(false)
{
}

Filesystem::~Filesystem()
{
}

bool Filesystem::init(const char* gamePath)
{
	// Initialize Emscripten filesystem
#ifdef __EMSCRIPTEN__
	// Mount game data directory
	// Emscripten FS API will be used to mount the game directory
	// TASK: Mount the actual game directory from gamePath
#ifdef __EMSCRIPTEN__
	// Create mount point if it doesn't exist
	EM_ASM({
		if (typeof FS !== 'undefined') {
			try {
				FS.mkdir('/game');
				FS.mkdir('/game/data');
			} catch(e) {
				// Directory may already exist
			}
		}
	});
#endif
	initialized = true;
	return true;
#else
	initialized = false;
	return false;
#endif
}

bool Filesystem::exists(const char* path) const
{
	if (!initialized) return false;
	
#ifdef __EMSCRIPTEN__
	// Use Emscripten FS API
	FILE* f = fopen(path, "r");
	if (f) {
		fclose(f);
		return true;
	}
	return false;
#else
	return false;
#endif
}

bool Filesystem::isFile(const char* path) const
{
	if (!initialized) return false;
	
#ifdef __EMSCRIPTEN__
	struct stat st;
	if (stat(path, &st) == 0)
	{
		return S_ISREG(st.st_mode);
	}
#endif
	return false;
}

bool Filesystem::isDirectory(const char* path) const
{
	if (!initialized) return false;
	
#ifdef __EMSCRIPTEN__
	struct stat st;
	if (stat(path, &st) == 0)
	{
		return S_ISDIR(st.st_mode);
	}
#endif
	return false;
}

int Filesystem::readFile(const char* path, void* buffer, int size) const
{
	if (!initialized) return -1;
	
#ifdef __EMSCRIPTEN__
	FILE* f = fopen(path, "rb");
	if (!f) return -1;
	
	int bytesRead = fread(buffer, 1, size, f);
	fclose(f);
	return bytesRead;
#else
	return -1;
#endif
}

int Filesystem::writeFile(const char* path, const void* data, int size)
{
	if (!initialized) return -1;
	
#ifdef __EMSCRIPTEN__
	FILE* f = fopen(path, "wb");
	if (!f) return -1;
	
	int bytesWritten = fwrite(data, 1, size, f);
	fclose(f);
	return bytesWritten;
#else
	return -1;
#endif
}

bool Filesystem::mountZip(const char* zipPath, const char* mountPoint)
{
	if (!initialized) return false;
	
#ifdef __EMSCRIPTEN__
	// Mount ZIP file using Emscripten FS API
	// This requires ZIP extraction and file mounting
	// TASK: Use a ZIP library (like minizip) to extract and mount
	// For now, return false as this requires additional library integration
	// Future implementation would:
	// 1. Read ZIP file
	// 2. Extract contents using minizip or similar
	// 3. Mount extracted directory to mountPoint
	(void)zipPath;
	(void)mountPoint;
	return false;
#else
	return false;
#endif
}
