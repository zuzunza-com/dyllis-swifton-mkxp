/*
** filesystem.h
**
** Filesystem for Palcon-RGSS (WASM)
** Based on mkxp filesystem.h but adapted for Emscripten FS API
**
** Copyright (C) 2025 Palcon Team
*/

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

class Filesystem
{
public:
	Filesystem();
	~Filesystem();

	bool init(const char* gamePath);

	bool exists(const char* path) const;
	bool isFile(const char* path) const;
	bool isDirectory(const char* path) const;

	int readFile(const char* path, void* buffer, int size) const;
	int writeFile(const char* path, const void* data, int size);

	// Mount ZIP file (game data)
	bool mountZip(const char* zipPath, const char* mountPoint = "/game");

private:
	bool initialized;
};

#endif // FILESYSTEM_H
