#ifndef _WAV_H_
#define _WAV_H_
#include <stdint.h>
#pragma pack(push, 1)
struct WAVHEADER {
    char RIFF[4];            // "RIFF"
    uint32_t chunkSize;
    char WAVE[4];            // "WAVE"

    char fmt[4];             // "fmt "
    uint32_t fmtSize;        // 16 for PCM
    uint16_t audioFormat;    // 1 = PCM
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;

    char data[4];            // "data"
    uint32_t dataSize;       // PCM data size
};
#pragma pack(pop)
#endif