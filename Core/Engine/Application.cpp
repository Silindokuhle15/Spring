#include "Application.h"
#include <fstream>

void Application::advanceAudioMix()
{
	m_CurrentMixFrame = (m_CurrentMixFrame >= m_CurrentMixFrameCount) ? 0 : m_CurrentMixFrame;
	for (auto& request : m_AudioRequestQueue)
	{
		auto& currentFrame = request.currentFrame;
		auto& frameCount = request.frameCount;
		if (currentFrame >= frameCount)
		{
			continue;
		}
		auto& assetHandle = request.SoundID;
		auto& sound = m_SoundMap[assetHandle];
		m_CurrentMixFrameCount = max<uint32_t>(m_CurrentMixFrameCount, sound.frameCount);
		if (sound.numChannels == 1)
		{

		}
		for (auto frameIndex = currentFrame; frameIndex < frameCount; frameIndex++)
		{
			if (sound.bitsPerSample == 16)
			{
				int16_t* s = reinterpret_cast<int16_t*>(sound.PCMData);

				if (sound.numChannels == 1)
				{
					m_AudioMixBuffer[2 * frameIndex + 0] = s[frameIndex];
					m_AudioMixBuffer[2 * frameIndex + 1] = s[frameIndex];
				}
				if (sound.numChannels == 2)
				{
					m_AudioMixBuffer[frameIndex] = s[frameIndex] / 32768.0f;
				}
			}
			if (sound.bitsPerSample == 32)
			{
				float* s = reinterpret_cast<float*> (sound.PCMData);

				if (sound.numChannels == 1)
				{
					m_AudioMixBuffer[2 * frameIndex + 0] += 0.25 * (min<float>(1.0f, max<float>(s[frameIndex], -1.0f)));
					m_AudioMixBuffer[2 * frameIndex + 1] += 0.25 * (min<float>(1.0f, max<float>(s[frameIndex], -1.0f)));
				}
				if (sound.numChannels == 2)
				{
					m_AudioMixBuffer[frameIndex] += s[frameIndex] / 32768.0f;
				}
			}
		}
	}
	uint32_t remainingFrames = m_CurrentMixFrameCount - m_CurrentMixFrame;
	if (remainingFrames > 0 && !m_ExitApplication.load())
	{
		float* src = m_AudioMixBuffer.data() + m_CurrentMixFrame;
		uint32_t padding{ 0 };
		uint32_t framesAvailable{ 0 };
		uint32_t framesToWrite{ 0 };
		BYTE* physicalAudioBuffer{ nullptr };
		m_AudioClient->GetCurrentPadding(&padding);
		framesAvailable = m_AudioBufferFrameCount - padding;
		framesToWrite = min<uint32_t>(framesAvailable, remainingFrames);
		m_AudioRenderClient->GetBuffer(framesToWrite, &physicalAudioBuffer);
		memcpy(physicalAudioBuffer, src, framesToWrite * m_DefaultWAVMixFormat->nChannels * sizeof(float));
		m_AudioRenderClient->ReleaseBuffer(framesToWrite, 0);
		remainingFrames -= framesToWrite;
		for (auto index = m_CurrentMixFrame; index < m_CurrentMixFrame + framesToWrite; index++)
		{
			m_AudioMixBuffer[index] = 0.0f;
		}
		for (auto& request : m_AudioRequestQueue)
		{
			request.currentFrame += framesToWrite;
		}
		m_CurrentMixFrame += framesToWrite;
	}
	std::erase_if(m_AudioRequestQueue, [&](primitives::PlaySoundRequest& request) { return request.currentFrame > -request.frameCount; });
}

Application::Application():
	m_IMMDeviceEnumerator{nullptr},
	m_IMMDevice{nullptr},
	m_AudioClient{ nullptr},
	m_AudioRenderClient{nullptr},
	m_AudioBufferFrameCount{ 0 },
	m_AudioMixMaxFrameCount{ 65536 },
	m_CurrentMixFrame{ 0 },
	m_CurrentMixFrameCount{ 0 },
	m_AudioMixBuffer(size_t(m_AudioMixMaxFrameCount) , 0.0f),
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

bool Application::LoadAudioSourcesFromFile(std::ifstream& ifs)
{
	if (!ifs.is_open())
	{
		throw std::runtime_error("Failed to open file for reading !!!");
	}
	PakHeader pakHeader{};
	ifs.read(
		reinterpret_cast<char*>(&pakHeader),
		sizeof(PakHeader)
	);
	assert(pakHeader.magic == 0x4B434150);
	for (auto itemIndex = 0; itemIndex < pakHeader.itemCount; itemIndex++)
	{
		AssetHandle assetHandle{ 0,0 };
		ifs.read(
			reinterpret_cast<char*>(&assetHandle),
			sizeof(AssetHandle)
		);
		PakHeader itemHeader{};
		ifs.read(
			reinterpret_cast<char*>(&itemHeader),
			sizeof(PakHeader)
		);
		assert(itemHeader.magic == 0x4B434150);
		assert(itemHeader.itemCount == 1);
		auto sound = SoundReader::ReadSoundClipFromFile(ifs, soundBlockAllocator);
		m_SoundMap[assetHandle] = sound;
	}
	return true;
}

int Application::RenderAudiRequest(primitives::PlaySoundRequest& request)
{
	auto& assetHandle = request.SoundID;
	if (!m_SoundMap.contains(assetHandle))
	{
		return -1;
	}
	auto& sound = m_SoundMap[assetHandle];
	request.currentFrame = 0;
	request.frameCount = sound.frameCount;
	m_AudioRequestQueue.emplace_back(std::move(request));
	return 0;
}
