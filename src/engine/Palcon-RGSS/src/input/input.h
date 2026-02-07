/*
** input.h
**
** Input system for Palcon-RGSS
** Based on mkxp input.h
**
** Copyright (C) 2025 Palcon Team
*/

#ifndef INPUT_H
#define INPUT_H

class Input
{
public:
	Input();
	~Input();

	void update();

	bool isPressed(int key) const;
	bool isTriggered(int key) const;
	bool isRepeating(int key) const;

	int getDir4() const;
	int getDir8() const;

	void setKeyState(int key, bool pressed);

private:
	bool keyStates[256];
	bool keyTriggered[256];
	bool keyRepeating[256];
};

#endif // INPUT_H
