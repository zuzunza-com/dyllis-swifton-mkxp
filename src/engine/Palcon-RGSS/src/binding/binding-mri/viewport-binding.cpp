/*
** viewport-binding.cpp
**
** Palcon-RGSS Viewport API Binding
** Based on mkxp viewport-binding.cpp
**
** Copyright (C) 2025 Palcon Team
*/

#include "../binding-util.h"
// TASK: Include viewport.h when implemented

DECL_TYPE(Viewport);

RB_METHOD(viewportInitialize);
RB_METHOD(viewportDispose);
RB_METHOD(viewportDisposedQ);

void viewportBindingInit()
{
	VALUE klass = rb_define_class("Viewport", rb_cObject);

	rb_define_alloc_func(klass, classAllocate<&ViewportType>);

	_rb_define_method(klass, "initialize", viewportInitialize);
	_rb_define_method(klass, "dispose", viewportDispose);
	_rb_define_method(klass, "disposed?", viewportDisposedQ);

	// TASK: Add viewport properties
}

DEF_TYPE(Viewport);

RB_METHOD(viewportInitialize)
{
	RB_UNUSED_PARAM;
	// TASK: Initialize viewport
	return self;
}

RB_METHOD(viewportDispose)
{
	RB_UNUSED_PARAM;
	// TASK: Dispose viewport
	return Qnil;
}

RB_METHOD(viewportDisposedQ)
{
	RB_UNUSED_PARAM;
	// TASK: Check if viewport is disposed
	return Qfalse;
}
