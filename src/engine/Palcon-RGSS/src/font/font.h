/*
** font.h
**
** Font class for Palcon-RGSS
** Based on mkxp font.h
**
** Copyright (C) 2025 Palcon Team
*/

#ifndef FONT_H
#define FONT_H

#include "../etc/color.h"
#include <string>
#include <vector>

class Font
{
public:
	Font(const std::vector<std::string>* names = nullptr, int size = 24);
	Font(const Font &other);
	~Font();
	
	static bool doesExist(const char* name);
	
	void initDynAttribs();
	
	Color& getColor() { return color; }
	Color& getOutColor() { return outColor; }
	
	std::vector<std::string> names;
	int size;
	
	// Default font properties (static)
	static std::string defaultName;
	static int defaultSize;
	static bool defaultBold;
	static bool defaultItalic;
	static Color defaultColor;
	static bool defaultOutline;
	static Color defaultOutlineColor;
	static bool defaultShadow;
	static Color defaultShadowColor;
	
private:
	Color color;
	Color outColor; // RGSS3
};

#endif // FONT_H
