/*
** tilemap-binding.cpp
**
** Palcon-RGSS Tilemap API Binding
** Based on mkxp tilemap-binding.cpp
**
** Copyright (C) 2025 Palcon Team
*/

#include "../binding-util.h"
// TASK: Include tilemap.h when implemented

DECL_TYPE(Tilemap);

RB_METHOD(tilemapInitialize);
RB_METHOD(tilemapDispose);
RB_METHOD(tilemapDisposedQ);

void tilemapBindingInit()
{
	VALUE klass = rb_define_class("Tilemap", rb_cObject);

	rb_define_alloc_func(klass, classAllocate<&TilemapType>);

	_rb_define_method(klass, "initialize", tilemapInitialize);
	_rb_define_method(klass, "dispose", tilemapDispose);
	_rb_define_method(klass, "disposed?", tilemapDisposedQ);

	// TASK: Add tilemap properties
}

DEF_TYPE(Tilemap);

RB_METHOD(tilemapInitialize)
{
	RB_UNUSED_PARAM;
	// TASK: Initialize tilemap
	return self;
}

RB_METHOD(tilemapDispose)
{
	RB_UNUSED_PARAM;
	// TASK: Dispose tilemap
	return Qnil;
}

RB_METHOD(tilemapDisposedQ)
{
	RB_UNUSED_PARAM;
	// TASK: Check if tilemap is disposed
	return Qfalse;
}

void tilemapVXBindingInit()
{
	// VX version of tilemap binding
	tilemapBindingInit();
	// TASK: Add VX-specific tilemap methods
}
