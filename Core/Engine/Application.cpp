#include "Application.h"

Application::Application():
	m_IMMDeviceEnumerator{nullptr},
	m_IMMDevice{nullptr},
	m_AudioClient{ nullptr},
	m_AudioRenderClient{nullptr},
	m_AudioData{nullptr},
	m_DefaultWAVMixFormat{nullptr},
	m_ExitApplication{false}
{
	auto initialized = CoInitialize(nullptr);

	auto deviceEnumerator = CoCreateInstance(
		__uuidof(MMDeviceEnumerator),
		nullptr,
		CLSCTX_ALL,
		IID_PPV_ARGS(&m_IMMDeviceEnumerator)
	);

	auto defaultAudioEndpoint = m_IMMDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_IMMDevice);

	auto deviceActivate = m_IMMDevice->Activate(
		__uuidof(IAudioClient),
		CLSCTX_ALL,
		nullptr,
		(void**)&m_AudioClient
	);

	auto hr = m_AudioClient->GetMixFormat(&m_DefaultWAVMixFormat);
	if (FAILED(hr)) 
	{
		throw std::runtime_error("AudioClient:GetMixFormat failed: !!!");
	}

	DWORD streamFlags{ 0 };
	REFERENCE_TIME bufferDuration{ 0 };
	REFERENCE_TIME periodicity{ 0 };

	hr = m_AudioClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		streamFlags,
		bufferDuration,
		periodicity,
		m_DefaultWAVMixFormat,
		nullptr);
	if (FAILED(hr))
	{
		throw std::runtime_error("AudioClient:Initialize failed: !!!");
	}

	hr = m_AudioClient->GetBufferSize(&m_AudioBufferFrameCount);
	if (FAILED(hr)) 
	{
		throw std::runtime_error("AudioClient:GetBufferSize failed: !!!");
	}

	hr = m_AudioClient->GetService(IID_PPV_ARGS(&m_AudioRenderClient));
	if (FAILED(hr)) 
	{
		throw std::runtime_error("AudioClient:GetService(IAudioRenderClient) failed: !!!");
	}
	m_AudioClient->Start();
}

Application::~Application()
{
	m_IMMDevice->Release();
	m_IMMDeviceEnumerator->Release();
	CoUninitialize();
}

int Application::RenderAudioData(Sound& sound)
{
	size_t sampleCount = sound.frameCount * sound.numChannels;
	std::vector<float> audioBuffer(sampleCount);
	if (sound.numChannels == 1) // mono audio
	{
		audioBuffer.resize(sampleCount * 2);
	}
	// Will be used later for mixing multiple audio sources
	for (size_t index = 0; index < sampleCount; index++)
	{
		if (sound.bitsPerSample == 16)
		{
			int16_t* s = reinterpret_cast<int16_t*>(sound.PCMData);

			if (sound.numChannels == 1)
			{
				audioBuffer[2 * index + 0] = s[index];
				audioBuffer[2 * index + 1] = s[index];
			}
			if (sound.numChannels == 2)
			{
				audioBuffer[index] = s[index] / 32768.0f;
			}
		}
		if (sound.bitsPerSample == 32)
		{
			float* s = reinterpret_cast<float*> (sound.PCMData);

			if (sound.numChannels == 1)
			{
				audioBuffer[2 * index + 0] = 0.25 * (min(1.0f, max(s[index], -1.0f)));
				audioBuffer[2 * index + 1] = 0.25 * (min(1.0f, max(s[index], -1.0f)));
			}
			if (sound.numChannels == 2)
			{
				audioBuffer[index] = s[index] / 32768.0f;
			}
		}
	}
	uint32_t remainingFrames = sound.frameCount - sound.currentFrame;
	while (remainingFrames > 0 && sound.playing && !m_ExitApplication)
	{
		float* src = audioBuffer.data() + sound.currentFrame * sound.numChannels;
		uint32_t padding{ 0 };
		uint32_t framesAvailable{ 0 };
		uint32_t framesToWrite{ 0 };
		BYTE* physicalAudioBuffer{ nullptr };
		m_AudioClient->GetCurrentPadding(&padding);
		framesAvailable = m_AudioBufferFrameCount - padding;
		framesToWrite = min(framesAvailable, remainingFrames);
		m_AudioRenderClient->GetBuffer(framesToWrite, &physicalAudioBuffer);
		memcpy(physicalAudioBuffer, src, framesToWrite * m_DefaultWAVMixFormat->nChannels * sizeof(float));
		m_AudioRenderClient->ReleaseBuffer(framesToWrite, 0);
		remainingFrames -= framesToWrite;
		sound.currentFrame += framesToWrite;
	}
	sound.currentFrame = 0;
	sound.playing = false;
	return 0;
}
