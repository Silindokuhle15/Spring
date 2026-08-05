#ifndef _APPLICATION_
#define _APPLICATION_
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <atomic>
#include <map>
#include "Event.h"
#include "ObjectLoader.h"

template<typename T>
T min(T a, T b)
{
	return a <= b ? a : b;
}
template<typename T>
T max(T a, T b)
{
	return a >= b ? a : b;
}
constexpr uint32_t AudioBufferCapacity = 256;
constexpr uint32_t AudioBufferCapacity2x = AudioBufferCapacity * 2;

struct AudioBuffer
{
	uint32_t currentFrame = 0;
	uint32_t bufferCapacity = AudioBufferCapacity;
	std::vector<float> data;
};
constexpr enum AudioFormat
{
	Format16BitMono = 1,
	Format16BitStereo,
	Format32BitMono,
	Format32BitStereo
};

class Application : public event::IEventListener
{
private:
	IMMDeviceEnumerator* m_IMMDeviceEnumerator;
	IMMDevice* m_IMMDevice;
	IAudioClient* m_AudioClient;
	IAudioRenderClient* m_AudioRenderClient;
protected:
	uint32_t m_AudioBufferFrameCount;
	uint32_t m_AudioMixMaxFrameCount;
	WAVEFORMATEX* m_DefaultWAVMixFormat;
	BlockAllocator<BYTE> soundBlockAllocator;
	std::map<AssetHandle, Sound> m_SoundMap;
	std::vector<primitives::PlaySoundRequest> m_AudioRequestQueue;
	float m_MasterVolume;
	uint32_t m_AudioWriteIndex = 0;
	uint32_t m_AudioReadIndex = 1;
	std::atomic<uint32_t> m_AudioReady{ 2 };
	AudioBuffer m_AudioMixBuffer[3];
	void advanceAudioMix();
	void recordAudioBuffer();
	void mix16BitMono(AudioBuffer& write_buffer, Sound& sound, uint32_t start_frame, uint32_t frames_to_mix);
	void mix16BitStereo(AudioBuffer& write_buffer, Sound& sound, uint32_t start_frame, uint32_t frames_to_mix);
	void mix32BitMono(AudioBuffer& write_buffer, Sound& sound, uint32_t start_frame, uint32_t frames_to_mix);
	void mix32BitStereo(AudioBuffer& write_buffer, Sound& sound, uint32_t start_frame, uint32_t frames_to_mix);
	AudioFormat getAudioFormat(uint32_t bits_per_sample, uint32_t num_channels);
public:
	std::atomic_bool m_ExitApplication;
public:

	virtual void ReleaseAudioBuffer()
	{
	}
	Application();
	~Application();
	
	bool LoadAudioSourcesFromFile(std::ifstream& ifs);
	virtual int RenderAudiRequest(primitives::PlaySoundRequest& request);
	virtual void Run() = 0;
};

#endif