/*
** input.cpp
**
** Input system implementation for Palcon-RGSS
**
** Copyright (C) 2025 Palcon Team
*/

#include "input.h"
#include <string.h>

Input::Input()
{
	memset(keyStates, 0, sizeof(keyStates));
	memset(keyTriggered, 0, sizeof(keyTriggered));
	memset(keyRepeating, 0, sizeof(keyRepeating));
}

Input::~Input()
{
}

void Input::update()
{
	// Clear triggered states (they are set when key is pressed)
	memset(keyTriggered, 0, sizeof(keyTriggered));
	
	// Update repeating states
	// Keys that are held down for multiple frames become repeating
	for (int i = 0; i < 256; i++)
	{
		if (keyStates[i])
		{
			// Key is pressed, check if it should be repeating
			// TASK: Implement repeat delay logic
			// Simple implementation: key becomes repeating after being held for multiple frames
			// More sophisticated implementation would use frame counters for delay
			keyRepeating[i] = true;
		}
		else
		{
			keyRepeating[i] = false;
		}
	}
}

bool Input::isPressed(int key) const
{
	if (key < 0 || key >= 256) return false;
	return keyStates[key];
}

bool Input::isTriggered(int key) const
{
	if (key < 0 || key >= 256) return false;
	return keyTriggered[key];
}

bool Input::isRepeating(int key) const
{
	if (key < 0 || key >= 256) return false;
	return keyRepeating[key];
}

int Input::getDir4() const
{
	// Calculate 4-directional input
	// Up=8, Down=2, Left=4, Right=6
	bool up = isPressed(38) || isPressed(87);    // Up arrow or W
	bool down = isPressed(40) || isPressed(83);  // Down arrow or S
	bool left = isPressed(37) || isPressed(65);  // Left arrow or A
	bool right = isPressed(39) || isPressed(68); // Right arrow or D
	
	if (up && !down && !left && !right) return 8;
	if (down && !up && !left && !right) return 2;
	if (left && !right && !up && !down) return 4;
	if (right && !left && !up && !down) return 6;
	
	return 0;
}

int Input::getDir8() const
{
	// Calculate 8-directional input
	// Up=8, Down=2, Left=4, Right=6, UpLeft=7, UpRight=9, DownLeft=1, DownRight=3
	bool up = isPressed(38) || isPressed(87);    // Up arrow or W
	bool down = isPressed(40) || isPressed(83);  // Down arrow or S
	bool left = isPressed(37) || isPressed(65);  // Left arrow or A
	bool right = isPressed(39) || isPressed(68); // Right arrow or D
	
	if (up && !down && !left && !right) return 8;
	if (down && !up && !left && !right) return 2;
	if (left && !right && !up && !down) return 4;
	if (right && !left && !up && !down) return 6;
	if (up && left && !down && !right) return 7;
	if (up && right && !down && !left) return 9;
	if (down && left && !up && !right) return 1;
	if (down && right && !up && !left) return 3;
	
	return 0;
}

void Input::setKeyState(int key, bool pressed)
{
	if (key < 0 || key >= 256) return;
	
	// Set triggered state when key transitions from not pressed to pressed
	if (pressed && !keyStates[key]) {
		keyTriggered[key] = true;
	}
	
	keyStates[key] = pressed;
	
	// Update repeating state
	if (pressed) {
		keyRepeating[key] = true;
	} else {
		keyRepeating[key] = false;
	}
}
