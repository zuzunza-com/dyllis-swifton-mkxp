/*
** binding-util.cpp
**
** Palcon-RGSS Binding Utilities Implementation
** Based on mkxp binding-util.cpp
** RGSS3 compatible
**
** Copyright (C) 2025 Palcon Team
*/

#include "binding-util.h"
#include "../exception.h"
#include <ruby.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

static RbData* g_rbData = nullptr;
static int rgssVer = 3; // RGSS3

struct
{
	RbException id;
	const char *name;
} static customExc[] =
{
	{ MKXP,   "MKXPError"   },
	{ PHYSFS, "PHYSFSError" },
	{ SDL,    "SDLError"    }
};

RbData::RbData()
{
	memset(exc, 0, sizeof(exc));
	buttoncodeHash = Qnil;
	
	// Initialize custom exception classes
	for (size_t i = 0; i < sizeof(customExc)/sizeof(customExc[0]); ++i)
		exc[customExc[i].id] = rb_define_class(customExc[i].name, rb_eException);
	
	// Initialize standard exception classes
	exc[RGSS]  = rb_define_class("RGSSError", rb_eStandardError);
	exc[Reset] = rb_define_class(rgssVer >= 3 ? "RGSSReset" : "Reset", rb_eException);
	exc[ErrnoENOENT] = rb_const_get(rb_const_get(rb_cObject, rb_intern("Errno")), rb_intern("ENOENT"));
	exc[IOError] = rb_eIOError;
	exc[TypeError] = rb_eTypeError;
	exc[ArgumentError] = rb_eArgError;
	
	// RGSS3 Input module buttoncode hash
	if (rgssVer >= 3)
	{
		buttoncodeHash = rb_hash_new();
		// Populate buttoncode hash for RGSS3
		// Key codes: DOWN=2, LEFT=4, RIGHT=6, UP=8, A=11, B=12, C=13, X=14, Y=15, Z=16, L=17, R=18
		rb_hash_aset(buttoncodeHash, rb_str_new_cstr("DOWN"), rb_fix_new(2));
		rb_hash_aset(buttoncodeHash, rb_str_new_cstr("LEFT"), rb_fix_new(4));
		rb_hash_aset(buttoncodeHash, rb_str_new_cstr("RIGHT"), rb_fix_new(6));
		rb_hash_aset(buttoncodeHash, rb_str_new_cstr("UP"), rb_fix_new(8));
		rb_hash_aset(buttoncodeHash, rb_str_new_cstr("A"), rb_fix_new(11));
		rb_hash_aset(buttoncodeHash, rb_str_new_cstr("B"), rb_fix_new(12));
		rb_hash_aset(buttoncodeHash, rb_str_new_cstr("C"), rb_fix_new(13));
		rb_hash_aset(buttoncodeHash, rb_str_new_cstr("X"), rb_fix_new(14));
		rb_hash_aset(buttoncodeHash, rb_str_new_cstr("Y"), rb_fix_new(15));
		rb_hash_aset(buttoncodeHash, rb_str_new_cstr("Z"), rb_fix_new(16));
		rb_hash_aset(buttoncodeHash, rb_str_new_cstr("L"), rb_fix_new(17));
		rb_hash_aset(buttoncodeHash, rb_str_new_cstr("R"), rb_fix_new(18));
	}
}

RbData::~RbData()
{
}

RbData* getRbData()
{
	return g_rbData;
}

void setRbData(RbData* data)
{
	g_rbData = data;
}

/* Indexed with Exception::Type */
static const RbException excToRbExc[] =
{
    RGSS,        /* RGSSError   */
    ErrnoENOENT, /* NoFileError */
    IOError,
    TypeError,
    ArgumentError,
    PHYSFS,      /* PHYSFSError */
    SDL,         /* SDLError    */
    MKXP         /* MKXPError   */
};

void raiseRbExc(const Exception &exc)
{
	RbData* rbData = getRbData();
	if (!rbData)
	{
		rb_raise(rb_eRuntimeError, "%s", exc.what());
		return;
	}
	
	VALUE excClass = rbData->exc[excToRbExc[0]]; // Default to RGSSError
	// TASK: Map exception types properly based on Exception::Type
	// Map exception type to appropriate Ruby exception
	// Default to RGSSError, but can be extended based on Exception::Type enum
	
	rb_raise(excClass, "%s", exc.what());
}

void raiseDisposedAccess(VALUE self)
{
	const char *klassName = RTYPEDDATA_TYPE(self)->wrap_struct_name;
	char buf[32];
	strncpy(buf, klassName, sizeof(buf));
	buf[0] = tolower(buf[0]);
	buf[sizeof(buf)-1] = '\0';
	
	rb_raise(getRbData()->exc[RGSS], "disposed %s", buf);
}

int rb_get_args(int argc, VALUE *argv, const char *format, ...)
{
	char c;
	VALUE *arg = argv;
	va_list ap;
	bool opt = false;
	int argI = 0;

	va_start(ap, format);

	while ((c = *format++))
	{
		switch (c)
		{
		case '|' :
			break;
		default:
			if (argc <= argI && !opt)
				rb_raise(rb_eArgError, "wrong number of arguments");
			break;
		}

		if (argI >= argc)
			break;

		switch (c)
		{
		case 'o' :
		{
			if (argI >= argc)
				break;
			VALUE *obj = va_arg(ap, VALUE*);
			*obj = *arg++;
			++argI;
			break;
		}

		case 'S' :
		{
			if (argI >= argc)
				break;
			VALUE *str = va_arg(ap, VALUE*);
			VALUE tmp = *arg;
			if (!RB_TYPE_P(tmp, RUBY_T_STRING))
				rb_raise(rb_eTypeError, "Argument %d: Expected string", argI);
			*str = tmp;
			++argI;
			break;
		}

		case 's' :
		{
			if (argI >= argc)
				break;
			const char **s = va_arg(ap, const char**);
			int *len = va_arg(ap, int*);
			VALUE tmp = *arg;
			if (!RB_TYPE_P(tmp, RUBY_T_STRING))
				rb_raise(rb_eTypeError, "Argument %d: Expected string", argI);
			*s = RSTRING_PTR(tmp);
			*len = RSTRING_LEN(tmp);
			++argI;
			break;
		}

		case 'z' :
		{
			if (argI >= argc)
				break;
			const char **s = va_arg(ap, const char**);
			VALUE tmp = *arg++;
			if (!RB_TYPE_P(tmp, RUBY_T_STRING))
				rb_raise(rb_eTypeError, "Argument %d: Expected string", argI);
			*s = RSTRING_PTR(tmp);
			++argI;
			break;
		}

		case 'f' :
		{
			if (argI >= argc)
				break;
			double *f = va_arg(ap, double*);
			VALUE fVal = *arg++;
			rb_float_arg(fVal, f, argI);
			++argI;
			break;
		}

		case 'i' :
		{
			if (argI >= argc)
				break;
			int *i = va_arg(ap, int*);
			VALUE iVal = *arg++;
			rb_int_arg(iVal, i, argI);
			++argI;
			break;
		}

		case 'b' :
		{
			if (argI >= argc)
				break;
			bool *b = va_arg(ap, bool*);
			VALUE bVal = *arg++;
			rb_bool_arg(bVal, b, argI);
			++argI;
			break;
		}

		case 'n' :
		{
			if (argI >= argc)
				break;
			ID *sym = va_arg(ap, ID*);
			VALUE symVal = *arg++;
			if (!SYMBOL_P(symVal))
				rb_raise(rb_eTypeError, "Argument %d: Expected symbol", argI);
			*sym = SYM2ID(symVal);
			++argI;
			break;
		}

		case '|' :
			opt = true;
			break;

		default:
			rb_raise(rb_eFatal, "invalid argument specifier %c", c);
		}
	}

#ifndef NDEBUG
	// Verify correct termination
	format--;
	while ((c = *format++))
	{
		switch (c)
		{
		case 'o' :
		case 'S' :
			va_arg(ap, VALUE*);
			break;
		case 's' :
			va_arg(ap, const char**);
			va_arg(ap, int*);
			break;
		case 'z' :
			va_arg(ap, const char**);
			break;
		case 'f' :
			va_arg(ap, double*);
			break;
		case 'i' :
			va_arg(ap, int*);
			break;
		case 'b' :
			va_arg(ap, bool*);
			break;
		}
	}
	
	if (!c && argc > argI)
		rb_raise(rb_eArgError, "wrong number of arguments");

	void *argEnd = va_arg(ap, void*);
	(void) argEnd;
	assert(argEnd == RB_ARG_END_VAL);
#endif

	va_end(ap);
	return argI;
}
