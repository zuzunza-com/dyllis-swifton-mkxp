/*
** sharedstate.h
**
** Shared State for Palcon-RGSS
** Provides access to engine subsystems
**
** Copyright (C) 2025 Palcon Team
*/

#ifndef SHAREDSTATE_H
#define SHAREDSTATE_H

#include "graphics/graphics.h"
#include "audio/audio.h"
#include "input/input.h"
#include "filesystem/filesystem.h"

class SharedState
{
public:
	SharedState();
	~SharedState();

	Graphics& graphics() { return *gfx; }
	Audio& audio() { return *aud; }
	Input& input() { return *inp; }
	Filesystem& filesystem() { return *fs; }

	static SharedState* instance();

private:
	Graphics* gfx;
	Audio* aud;
	Input* inp;
	Filesystem* fs;

	static SharedState* g_instance;
};

// Convenience functions
inline Graphics& shStateGraphics() { return SharedState::instance()->graphics(); }
inline Audio& shStateAudio() { return SharedState::instance()->audio(); }
inline Input& shStateInput() { return SharedState::instance()->input(); }
inline Filesystem& shStateFilesystem() { return SharedState::instance()->filesystem(); }

// For C bindings
extern "C" {
	Audio* getAudio();
	Graphics* getGraphics();
	Input* getInput();
	Filesystem* getFilesystem();
}

#endif // SHAREDSTATE_H
