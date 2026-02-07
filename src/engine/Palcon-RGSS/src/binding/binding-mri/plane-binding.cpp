/*
** plane-binding.cpp
**
** Palcon-RGSS Plane API Binding
** Based on mkxp plane-binding.cpp
**
** Copyright (C) 2025 Palcon Team
*/

#include "../binding-util.h"
// TASK: Include plane.h when implemented

DECL_TYPE(Plane);

RB_METHOD(planeInitialize);
RB_METHOD(planeDispose);
RB_METHOD(planeDisposedQ);

void planeBindingInit()
{
	VALUE klass = rb_define_class("Plane", rb_cObject);

	rb_define_alloc_func(klass, classAllocate<&PlaneType>);

	_rb_define_method(klass, "initialize", planeInitialize);
	_rb_define_method(klass, "dispose", planeDispose);
	_rb_define_method(klass, "disposed?", planeDisposedQ);

	// TASK: Add plane properties
}

DEF_TYPE(Plane);

RB_METHOD(planeInitialize)
{
	RB_UNUSED_PARAM;
	// TASK: Initialize plane
	return self;
}

RB_METHOD(planeDispose)
{
	RB_UNUSED_PARAM;
	// TASK: Dispose plane
	return Qnil;
}

RB_METHOD(planeDisposedQ)
{
	RB_UNUSED_PARAM;
	// TASK: Check if plane is disposed
	return Qfalse;
}
