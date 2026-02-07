/*
** input-binding.cpp
**
** Palcon-RGSS Input API Binding
** Based on mkxp input-binding.cpp
** RGSS3 compatible
**
** Copyright (C) 2025 Palcon Team
*/

#include "../binding-util.h"
#include "../../input/input.h"
#include "../../sharedstate.h"
#include "../exception.h"

RB_METHOD(inputUpdate);
RB_METHOD(inputPressQ);
RB_METHOD(inputTriggerQ);
RB_METHOD(inputRepeatQ);
RB_METHOD(inputDir4);
RB_METHOD(inputDir8);

void inputBindingInit()
{
	VALUE module = rb_define_module("Input");

	_rb_define_module_function(module, "update", inputUpdate);
	_rb_define_module_function(module, "press?", inputPressQ);
	_rb_define_module_function(module, "trigger?", inputTriggerQ);
	_rb_define_module_function(module, "repeat?", inputRepeatQ);
	_rb_define_module_function(module, "dir4", inputDir4);
	_rb_define_module_function(module, "dir8", inputDir8);
}

RB_METHOD(inputUpdate)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		GUARD_EXC( ss->input().update(); )
	}
	return Qnil;
}

RB_METHOD(inputPressQ)
{
	RB_UNUSED_PARAM;
	int key;
	rb_get_args(argc, argv, "i", &key RB_ARG_END);
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		bool pressed = false;
		GUARD_EXC( pressed = ss->input().isPressed(key); )
		return rb_bool_new(pressed);
	}
	return Qfalse;
}

RB_METHOD(inputTriggerQ)
{
	RB_UNUSED_PARAM;
	int key;
	rb_get_args(argc, argv, "i", &key RB_ARG_END);
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		bool triggered = false;
		GUARD_EXC( triggered = ss->input().isTriggered(key); )
		return rb_bool_new(triggered);
	}
	return Qfalse;
}

RB_METHOD(inputRepeatQ)
{
	RB_UNUSED_PARAM;
	int key;
	rb_get_args(argc, argv, "i", &key RB_ARG_END);
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		bool repeating = false;
		GUARD_EXC( repeating = ss->input().isRepeating(key); )
		return rb_bool_new(repeating);
	}
	return Qfalse;
}

RB_METHOD(inputDir4)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		int dir = 0;
		GUARD_EXC( dir = ss->input().getDir4(); )
		return rb_fix_new(dir);
	}
	return rb_fix_new(0);
}

RB_METHOD(inputDir8)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		int dir = 0;
		GUARD_EXC( dir = ss->input().getDir8(); )
		return rb_fix_new(dir);
	}
	return rb_fix_new(0);
}
