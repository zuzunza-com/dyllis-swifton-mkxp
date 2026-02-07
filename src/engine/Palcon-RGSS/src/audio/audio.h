/*
** audio.h
**
** Audio system for Palcon-RGSS
** Based on mkxp audio.h
**
** Copyright (C) 2025 Palcon Team
*/

#ifndef AUDIO_H
#define AUDIO_H

class Audio
{
public:
	Audio();
	~Audio();

	void update();

	// BGM
	void bgmPlay(const char* filename, int volume = 100, int pitch = 100, double pos = 0.0);
	void bgmStop();
	void bgmFade(int duration);
	double bgmPos() const;

	// BGS
	void bgsPlay(const char* filename, int volume = 100, int pitch = 100, double pos = 0.0);
	void bgsStop();
	void bgsFade(int duration);
	double bgsPos() const;

	// ME
	void mePlay(const char* filename, int volume = 100, int pitch = 100);
	void meStop();
	void meFade(int duration);

	// SE
	void sePlay(const char* filename, int volume = 100, int pitch = 100);
	void seStop();

	void setupMidi();
	void reset();

private:
	// Audio state
	double bgmPosition;
	double bgsPosition;
	bool midiSetup;
};

#endif // AUDIO_H
