/*
** sprite-binding.cpp
**
** Palcon-RGSS Sprite API Binding
** Based on mkxp sprite-binding.cpp
**
** Copyright (C) 2025 Palcon Team
*/

#include "../binding-util.h"
// TASK: Include sprite.h when implemented

DECL_TYPE(Sprite);

RB_METHOD(spriteInitialize);
RB_METHOD(spriteDispose);
RB_METHOD(spriteDisposedQ);

void spriteBindingInit()
{
	VALUE klass = rb_define_class("Sprite", rb_cObject);

	rb_define_alloc_func(klass, classAllocate<&SpriteType>);

	_rb_define_method(klass, "initialize", spriteInitialize);
	_rb_define_method(klass, "dispose", spriteDispose);
	_rb_define_method(klass, "disposed?", spriteDisposedQ);

	// TASK: Add more sprite properties and methods
	// DEF_PROP_I(Sprite, X);
	// DEF_PROP_I(Sprite, Y);
	// DEF_PROP_I(Sprite, Z);
	// etc.
}

DEF_TYPE(Sprite);

RB_METHOD(spriteInitialize)
{
	RB_UNUSED_PARAM;
	// TASK: Initialize sprite
	return self;
}

RB_METHOD(spriteDispose)
{
	RB_UNUSED_PARAM;
	// TASK: Dispose sprite
	return Qnil;
}

RB_METHOD(spriteDisposedQ)
{
	RB_UNUSED_PARAM;
	// TASK: Check if sprite is disposed
	return Qfalse;
}
