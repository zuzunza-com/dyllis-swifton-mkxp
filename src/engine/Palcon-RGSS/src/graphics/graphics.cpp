/*
** graphics.cpp
**
** Graphics system implementation for Palcon-RGSS
**
** Copyright (C) 2025 Palcon Team
*/

#include "graphics.h"

Graphics::Graphics()
	: width(640)
	, height(480)
	, frameRate(60)
	, frameCount(0)
	, brightness(255)
{
}

Graphics::~Graphics()
{
}

void Graphics::update()
{
	frameCount++;
	// Render frame
	// TASK: Actual rendering implementation using WebGL/Canvas
	// Rendering is handled via JavaScript bridge
	// Call JavaScript function: Module._rgss_graphics_render()
}

void Graphics::freeze()
{
	// Freeze graphics (capture current frame)
	// TASK: Capture current frame to a texture/surface
	// Call JavaScript function: Module._rgss_graphics_freeze()
}

void Graphics::transition(int duration, const char* filename, int vague)
{
	// Perform transition effect
	// TASK: Implement transition effect (fade, dissolve, etc.)
	// duration: transition duration in frames
	// filename: transition graphic filename (optional)
	// vague: transition vagueness (0-255)
	// Call JavaScript function: Module._rgss_graphics_transition(duration, filename, vague)
	(void)duration;
	(void)filename;
	(void)vague;
}

void Graphics::frameReset()
{
	frameCount = 0;
}

void Graphics::setFrameRate(int rate)
{
	frameRate = rate;
}

void Graphics::setBrightness(int bright)
{
	brightness = bright;
	if (brightness < 0) brightness = 0;
	if (brightness > 255) brightness = 255;
}

void Graphics::resizeScreen(int w, int h)
{
	width = w;
	height = h;
	// Resize render target
	// TASK: Resize WebGL canvas or render target
	// This should notify the rendering system to update canvas size
	// Call JavaScript function: Module._rgss_graphics_resize(w, h)
}
