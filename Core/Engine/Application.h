#ifndef _APPLICATION_
#define _APPLICATION_
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <fstream>
#include "Event.h"
#include "WAV.h"

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
	
	virtual int ReadWAVHeader(std::ifstream& ifs, WAVHEADER& wave_header);
	virtual int LoadWAVFromDisk(std::ifstream& file_path, WAVHEADER& wave_header);
	virtual int RenderAudioData(const WAVHEADER& header);
	virtual uint32_t UploadDataToAudioBuffer(BYTE * audio_data, uint32_t bytes_to_write, uint32_t block_align);
	virtual void Run() = 0;
};


#endif