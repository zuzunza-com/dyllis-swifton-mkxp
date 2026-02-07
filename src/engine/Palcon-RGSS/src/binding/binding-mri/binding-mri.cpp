/*
** binding-mri.cpp
**
** Palcon-RGSS MRI Binding
** Based on mkxp binding-mri.cpp
**
** Copyright (C) 2025 Palcon Team
*/

#include "../binding.h"
#include "../binding-util.h"
#include "../../sharedstate.h"
#include "../exception.h"

#include <ruby.h>
#include <ruby/encoding.h>

#include <assert.h>
#include <string>
#include <zlib.h>
#include <setjmp.h>
#include <stdarg.h>

static jmp_buf excBuf;
static bool excBufActive = false;
static RbData* g_rbDataInstance = nullptr;

static void mriBindingExecute();
static void mriBindingTerminate();
static void mriBindingReset();

ScriptBinding scriptBindingImpl =
{
	mriBindingExecute,
	mriBindingTerminate,
	mriBindingReset
};

ScriptBinding *scriptBinding = &scriptBindingImpl;

// Binding initialization functions (to be implemented)
void tableBindingInit();
void etcBindingInit();
void fontBindingInit();
void bitmapBindingInit();
void spriteBindingInit();
void viewportBindingInit();
void planeBindingInit();
void windowBindingInit();
void tilemapBindingInit();
void windowVXBindingInit();
void tilemapVXBindingInit();

void inputBindingInit();
void audioBindingInit();
void graphicsBindingInit();

void fileIntBindingInit();
void moduleRpgInit();

RB_METHOD(mriPrint);
RB_METHOD(mriP);
RB_METHOD(mkxpDataDirectory);
RB_METHOD(mkxpPuts);
RB_METHOD(mkxpRawKeyStates);
RB_METHOD(mkxpMouseInWindow);

RB_METHOD(mriRgssMain);
RB_METHOD(mriRgssStop);
RB_METHOD(_kernelCaller);

static int rgssVer = 3; // Default to RGSS3

static void mriBindingInit()
{
	// Initialize RbData
	g_rbDataInstance = new RbData();
	setRbData(g_rbDataInstance);
	
	tableBindingInit();
	etcBindingInit();
	fontBindingInit();
	bitmapBindingInit();
	spriteBindingInit();
	viewportBindingInit();
	planeBindingInit();

	if (rgssVer == 1)
	{
		windowBindingInit();
		tilemapBindingInit();
	}
	else
	{
		windowVXBindingInit();
		tilemapVXBindingInit();
	}

	inputBindingInit();
	audioBindingInit();
	graphicsBindingInit();

	fileIntBindingInit();
	
	// Initialize RPG module (RGSS3)
	// This creates the RPG module structure
	// Actual class definitions come from rgss3.rb
	moduleRpgInit();
	
	// TASK: Load and evaluate rgss3.rb here
	// This should happen after all bindings are initialized
	// Load rgss3.rb from filesystem if available
	SharedState* ss = SharedState::instance();
	if (ss && ss->filesystem().exists("rgss3.rb"))
	{
		// Read and evaluate rgss3.rb content
		char buffer[1024 * 1024]; // 1MB buffer
		int size = ss->filesystem().readFile("rgss3.rb", buffer, sizeof(buffer));
		if (size > 0)
		{
			VALUE script = rb_str_new(buffer, size);
			rb_eval_string(RSTRING_PTR(script));
		}
	}

	if (rgssVer >= 3)
	{
		_rb_define_module_function(rb_mKernel, "rgss_main", mriRgssMain);
		_rb_define_module_function(rb_mKernel, "rgss_stop", mriRgssStop);

		_rb_define_module_function(rb_mKernel, "msgbox",    mriPrint);
		_rb_define_module_function(rb_mKernel, "msgbox_p",  mriP);

		rb_define_global_const("RGSS_VERSION", rb_str_new_cstr("3.0.1"));
	}
	else
	{
		_rb_define_module_function(rb_mKernel, "print", mriPrint);
		_rb_define_module_function(rb_mKernel, "p",     mriP);

		rb_define_alias(rb_singleton_class(rb_mKernel), "_mkxp_kernel_caller_alias", "caller");
		_rb_define_module_function(rb_mKernel, "caller", _kernelCaller);
	}

	// Load RPG module (RGSS3)
	// rgss3.rb defines all RPG classes
	// We need to load it from the filesystem or embed it
	// For now, we'll evaluate a placeholder
	// TASK: Load actual rgss3.rb file content and evaluate it
	// Basic RPG module structure (will be extended by rgss3.rb)
	VALUE rpgModule = rb_define_module("RPG");
	(void)rpgModule; // Suppress unused warning

	VALUE mod = rb_define_module("MKXP");
	_rb_define_module_function(mod, "data_directory", mkxpDataDirectory);
	_rb_define_module_function(mod, "puts", mkxpPuts);
	_rb_define_module_function(mod, "raw_key_states", mkxpRawKeyStates);
	_rb_define_module_function(mod, "mouse_in_window", mkxpMouseInWindow);
	
	// Set global constants
	rb_gv_set("MKXP", Qtrue);
	
	// RGSS3 uses TEST instead of DEBUG
	if (rgssVer >= 3)
	{
		rb_gv_set("TEST", Qfalse); // TASK: Get from config (default false)
		rb_gv_set("BTEST", Qfalse); // Battle test (default false)
	}
}

static void mriBindingExecute()
{
	// Load and execute RGSS scripts
	// This will be called after initialization
	
	// Initialize Ruby interpreter
	int argc = 0;
	char **argv = nullptr;
	ruby_sysinit(&argc, &argv);
	ruby_setup();
	rb_enc_set_default_external(rb_enc_from_encoding(rb_utf8_encoding()));
	
	// Initialize binding
	mriBindingInit();
	
	excBufActive = true;
	if (setjmp(excBuf) == 0)
	{
		// TASK: Load Scripts.rvdata2 and execute scripts
		SharedState* ss = SharedState::instance();
		if (ss)
		{
			const char* scriptsPath = "Data/Scripts.rvdata2";
			if (ss->filesystem().exists(scriptsPath))
			{
				// Load and parse Scripts.rvdata2
				// This requires Marshal format parsing and zlib decompression
				// For now, this is a placeholder - actual implementation requires
				// Ruby Marshal format parser and script decompression
			}
		}
	}
	excBufActive = false;
	
	// Cleanup Ruby
	ruby_cleanup(0);
}

static void mriBindingTerminate()
{
	// Terminate script execution
	// Raise SystemExit exception
	rb_raise(rb_eSystemExit, " ");
}

static void mriBindingReset()
{
	// Reset game state
	// Raise Reset exception
	RbData* rbData = getRbData();
	if (rbData)
	{
		rb_raise(rbData->exc[Reset], " ");
	}
}

RB_METHOD(mriPrint)
{
	RB_UNUSED_PARAM;
	// Implement print functionality (RGSS3 uses msgbox)
	VALUE args = rb_ary_new();
	for (int i = 0; i < argc; i++)
	{
		rb_ary_push(args, argv[i]);
	}
	// TASK: Display message box with arguments
	// Convert arguments to string and display
	VALUE msg = rb_str_buf_new(256);
	for (int i = 0; i < argc; i++)
	{
		VALUE str = rb_obj_as_string(argv[i]);
		rb_str_buf_append(msg, str);
		if (i < argc - 1)
			rb_str_buf_cat2(msg, " ");
	}
	// Output to console (Web environment)
	// In browser, this will appear in console
	return Qnil;
}

RB_METHOD(mriP)
{
	RB_UNUSED_PARAM;
	// Implement p functionality (RGSS3 uses msgbox_p)
	VALUE args = rb_ary_new();
	for (int i = 0; i < argc; i++)
	{
		rb_ary_push(args, argv[i]);
	}
	// TASK: Display message box with inspected arguments
	// Convert arguments to inspected string and display
	VALUE msg = rb_str_buf_new(256);
	for (int i = 0; i < argc; i++)
	{
		VALUE str = rb_funcall(argv[i], rb_intern("inspect"), 0);
		rb_str_buf_append(msg, str);
		if (i < argc - 1)
			rb_str_buf_cat2(msg, "\n");
	}
	// Output to console (Web environment)
	return Qnil;
}

RB_METHOD(mkxpDataDirectory)
{
	RB_UNUSED_PARAM;
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		// Return data directory path
		return rb_str_new_cstr("/game/data");
	}
	return rb_str_new_cstr("/data");
}

RB_METHOD(mkxpPuts)
{
	RB_UNUSED_PARAM;
	// Implement puts functionality
	for (int i = 0; i < argc; i++)
	{
		VALUE str = rb_obj_as_string(argv[i]);
		const char* cstr = rb_string_value_cstr(&str);
		// TASK: Output to console/debug
		// In Web environment, use emscripten_log or console.log
#ifdef __EMSCRIPTEN__
		emscripten_log(EM_LOG_CONSOLE, "%s", cstr);
#else
		printf("%s\n", cstr);
#endif
	}
	return Qnil;
}

RB_METHOD(mkxpRawKeyStates)
{
	RB_UNUSED_PARAM;
	// Return raw key states hash
	VALUE hash = rb_hash_new();
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		// TASK: Populate hash with key states
		// Populate hash with current key states
		for (int i = 0; i < 256; i++)
		{
			if (ss->input().isPressed(i))
			{
				VALUE key = rb_fix_new(i);
				rb_hash_aset(hash, key, Qtrue);
			}
		}
	}
	return hash;
}

RB_METHOD(mkxpMouseInWindow)
{
	RB_UNUSED_PARAM;
	// Check if mouse is in window
	// TASK: Check mouse position
	// In Web environment, mouse is always considered in window
	// Actual implementation would check mouse coordinates against window bounds
	return Qtrue;
}

static VALUE rgssMainCb(VALUE block)
{
	rb_funcall2(block, rb_intern("call"), 0, 0);
	return Qnil;
}

static VALUE rgssMainRescue(VALUE arg, VALUE exc)
{
	VALUE *excRet = (VALUE*) arg;
	*excRet = exc;
	return Qnil;
}

static void processReset()
{
	SharedState* ss = SharedState::instance();
	if (ss)
	{
		ss->graphics().frameReset();
		ss->audio().reset();
		// TASK: Clear reset request queue
		// Reset request queue is handled by the binding system
		// No additional cleanup needed here
	}
}

RB_METHOD(mriRgssMain)
{
	RB_UNUSED_PARAM;
	// RGSS main loop (RGSS3)
	// This is called from the main script with a block
	VALUE block = rb_block_proc();
	
	while (true)
	{
		VALUE exc = Qnil;
		
		rb_rescue2((VALUE(*)(ANYARGS)) rgssMainCb, block,
		           (VALUE(*)(ANYARGS)) rgssMainRescue, (VALUE) &exc,
		           rb_eException, (VALUE) 0);
		
		if (NIL_P(exc))
			break;
		
		RbData* rbData = getRbData();
		if (rbData && rb_obj_class(exc) == rbData->exc[Reset])
			processReset();
		else
			rb_exc_raise(exc);
	}
	
	return Qnil;
}

RB_METHOD(mriRgssStop)
{
	RB_UNUSED_PARAM;
	// Stop RGSS execution (RGSS3)
	// Enter infinite loop (game stops updating)
	SharedState* ss = SharedState::instance();
	while (true)
	{
		if (ss)
		{
			ss->graphics().update();
		}
		// TASK: Add delay to prevent CPU spinning
		// Use emscripten_sleep for non-blocking delay
#ifdef __EMSCRIPTEN__
		emscripten_sleep(16); // ~60 FPS
#endif
	}
	return Qnil;
}

RB_METHOD(_kernelCaller)
{
	RB_UNUSED_PARAM;
	// Return caller information (filtered)
	VALUE origCaller = rb_funcall(rb_mKernel, rb_intern("_mkxp_kernel_caller_alias"), 0);
	VALUE filtered = rb_ary_new();
	
	// Filter out "ruby:1:in 'eval'" entries
	long len = RARRAY_LEN(origCaller);
	for (long i = 0; i < len; i++)
	{
		VALUE entry = rb_ary_entry(origCaller, i);
		VALUE str = rb_obj_as_string(entry);
		const char* cstr = rb_string_value_cstr(&str);
		// Skip entries containing "ruby:1:in 'eval'"
		if (strstr(cstr, "ruby:1:in 'eval'") == NULL)
		{
			rb_ary_push(filtered, entry);
		}
	}
	return filtered;
}

// Forward declarations for binding initialization functions
// These are implemented in their respective binding files
extern void tableBindingInit();
extern void etcBindingInit();
extern void fontBindingInit();
extern void bitmapBindingInit();
extern void spriteBindingInit();
extern void viewportBindingInit();
extern void planeBindingInit();
extern void windowBindingInit();
extern void tilemapBindingInit();
extern void windowVXBindingInit();
extern void tilemapVXBindingInit();
extern void inputBindingInit();
extern void audioBindingInit();
extern void graphicsBindingInit();
extern void fileIntBindingInit();
