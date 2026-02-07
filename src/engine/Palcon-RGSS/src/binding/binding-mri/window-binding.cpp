/*
** window-binding.cpp
**
** Palcon-RGSS Window API Binding
** Based on mkxp window-binding.cpp
**
** Copyright (C) 2025 Palcon Team
*/

#include "../binding-util.h"
// TASK: Include window.h when implemented

DECL_TYPE(Window);

RB_METHOD(windowInitialize);
RB_METHOD(windowDispose);
RB_METHOD(windowDisposedQ);

void windowBindingInit()
{
	VALUE klass = rb_define_class("Window", rb_cObject);

	rb_define_alloc_func(klass, classAllocate<&WindowType>);

	_rb_define_method(klass, "initialize", windowInitialize);
	_rb_define_method(klass, "dispose", windowDispose);
	_rb_define_method(klass, "disposed?", windowDisposedQ);

	// TASK: Add window properties
}

DEF_TYPE(Window);

RB_METHOD(windowInitialize)
{
	RB_UNUSED_PARAM;
	// TASK: Initialize window
	return self;
}

RB_METHOD(windowDispose)
{
	RB_UNUSED_PARAM;
	// TASK: Dispose window
	return Qnil;
}

RB_METHOD(windowDisposedQ)
{
	RB_UNUSED_PARAM;
	// TASK: Check if window is disposed
	return Qfalse;
}

void windowVXBindingInit()
{
	// VX version of window binding
	windowBindingInit();
	// TASK: Add VX-specific window methods
}
