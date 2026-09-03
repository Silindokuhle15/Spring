#include "Application.h"
#include <fstream>

void Application::advanceAudioMix()
{
	auto& audioReadBuffer = m_AudioMixBuffer[m_AudioReadIndex];
	uint32_t remainingFrames = audioReadBuffer.bufferCapacity - audioReadBuffer.currentFrame;
	uint32_t framesAvailable{ 0 };
	uint32_t framesToWrite{ 0 };
	uint32_t padding{ 0 };

	while (remainingFrames > 0 && !m_ExitApplication.load())
	{
		float* src = audioReadBuffer.data.data() + audioReadBuffer.currentFrame;
		BYTE* physicalAudioBuffer{ nullptr };
		m_AudioClient->GetCurrentPadding(&padding);
		framesAvailable = m_AudioBufferFrameCount - padding;
		framesToWrite = std::min(framesAvailable, remainingFrames);
		m_AudioRenderClient->GetBuffer(framesToWrite, &physicalAudioBuffer);
		memcpy(physicalAudioBuffer, src, framesToWrite * m_DefaultWAVMixFormat->nChannels * sizeof(float));
		m_AudioRenderClient->ReleaseBuffer(framesToWrite, 0);
		audioReadBuffer.currentFrame += framesToWrite;
		remainingFrames -= framesToWrite;
	}
	uint32_t newest = m_AudioReady.exchange(m_AudioReadIndex, std::memory_order_acq_rel);
	if (newest == m_AudioReadIndex)
	{
		return;
	}
	m_AudioReadIndex = newest;
}

void Application::recordAudioBuffer()
{
	std::erase_if(m_AudioRequestQueue, [&](primitives::PlaySoundRequest& request) { return request.currentFrame >= request.frameCount; });
	auto& audioWriteBuffer = m_AudioMixBuffer[m_AudioWriteIndex];
	std::fill(audioWriteBuffer.data.begin(), audioWriteBuffer.data.end(), 0.0f);
	for (auto& request : m_AudioRequestQueue)
	{
		auto& currentFrame = request.currentFrame;
		auto& frameCount = request.frameCount;
		auto& assetHandle = request.SoundID;
		auto& sound = m_SoundMap[assetHandle];
		uint32_t framesToMix = std::min(audioWriteBuffer.bufferCapacity, frameCount - currentFrame);
		const AudioFormat format = getAudioFormat(sound.bitsPerSample,sound.numChannels);
		switch (format)
		{
		case AudioFormat::Format16BitMono:
			mix16BitMono(audioWriteBuffer, sound, currentFrame, framesToMix);
			break;
		case AudioFormat::Format16BitStereo:
			mix16BitStereo(audioWriteBuffer, sound, currentFrame, framesToMix);
			break;
		case AudioFormat::Format32BitMono:
			mix32BitMono(audioWriteBuffer, sound, currentFrame, framesToMix);
			break;
		case AudioFormat::Format32BitStereo:
			mix32BitStereo(audioWriteBuffer, sound, currentFrame, framesToMix);
			break;
		}
		request.currentFrame += framesToMix;
	}
	for (size_t index = 0; index < audioWriteBuffer.bufferCapacity; index++)
	{
		auto& sample = audioWriteBuffer.data[index];
		audioWriteBuffer.data[index] = std::clamp(sample, -1.0f, 1.0f);
	}
	audioWriteBuffer.currentFrame = 0;
	m_AudioWriteIndex = m_AudioReady.exchange(m_AudioWriteIndex, std::memory_order_acq_rel);
}

void Application::mix16BitMono(AudioBuffer& write_buffer, Sound& sound, uint32_t start_frame, uint32_t frames_to_mix)
{
	const int16_t* s = reinterpret_cast<const int16_t*>(sound.PCMData);
	const float volume = m_MasterVolume;
	const float int16Scale = 1.0f / 32768.0f;
	for (uint32_t frameIndex = 0; frameIndex < frames_to_mix; frameIndex++)
	{
		uint32_t requestReadIndex = frameIndex + start_frame;
		uint32_t writeIndex = frameIndex * 2;
		auto sample = s[requestReadIndex] * int16Scale;
		write_buffer.data[writeIndex + 0] += volume * sample;
		write_buffer.data[writeIndex + 1] += volume * sample;
	}
}

void Application::mix16BitStereo(AudioBuffer& write_buffer, Sound& sound, uint32_t start_frame, uint32_t frames_to_mix)
{
	const int16_t* s = reinterpret_cast<const int16_t*>(sound.PCMData);
	const float volume = m_MasterVolume;
	const float int16Scale = 1.0f / 32768.0f;
	for (uint32_t frameIndex = 0; frameIndex < frames_to_mix; frameIndex++)
	{
		uint32_t requestReadIndex = frameIndex + start_frame;
		uint32_t writeIndex = frameIndex * 2;
		write_buffer.data[writeIndex + 0] += volume * s[requestReadIndex * 2 + 0] * int16Scale;
		write_buffer.data[writeIndex + 1] += volume * s[requestReadIndex * 2 + 1] * int16Scale;
	}
}

void Application::mix32BitMono(AudioBuffer& write_buffer, Sound& sound, uint32_t start_frame, uint32_t frames_to_mix)
{
	const float* s = reinterpret_cast<const float*>(sound.PCMData);
	const float volume = m_MasterVolume;
	for (uint32_t frameIndex = 0; frameIndex < frames_to_mix; frameIndex++)
	{
		uint32_t requestReadIndex = frameIndex + start_frame;
		uint32_t writeIndex = frameIndex * 2;
		auto sample = s[requestReadIndex];
		write_buffer.data[writeIndex + 0] += volume * sample;
		write_buffer.data[writeIndex + 1] += volume * sample;
	}
}

void Application::mix32BitStereo(AudioBuffer& write_buffer, Sound& sound, uint32_t start_frame, uint32_t frames_to_mix)
{
	const float* s = reinterpret_cast<const float*>(sound.PCMData);
	const float volume = m_MasterVolume;
	for (uint32_t frameIndex = 0; frameIndex < frames_to_mix; frameIndex++)
	{
		uint32_t requestReadIndex = frameIndex + start_frame;
		uint32_t writeIndex = frameIndex * 2;
		write_buffer.data[writeIndex + 0] += volume * s[requestReadIndex * 2 + 0];
		write_buffer.data[writeIndex + 1] += volume * s[requestReadIndex * 2 + 1];
	}
}

AudioFormat Application::getAudioFormat(uint32_t bits_per_sample, uint32_t num_channels)
{
	if (bits_per_sample == 16 && num_channels == 1) { return AudioFormat::Format16BitMono; }
	if (bits_per_sample == 16 && num_channels == 2) { return AudioFormat::Format16BitStereo; }
	if (bits_per_sample == 32 && num_channels == 1) { return AudioFormat::Format32BitMono; }
	if (bits_per_sample == 32 && num_channels == 2) { return AudioFormat::Format32BitStereo; }
	throw std::runtime_error("Unsupported Audio Format !!!");
}

Application::Application():
	m_IMMDeviceEnumerator{nullptr},
	m_IMMDevice{nullptr},
	m_AudioClient{ nullptr},
	m_AudioRenderClient{nullptr},
	m_AudioBufferFrameCount{ 0 },
	m_MasterVolume{0.25f},
	m_AudioMixBuffer{},
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
	m_AudioMixBuffer[0] = AudioBuffer(0, AudioBufferCapacity, std::vector<float>(size_t(AudioBufferCapacity2x), 0.0f));
	m_AudioMixBuffer[1] = AudioBuffer(0, AudioBufferCapacity, std::vector<float>(size_t(AudioBufferCapacity2x), 0.0f));
	m_AudioMixBuffer[2] = AudioBuffer(0, AudioBufferCapacity, std::vector<float>(size_t(AudioBufferCapacity2x), 0.0f));
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
	const PakHeader pakHeader = PakReader::ReadPakHeader(ifs);
	for (auto itemIndex = 0; itemIndex < pakHeader.itemCount; itemIndex++)
	{
		const PakHeader itemHeader = PakReader::ReadPakHeader(ifs);
		assert(itemHeader.itemCount == 1);
		const AssetHandle assetHandle = PakReader::ReadAssetHandle(ifs);
		auto sound = SoundReader::ReadSoundClipFromFile(ifs, soundBlockAllocator);
		ifs.seekg(itemHeader.offset + itemHeader.size);
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
