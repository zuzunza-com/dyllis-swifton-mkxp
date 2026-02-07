/*
** sharedstate.cpp
**
** Shared State implementation for Palcon-RGSS
**
** Copyright (C) 2025 Palcon Team
*/

#include "sharedstate.h"

SharedState* SharedState::g_instance = nullptr;

SharedState::SharedState()
	: gfx(new Graphics())
	, aud(new Audio())
	, inp(new Input())
	, fs(new Filesystem())
{
	g_instance = this;
}

SharedState::~SharedState()
{
	delete gfx;
	delete aud;
	delete inp;
	delete fs;
	g_instance = nullptr;
}

SharedState* SharedState::instance()
{
	return g_instance;
}

extern "C" {
	Audio* getAudio()
	{
		SharedState* ss = SharedState::instance();
		return ss ? &ss->audio() : nullptr;
	}

	Graphics* getGraphics()
	{
		SharedState* ss = SharedState::instance();
		return ss ? &ss->graphics() : nullptr;
	}

	Input* getInput()
	{
		SharedState* ss = SharedState::instance();
		return ss ? &ss->input() : nullptr;
	}

	Filesystem* getFilesystem()
	{
		SharedState* ss = SharedState::instance();
		return ss ? &ss->filesystem() : nullptr;
	}
}
