#include "Application.h"
#include <iostream>
#include <vector>

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

	auto defaultWAVMixFormat = m_AudioClient->GetMixFormat(&m_DefaultWAVMixFormat);
	if (FAILED(defaultWAVMixFormat)) {
		std::cout << "AudioClient:GetMixFormat failed: " << std::hex << defaultWAVMixFormat << "\n";
	}
}

Application::~Application()
{
	m_IMMDevice->Release();
	m_IMMDeviceEnumerator->Release();
	CoUninitialize();
}

int Application::ReadWAVHeader(std::ifstream& ifs, WAVHEADER& wave_header)
{
	if (!ifs) return -1;
	ZeroMemory(&wave_header, sizeof(WAVHEADER));
	ifs.read((char*)&wave_header, sizeof(WAVHEADER));
	if (strncmp(wave_header.RIFF, "RIFF", 4) != 0 ||
		strncmp(wave_header.WAVE, "WAVE", 4) != 0 ||
		wave_header.audioFormat != 1) return -1;
	return 0;
}

int Application::LoadWAVFromDisk(std::ifstream& file_input_stream, WAVHEADER& wave_header)
{
	if (ReadWAVHeader(file_input_stream, wave_header) == -1) return -1;
	m_AudioData = new BYTE[wave_header.dataSize];
	file_input_stream.read((char*)m_AudioData, wave_header.dataSize);
	return 0;
}

int Application::RenderAudioData(const WAVHEADER& header)
{
	const WAVEFORMATEX f{
		WAVE_FORMAT_PCM,
		header.numChannels,
		header.sampleRate,
		header.byteRate,
		header.blockAlign,
		header.bitsPerSample,
		0
	};

	DWORD streamFlags{ 0 };
	REFERENCE_TIME bufferDuration{ 0 };
	REFERENCE_TIME periodicity{ 0 };
	HRESULT hr;
	hr = m_AudioClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		streamFlags,
		bufferDuration,
		periodicity,
		&f,
		nullptr);
	if (FAILED(hr)) {
		std::cout << "AudioClient:Initialize failed: " << std::hex << hr << "\n";
		return -1;
	}

	hr = m_AudioClient->GetBufferSize(&m_AudioBufferFrameCount);
	if (FAILED(hr)) {
		std::cout << "AudioClient:GetBufferSize failed: " << std::hex << hr << "\n";
		return -1;
	}

	hr = m_AudioClient->GetService(IID_PPV_ARGS(&m_AudioRenderClient));
	if (FAILED(hr)) {
		std::cout << "AudioClient:GetService(IAudioRenderClient) failed: " << std::hex << hr << "\n";
		return -1;
	}

	m_AudioClient->Start();
	BYTE* audioPtr = m_AudioData;
	uint32_t remainingBytes = header.dataSize;
	while (remainingBytes > 0 && (!m_ExitApplication))
	{
		auto bytesWritten = UploadDataToAudioBuffer(audioPtr, remainingBytes, header.blockAlign);
		audioPtr += bytesWritten;
		remainingBytes -= bytesWritten;
		Sleep(1);
	}
	audioPtr = nullptr;
	m_AudioClient->Stop();
	m_AudioRenderClient->Release();
	m_AudioClient->Release();
	return 0;
}

uint32_t Application::UploadDataToAudioBuffer(BYTE* audio_data, uint32_t bytes_to_write, uint32_t block_align)
{
	uint32_t padding{ 0 };
	uint32_t framesAvailable{ 0 };
	uint32_t bytesAvailable{ 0 };
	uint32_t bytesToWrite{ 0 };
	BYTE* physicalAudioBuffer{ nullptr };

	m_AudioClient->GetCurrentPadding(&padding);
	framesAvailable = m_AudioBufferFrameCount - padding;
	bytesAvailable = framesAvailable * block_align;
	bytesToWrite = min(bytesAvailable, bytes_to_write);
	m_AudioRenderClient->GetBuffer(framesAvailable, &physicalAudioBuffer);
	memcpy(physicalAudioBuffer, audio_data, bytesToWrite);
	m_AudioRenderClient->ReleaseBuffer(framesAvailable, 0);
	return bytesToWrite;
}
