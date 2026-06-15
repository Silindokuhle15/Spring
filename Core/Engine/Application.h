#ifndef _APPLICATION_
#define _APPLICATION_
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include "Event.h"
#include "Sound.h"
#include <stdexcept>

class Application : public event::IEventListener
{
private:
	IMMDeviceEnumerator* m_IMMDeviceEnumerator;
	IMMDevice* m_IMMDevice;
	IAudioClient* m_AudioClient;
	IAudioRenderClient* m_AudioRenderClient;
protected:
	uint32_t m_AudioBufferFrameCount{ 0 };
	BYTE* m_AudioData;
	WAVEFORMATEX* m_DefaultWAVMixFormat;
public:
	bool m_ExitApplication;
public:

	virtual void ReleaseAudioBuffer()
	{
		delete[] m_AudioData;
		m_AudioData = nullptr;
	}
	Application();
	~Application();
	
	virtual int RenderAudioData(Sound& sound);
	virtual void Run() = 0;
};

#endif