/*
** font-binding.cpp
**
** Palcon-RGSS Font API Binding
** Based on mkxp font-binding.cpp
** RGSS3 compatible
**
** Copyright (C) 2025 Palcon Team
*/

#include "../binding-util.h"
#include "../../font/font.h"
#include "../../etc/etc.h"
#include "../exception.h"
#include <ruby.h>
#include <string.h>
#include <vector>

static int rgssVer = 3; // RGSS3

DECL_TYPE(Font);
// ColorType is defined in etc-binding.cpp
extern rb_data_type_t ColorType;

static void collectStrings(VALUE obj, std::vector<std::string> &out)
{
	if (RB_TYPE_P(obj, RUBY_T_STRING))
	{
		out.push_back(RSTRING_PTR(obj));
	}
	else if (RB_TYPE_P(obj, RUBY_T_ARRAY))
	{
		for (long i = 0; i < RARRAY_LEN(obj); ++i)
		{
			VALUE str = rb_ary_entry(obj, i);
			if (!RB_TYPE_P(str, RUBY_T_STRING))
				continue;
			out.push_back(RSTRING_PTR(str));
		}
	}
}

RB_METHOD(fontDoesExist);
RB_METHOD(fontInitialize);
RB_METHOD(fontInitializeCopy);
RB_METHOD(FontGetName);
RB_METHOD(FontSetName);
RB_METHOD(fontDefaultName);
RB_METHOD(fontDefaultSize);
RB_METHOD(fontDefaultBold);
RB_METHOD(fontDefaultItalic);
RB_METHOD(fontDefaultColor);
RB_METHOD(fontDefaultOutline);
RB_METHOD(fontDefaultOutlineColor);
RB_METHOD(fontDefaultShadow);
RB_METHOD(fontDefaultShadowColor);

void fontBindingInit()
{
	VALUE klass = rb_define_class("Font", rb_cObject);
	rb_define_alloc_func(klass, classAllocate<&FontType>);

	_rb_define_module_function(klass, "exist?", fontDoesExist);
	_rb_define_method(klass, "initialize", fontInitialize);
	_rb_define_method(klass, "initialize_copy", fontInitializeCopy);
	_rb_define_method(klass, "name", FontGetName);
	_rb_define_method(klass, "name=", FontSetName);

	_rb_define_module_function(klass, "default_name", fontDefaultName);
	_rb_define_module_function(klass, "default_size", fontDefaultSize);
	_rb_define_module_function(klass, "default_bold", fontDefaultBold);
	_rb_define_module_function(klass, "default_italic", fontDefaultItalic);
	_rb_define_module_function(klass, "default_color", fontDefaultColor);
	_rb_define_module_function(klass, "default_outline", fontDefaultOutline);
	_rb_define_module_function(klass, "default_outline_color", fontDefaultOutlineColor);
	_rb_define_module_function(klass, "default_shadow", fontDefaultShadow);
	_rb_define_module_function(klass, "default_shadow_color", fontDefaultShadowColor);
}

DEF_TYPE(Font);

RB_METHOD(fontDoesExist)
{
	RB_UNUSED_PARAM;
	const char *name = 0;
	VALUE nameObj;
	rb_get_args(argc, argv, "o", &nameObj RB_ARG_END);
	if (RB_TYPE_P(nameObj, RUBY_T_STRING))
		name = rb_string_value_cstr(&nameObj);
	return rb_bool_new(Font::doesExist(name));
}

RB_METHOD(fontInitialize)
{
	VALUE namesObj = Qnil;
	int size = 0;
	rb_get_args(argc, argv, "|oi", &namesObj, &size RB_ARG_END);

	Font *f;
	if (NIL_P(namesObj))
	{
		namesObj = rb_iv_get(rb_obj_class(self), "default_name");
		f = new Font(0, size);
	}
	else
	{
		std::vector<std::string> names;
		collectStrings(namesObj, names);
		f = new Font(&names, size);
	}

	rb_iv_set(self, "name", namesObj);
	setPrivateData(self, f);
	f->initDynAttribs();

	// Wrap color properties
	wrapProperty(self, &f->getColor(), "color", ColorType);
	if (rgssVer >= 3)
		wrapProperty(self, &f->getOutColor(), "out_color", ColorType);

	return self;
}

RB_METHOD(fontInitializeCopy)
{
	VALUE origObj;
	rb_get_args(argc, argv, "o", &origObj RB_ARG_END);
	if (!OBJ_INIT_COPY(self, origObj))
		return self;

	Font *orig = getPrivateData<Font>(origObj);
	Font *f = new Font(*orig);
	setPrivateData(self, f);

	f->initDynAttribs();
	wrapProperty(self, &f->getColor(), "color", ColorType);
	if (rgssVer >= 3)
		wrapProperty(self, &f->getOutColor(), "out_color", ColorType);

	return self;
}

RB_METHOD(FontGetName)
{
	RB_UNUSED_PARAM;
	return rb_iv_get(self, "name");
}

RB_METHOD(FontSetName)
{
	Font *f = getPrivateData<Font>(self);
	rb_check_argc(argc, 1);
	std::vector<std::string> namesObj;
	collectStrings(argv[0], namesObj);
	GUARD_EXC( f->names = namesObj; )
	rb_iv_set(self, "name", argv[0]);
	return argv[0];
}

RB_METHOD(fontDefaultName)
{
	if (argc == 0) {
		return rb_str_new_cstr(Font::defaultName.c_str());
	} else {
		const char* name;
		rb_get_args(argc, argv, "z", &name RB_ARG_END);
		Font::defaultName = name;
		return Qnil;
	}
}

RB_METHOD(fontDefaultSize)
{
	if (argc == 0) {
		return rb_fix_new(Font::defaultSize);
	} else {
		int size;
		rb_get_args(argc, argv, "i", &size RB_ARG_END);
		Font::defaultSize = size;
		return Qnil;
	}
}

RB_METHOD(fontDefaultBold)
{
	if (argc == 0) {
		return rb_bool_new(Font::defaultBold);
	} else {
		bool bold;
		rb_bool_arg(*argv, &bold);
		Font::defaultBold = bold;
		return Qnil;
	}
}

RB_METHOD(fontDefaultItalic)
{
	if (argc == 0) {
		return rb_bool_new(Font::defaultItalic);
	} else {
		bool italic;
		rb_bool_arg(*argv, &italic);
		Font::defaultItalic = italic;
		return Qnil;
	}
}

RB_METHOD(fontDefaultColor)
{
	RB_UNUSED_PARAM;
	// Return/get default color
	// Create a copy of default color
	Color* c = new Color(Font::defaultColor);
	VALUE colorObj = wrapObject(c, ColorType);
	return colorObj;
}

RB_METHOD(fontDefaultOutline)
{
	if (argc == 0) {
		return rb_bool_new(Font::defaultOutline);
	} else {
		bool outline;
		rb_bool_arg(*argv, &outline);
		Font::defaultOutline = outline;
		return Qnil;
	}
}

RB_METHOD(fontDefaultOutlineColor)
{
	RB_UNUSED_PARAM;
	// Return/get default outline color
	Color* c = new Color(Font::defaultOutlineColor);
	VALUE colorObj = wrapObject(c, ColorType);
	return colorObj;
}

RB_METHOD(fontDefaultShadow)
{
	if (argc == 0) {
		return rb_bool_new(Font::defaultShadow);
	} else {
		bool shadow;
		rb_bool_arg(*argv, &shadow);
		Font::defaultShadow = shadow;
		return Qnil;
	}
}

RB_METHOD(fontDefaultShadowColor)
{
	RB_UNUSED_PARAM;
	// Return/get default shadow color
	Color* c = new Color(Font::defaultShadowColor);
	VALUE colorObj = wrapObject(c, ColorType);
	return colorObj;
}
