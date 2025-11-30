#ifndef _APPLICATION_
#define _APPLICATION_
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <string>
#include "Event.h"

class Application : public event::IEventListener
{
private:
	IMMDeviceEnumerator* m_IMMDeviceEnumerator;
	IMMDevice* m_IMMDevice;
	IAudioClient* m_AudioClient;
	IAudioRenderClient* m_AudioRenderClient;
public:
	Application();
	~Application();
	virtual int RenderAudioData(const std::string& wave_file_path);
	virtual void Run() = 0;
};


#endif