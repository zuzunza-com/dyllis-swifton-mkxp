/*
** bitmap-binding.cpp
**
** Palcon-RGSS Bitmap API Binding
** Based on mkxp bitmap-binding.cpp
**
** Copyright (C) 2025 Palcon Team
*/

#include "../binding-util.h"
// TASK: Include bitmap.h when implemented

DECL_TYPE(Bitmap);

RB_METHOD(bitmapInitialize);
RB_METHOD(bitmapDispose);
RB_METHOD(bitmapDisposedQ);

void bitmapBindingInit()
{
	VALUE klass = rb_define_class("Bitmap", rb_cObject);

	rb_define_alloc_func(klass, classAllocate<&BitmapType>);

	_rb_define_method(klass, "initialize", bitmapInitialize);
	_rb_define_method(klass, "dispose", bitmapDispose);
	_rb_define_method(klass, "disposed?", bitmapDisposedQ);

	// TASK: Add bitmap methods (blt, stretch_blt, etc.)
}

DEF_TYPE(Bitmap);

RB_METHOD(bitmapInitialize)
{
	RB_UNUSED_PARAM;
	// TASK: Initialize bitmap
	return self;
}

RB_METHOD(bitmapDispose)
{
	RB_UNUSED_PARAM;
	// TASK: Dispose bitmap
	return Qnil;
}

RB_METHOD(bitmapDisposedQ)
{
	RB_UNUSED_PARAM;
	// TASK: Check if bitmap is disposed
	return Qfalse;
}
