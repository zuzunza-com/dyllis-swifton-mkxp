/*
** module-rpg.cpp
**
** RPG Module Binding for Palcon-RGSS
** RGSS3 RPG classes binding
** Based on rgss3.rb structure
**
** Copyright (C) 2025 Palcon Team
*/

#include "../binding-util.h"
#include "../exception.h"
#include "../../sharedstate.h"
#include <ruby.h>
#include <string>

// RGSS3 RPG module classes
// Most classes are defined in rgss3.rb Ruby script
// This file ensures the module exists and can be accessed from C++

void moduleRpgInit()
{
	// Define RPG module
	VALUE rpgModule = rb_define_module("RPG");
	
	// Note: Most RPG classes are defined in rgss3.rb
	// We need to load and evaluate rgss3.rb content here
	// For now, we'll define basic structure
	
	// The actual class definitions will come from rgss3.rb:
	// - RPG::AudioFile
	// - RPG::BGM, RPG::BGS, RPG::ME, RPG::SE
	// - RPG::Map, RPG::Event, RPG::Actor, RPG::Class, etc.
	// - RPG::System, RPG::Tileset, RPG::Animation, etc.
	// - Table class
	
	// TASK: Load rgss3.rb content and evaluate it
	// This should be done after Ruby is initialized
	// Load from filesystem if available
	SharedState* ss = SharedState::instance();
	if (ss && ss->filesystem().exists("rgss3.rb"))
	{
		char buffer[1024 * 1024]; // 1MB buffer
		int size = ss->filesystem().readFile("rgss3.rb", buffer, sizeof(buffer));
		if (size > 0)
		{
			VALUE script = rb_str_new(buffer, size);
			rb_eval_string(RSTRING_PTR(script));
		}
	}
}
