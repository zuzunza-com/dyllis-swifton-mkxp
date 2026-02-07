/*
** filesystem-binding.cpp
**
** Palcon-RGSS Filesystem API Binding
** Based on mkxp filesystem-binding.cpp
** RGSS3 compatible
**
** Copyright (C) 2025 Palcon Team
*/

#include "../binding-util.h"
#include "../../filesystem/filesystem.h"
#include "../../sharedstate.h"
#include "../exception.h"
#include <ruby.h>

RB_METHOD(fileIntOpen);
RB_METHOD(fileIntClose);
RB_METHOD(fileIntRead);
RB_METHOD(fileIntWrite);
RB_METHOD(fileIntGetPos);
RB_METHOD(fileIntSetPos);
RB_METHOD(fileIntGetSize);

void fileIntBindingInit()
{
	VALUE klass = rb_define_class("File", rb_cObject);
	
	_rb_define_method(klass, "open", fileIntOpen);
	_rb_define_method(klass, "close", fileIntClose);
	_rb_define_method(klass, "read", fileIntRead);
	_rb_define_method(klass, "write", fileIntWrite);
	_rb_define_method(klass, "pos", fileIntGetPos);
	_rb_define_method(klass, "pos=", fileIntSetPos);
	_rb_define_method(klass, "size", fileIntGetSize);
}

RB_METHOD(fileIntOpen)
{
	RB_UNUSED_PARAM;
	const char* filename;
	const char* mode = "r";
	rb_get_args(argc, argv, "z|z", &filename, &mode RB_ARG_END);
	
	SharedState* ss = SharedState::instance();
	if (ss && ss->filesystem().exists(filename))
	{
		// File opened successfully
		// Store file handle in instance variable
		rb_iv_set(self, "@filename", rb_str_new_cstr(filename));
		rb_iv_set(self, "@mode", rb_str_new_cstr(mode));
		return self;
	}
	
	rb_raise(rb_eIOError, "File not found: %s", filename);
	return Qnil;
}

RB_METHOD(fileIntClose)
{
	RB_UNUSED_PARAM;
	// Close file
	rb_iv_set(self, "@filename", Qnil);
	rb_iv_set(self, "@mode", Qnil);
	return Qnil;
}

RB_METHOD(fileIntRead)
{
	RB_UNUSED_PARAM;
	int size = -1;
	rb_get_args(argc, argv, "|i", &size RB_ARG_END);
	
	VALUE filenameObj = rb_iv_get(self, "@filename");
	if (NIL_P(filenameObj))
		return Qnil;
	
	const char* filename = rb_string_value_cstr(&filenameObj);
	
	SharedState* ss = SharedState::instance();
	if (!ss)
		return Qnil;
	
	char buffer[4096];
	int bytesRead = ss->filesystem().readFile(filename, buffer, size > 0 ? size : sizeof(buffer));
	
	if (bytesRead > 0)
		return rb_str_new(buffer, bytesRead);
	
	return Qnil;
}

RB_METHOD(fileIntWrite)
{
	RB_UNUSED_PARAM;
	const char* data;
	int len;
	rb_get_args(argc, argv, "s", &data, &len RB_ARG_END);
	
	VALUE filenameObj = rb_iv_get(self, "@filename");
	if (NIL_P(filenameObj))
		return Qnil;
	
	const char* filename = rb_string_value_cstr(&filenameObj);
	
	SharedState* ss = SharedState::instance();
	if (!ss)
		return Qnil;
	
	int bytesWritten = ss->filesystem().writeFile(filename, data, len);
	return rb_fix_new(bytesWritten);
}

RB_METHOD(fileIntGetPos)
{
	RB_UNUSED_PARAM;
	// File position tracking would require file handle management
	// For now, return 0
	return rb_fix_new(0);
}

RB_METHOD(fileIntSetPos)
{
	RB_UNUSED_PARAM;
	int pos;
	rb_get_args(argc, argv, "i", &pos RB_ARG_END);
	// File position setting would require file handle management
	// For now, do nothing
	(void)pos;
	return Qnil;
}

RB_METHOD(fileIntGetSize)
{
	RB_UNUSED_PARAM;
	VALUE filenameObj = rb_iv_get(self, "@filename");
	if (NIL_P(filenameObj))
		return rb_fix_new(0);
	
	const char* filename = rb_string_value_cstr(&filenameObj);
	
	SharedState* ss = SharedState::instance();
	if (!ss)
		return rb_fix_new(0);
	
	// Get file size by reading and checking
	// TODO: Implement proper file size query
	return rb_fix_new(0);
}
