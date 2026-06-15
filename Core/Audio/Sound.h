#ifndef _SOUND_H_
#define _SOUND_H_
#include "WAV.h"
struct Sound
{
	bool playing;
	bool reserved[3];
	uint32_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
	uint32_t currentFrame;
	uint32_t frameCount;
	uint8_t* PCMData;
};

#endif