/*
** font.cpp
**
** Font class implementation for Palcon-RGSS
**
** Copyright (C) 2025 Palcon Team
*/

#include "font.h"

// Default font properties
std::string Font::defaultName = "Arial";
int Font::defaultSize = 24;
bool Font::defaultBold = false;
bool Font::defaultItalic = false;
Color Font::defaultColor = Color(255, 255, 255, 255);
bool Font::defaultOutline = false;
Color Font::defaultOutlineColor = Color(0, 0, 0, 128);
bool Font::defaultShadow = false;
Color Font::defaultShadowColor = Color(0, 0, 0, 128);

Font::Font(const std::vector<std::string>* names, int size)
	: size(size > 0 ? size : defaultSize)
	, color(defaultColor)
	, outColor(defaultOutlineColor)
{
	if (names)
		this->names = *names;
	else
		this->names.push_back(defaultName);
}

Font::Font(const Font &other)
	: names(other.names)
	, size(other.size)
	, color(other.color)
	, outColor(other.outColor)
{
}

Font::~Font()
{
}

bool Font::doesExist(const char* name)
{
	// TASK: Check if font exists
	// For now, always return true
	// Future: Check system fonts
	(void)name;
	return true;
}

void Font::initDynAttribs()
{
	// Initialize dynamic attributes
	// This is called after Font object creation
}
