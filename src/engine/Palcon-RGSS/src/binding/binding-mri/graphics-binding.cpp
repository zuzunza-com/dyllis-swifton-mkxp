/*
** graphics-binding.cpp
**
** Palcon-RGSS Graphics API Binding
** Based on mkxp graphics-binding.cpp
** RGSS3 compatible
**
** Copyright (C) 2025 Palcon Team
*/

#include "../binding-util.h"
#include "../../graphics/graphics.h"
#include "../../sharedstate.h"
#include "../exception.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

RB_METHOD(graphicsUpdate);
RB_METHOD(graphicsWait);
RB_METHOD(graphicsFadeout);
RB_METHOD(graphicsFadein);
RB_METHOD(graphicsFreeze);
RB_METHOD(graphicsTransition);
RB_METHOD(graphicsFrameRate);
RB_METHOD(graphicsFrameCount);
RB_METHOD(graphicsBrightness);
RB_METHOD(graphicsWidth);
RB_METHOD(graphicsHeight);
RB_METHOD(graphicsResizeScreen);
RB_METHOD(graphicsPlayMovie);
RB_METHOD(graphicsSnapToBitmap);
RB_METHOD(graphicsReset);
RB_METHOD(graphicsFrameReset);

void graphicsBindingInit()
{
	VALUE module = rb_define_module("Graphics");

	_rb_define_module_function(module, "update", graphicsUpdate);
	_rb_define_module_function(module, "wait", graphicsWait);
	_rb_define_module_function(module, "fadeout", graphicsFadeout);
	_rb_define_module_function(module, "fadein", graphicsFadein);
	_rb_define_module_function(module, "freeze", graphicsFreeze);
	_rb_define_module_function(module, "transition", graphicsTransition);
	_rb_define_module_function(module, "frame_rate", graphicsFrameRate);
	_rb_define_module_function(module, "frame_count", graphicsFrameCount);
	_rb_define_module_function(module, "brightness", graphicsBrightness);
	_rb_define_module_function(module, "width", graphicsWidth);
	_rb_define_module_function(module, "height", graphicsHeight);
	_rb_define_module_function(module, "resize_screen", graphicsResizeScreen);
	_rb_define_module_function(module, "play_movie", graphicsPlayMovie);
	_rb_define_module_function(module, "snap_to_bitmap", graphicsSnapToBitmap);
	_rb_define_module_function(module, "reset", graphicsReset);
	_rb_define_module_function(module, "frame_reset", graphicsFrameReset);
}

RB_METHOD(graphicsUpdate)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->graphics().update(); )
	}
	return Qnil;
}

RB_METHOD(graphicsWait)
{
	RB_UNUSED_PARAM;
	int duration;
	rb_get_args(argc, argv, "i", &duration RB_ARG_END);
	// Wait for specified duration (frames)
	// TASK: Implement frame-based waiting
	// Wait for specified number of frames
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		int startFrame = ss->graphics().getFrameCount();
		while (ss->graphics().getFrameCount() - startFrame < duration)
		{
			ss->graphics().update();
#ifdef __EMSCRIPTEN__
			emscripten_sleep(16); // ~60 FPS
#endif
		}
	}
	(void)duration;
	return Qnil;
}

RB_METHOD(graphicsFadeout)
{
	RB_UNUSED_PARAM;
	int duration;
	rb_get_args(argc, argv, "i", &duration RB_ARG_END);
	// TASK: Fade out screen
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		// Fade out brightness from current to 0 over duration frames
		int startBrightness = ss->graphics().getBrightness();
		int frames = duration;
		for (int i = 0; i <= frames; i++)
		{
			int brightness = startBrightness * (frames - i) / frames;
			ss->graphics().setBrightness(brightness);
			ss->graphics().update();
#ifdef __EMSCRIPTEN__
			emscripten_sleep(16);
#endif
		}
	}
	(void)duration;
	return Qnil;
}

RB_METHOD(graphicsFadein)
{
	RB_UNUSED_PARAM;
	int duration;
	rb_get_args(argc, argv, "i", &duration RB_ARG_END);
	// TASK: Fade in screen
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		// Fade in brightness from 0 to 255 over duration frames
		int frames = duration;
		for (int i = 0; i <= frames; i++)
		{
			int brightness = 255 * i / frames;
			ss->graphics().setBrightness(brightness);
			ss->graphics().update();
#ifdef __EMSCRIPTEN__
			emscripten_sleep(16);
#endif
		}
	}
	(void)duration;
	return Qnil;
}

RB_METHOD(graphicsFreeze)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->graphics().freeze(); )
	}
	return Qnil;
}

RB_METHOD(graphicsTransition)
{
	RB_UNUSED_PARAM;
	int duration = 8;
	const char* filename = "";
	int vague = 40;
	rb_get_args(argc, argv, "|izi", &duration, &filename, &vague RB_ARG_END);

	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->graphics().transition(duration, filename, vague); )
	}
	return Qnil;
}

RB_METHOD(graphicsFrameRate)
{
	if (argc == 0) {
		// Getter
		SharedState* ss = SharedState::instance();
		if (ss)
		{
			int rate = 60;
			GUARD_EXC( rate = ss->graphics().getFrameRate(); )
			return rb_fix_new(rate);
		}
		return rb_fix_new(60);
	} else {
		// Setter
		int rate;
		rb_get_args(argc, argv, "i", &rate RB_ARG_END);
		SharedState* ss = SharedState::instance();
		if (ss)
		{
			GUARD_EXC( ss->graphics().setFrameRate(rate); )
		}
		return Qnil;
	}
}

RB_METHOD(graphicsFrameCount)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		int count = 0;
		GUARD_EXC( count = ss->graphics().getFrameCount(); )
		return rb_fix_new(count);
	}
	return rb_fix_new(0);
}

RB_METHOD(graphicsBrightness)
{
	if (argc == 0) {
		// Getter
		SharedState* ss = SharedState::instance();
		if (ss)
		{
			int bright = 255;
			GUARD_EXC( bright = ss->graphics().getBrightness(); )
			return rb_fix_new(bright);
		}
		return rb_fix_new(255);
	} else {
		// Setter
		int brightness;
		rb_get_args(argc, argv, "i", &brightness RB_ARG_END);
		SharedState* ss = SharedState::instance();
		if (ss)
		{
			GUARD_EXC( ss->graphics().setBrightness(brightness); )
		}
		return Qnil;
	}
}

RB_METHOD(graphicsWidth)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		int width = 640;
		GUARD_EXC( width = ss->graphics().getWidth(); )
		return rb_fix_new(width);
	}
	return rb_fix_new(640);
}

RB_METHOD(graphicsHeight)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		int height = 480;
		GUARD_EXC( height = ss->graphics().getHeight(); )
		return rb_fix_new(height);
	}
	return rb_fix_new(480);
}

RB_METHOD(graphicsResizeScreen)
{
	RB_UNUSED_PARAM;
	int width, height;
	rb_get_args(argc, argv, "ii", &width, &height RB_ARG_END);
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->graphics().resizeScreen(width, height); )
	}
	return Qnil;
}

RB_METHOD(graphicsPlayMovie)
{
	RB_UNUSED_PARAM;
	VALUE filename;
	rb_get_args(argc, argv, "o", &filename RB_ARG_END);
	// TASK: Play movie
	// Movie playback requires video codec support
	// For now, this is a placeholder
	// Future: Integrate video playback library
	(void)filename;
	return Qnil;
}

RB_METHOD(graphicsSnapToBitmap)
{
	RB_UNUSED_PARAM;
	// TASK: Capture screen to bitmap
	// Screen capture requires bitmap implementation
	// For now, return nil - bitmap binding needs to be implemented first
	return Qnil;
}

RB_METHOD(graphicsReset)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->graphics().frameReset(); )
	}
	return Qnil;
}

RB_METHOD(graphicsFrameReset)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->graphics().frameReset(); )
	}
	return Qnil;
}
