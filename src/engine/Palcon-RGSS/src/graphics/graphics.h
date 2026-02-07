/*
** graphics.h
**
** Graphics system for Palcon-RGSS
** Based on mkxp graphics.h
**
** Copyright (C) 2025 Palcon Team
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

class Graphics
{
public:
	Graphics();
	~Graphics();

	void update();
	void freeze();
	void transition(int duration, const char* filename, int vague);
	void frameReset();

	int getWidth() const { return width; }
	int getHeight() const { return height; }
	int getFrameRate() const { return frameRate; }
	int getFrameCount() const { return frameCount; }
	int getBrightness() const { return brightness; }

	void setFrameRate(int rate);
	void setBrightness(int bright);
	void resizeScreen(int w, int h);

private:
	int width;
	int height;
	int frameRate;
	int frameCount;
	int brightness;
};

#endif // GRAPHICS_H
