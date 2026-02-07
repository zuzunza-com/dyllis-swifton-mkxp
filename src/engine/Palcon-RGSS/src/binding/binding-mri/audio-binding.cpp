/*
** audio-binding.cpp
**
** Palcon-RGSS Audio API Binding
** Based on mkxp audio-binding.cpp
** RGSS3 compatible with pos parameter support
**
** Copyright (C) 2025 Palcon Team
*/

#include "../binding-util.h"
#include "../../audio/audio.h"
#include "../../sharedstate.h"
#include "../exception.h"

static int rgssVer = 3; // RGSS3

RB_METHOD(audioBgmPlay);
RB_METHOD(audioBgmStop);
RB_METHOD(audioBgmFade);
RB_METHOD(audioBgmPos);
RB_METHOD(audioBgsPlay);
RB_METHOD(audioBgsStop);
RB_METHOD(audioBgsFade);
RB_METHOD(audioBgsPos);
RB_METHOD(audioMePlay);
RB_METHOD(audioMeStop);
RB_METHOD(audioMeFade);
RB_METHOD(audioSePlay);
RB_METHOD(audioSeStop);
RB_METHOD(audioSetupMidi);
RB_METHOD(audioReset);

void audioBindingInit()
{
	VALUE module = rb_define_module("Audio");

	_rb_define_module_function(module, "bgm_play", audioBgmPlay);
	_rb_define_module_function(module, "bgm_stop", audioBgmStop);
	_rb_define_module_function(module, "bgm_fade", audioBgmFade);
	_rb_define_module_function(module, "bgs_play", audioBgsPlay);
	_rb_define_module_function(module, "bgs_stop", audioBgsStop);
	_rb_define_module_function(module, "bgs_fade", audioBgsFade);
	_rb_define_module_function(module, "me_play", audioMePlay);
	_rb_define_module_function(module, "me_stop", audioMeStop);
	_rb_define_module_function(module, "me_fade", audioMeFade);
	_rb_define_module_function(module, "se_play", audioSePlay);
	_rb_define_module_function(module, "se_stop", audioSeStop);

	// RGSS3 specific
	if (rgssVer >= 3)
	{
		_rb_define_module_function(module, "bgm_pos", audioBgmPos);
		_rb_define_module_function(module, "bgs_pos", audioBgsPos);
		_rb_define_module_function(module, "setup_midi", audioSetupMidi);
	}
}

RB_METHOD(audioBgmPlay)
{
	RB_UNUSED_PARAM;
	const char* filename;
	int volume = 100;
	int pitch = 100;
	double pos = 0.0;

	if (rgssVer >= 3)
		rb_get_args(argc, argv, "z|iif", &filename, &volume, &pitch, &pos RB_ARG_END);
	else
		rb_get_args(argc, argv, "z|ii", &filename, &volume, &pitch RB_ARG_END);

	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->audio().bgmPlay(filename, volume, pitch, pos); )
	}

	return Qnil;
}

RB_METHOD(audioBgmStop)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->audio().bgmPlay(filename, volume, pitch, pos); )
	}
	return Qnil;
}

RB_METHOD(audioBgmStop)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->audio().bgmStop(); )
	}
	return Qnil;
}

RB_METHOD(audioBgmFade)
{
	RB_UNUSED_PARAM;
	int duration;
	rb_get_args(argc, argv, "i", &duration RB_ARG_END);

	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->audio().bgmFade(duration); )
	}
	return Qnil;
}

RB_METHOD(audioBgmPos)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		double pos = 0.0;
		GUARD_EXC( pos = ss->audio().bgmPos(); )
		return rb_float_new(pos);
	}
	return rb_float_new(0.0);
}

RB_METHOD(audioBgsPlay)
{
	RB_UNUSED_PARAM;
	const char* filename;
	int volume = 100;
	int pitch = 100;
	double pos = 0.0;

	if (rgssVer >= 3)
		rb_get_args(argc, argv, "z|iif", &filename, &volume, &pitch, &pos RB_ARG_END);
	else
		rb_get_args(argc, argv, "z|ii", &filename, &volume, &pitch RB_ARG_END);

	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->audio().bgsPlay(filename, volume, pitch, pos); )
	}
	return Qnil;
}

RB_METHOD(audioBgsStop)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->audio().bgsStop(); )
	}
	return Qnil;
}

RB_METHOD(audioBgsFade)
{
	RB_UNUSED_PARAM;
	int duration;
	rb_get_args(argc, argv, "i", &duration RB_ARG_END);

	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->audio().bgsFade(duration); )
	}
	return Qnil;
}

RB_METHOD(audioBgsPos)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		double pos = 0.0;
		GUARD_EXC( pos = ss->audio().bgsPos(); )
		return rb_float_new(pos);
	}
	return rb_float_new(0.0);
}

RB_METHOD(audioMePlay)
{
	RB_UNUSED_PARAM;
	const char* filename;
	int volume = 100;
	int pitch = 100;
	rb_get_args(argc, argv, "z|ii", &filename, &volume, &pitch RB_ARG_END);

	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->audio().mePlay(filename, volume, pitch); )
	}
	return Qnil;
}

RB_METHOD(audioMeStop)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->audio().meStop(); )
	}
	return Qnil;
}

RB_METHOD(audioMeFade)
{
	RB_UNUSED_PARAM;
	int duration;
	rb_get_args(argc, argv, "i", &duration RB_ARG_END);

	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->audio().meFade(duration); )
	}
	return Qnil;
}

RB_METHOD(audioSePlay)
{
	RB_UNUSED_PARAM;
	const char* filename;
	int volume = 100;
	int pitch = 100;
	rb_get_args(argc, argv, "z|ii", &filename, &volume, &pitch RB_ARG_END);

	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->audio().sePlay(filename, volume, pitch); )
	}
	return Qnil;
}

RB_METHOD(audioSeStop)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->audio().seStop(); )
	}
	return Qnil;
}

RB_METHOD(audioSetupMidi)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->audio().setupMidi(); )
	}
	return Qnil;
}

RB_METHOD(audioReset)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->audio().reset(); )
	}
	return Qnil;
}
