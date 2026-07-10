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
	uint32_t m_CurrentMixFrame;
	uint32_t m_CurrentMixFrameCount;
	WAVEFORMATEX* m_DefaultWAVMixFormat;
	BlockAllocator<BYTE> soundBlockAllocator;
	std::map<AssetHandle, Sound> m_SoundMap;
	std::vector<primitives::PlaySoundRequest> m_AudioRequestQueue;
	std::vector<float> m_AudioMixBuffer;
	void advanceAudioMix();
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