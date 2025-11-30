#include "Application.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "WAV.h"

Application::Application():
	m_IMMDeviceEnumerator{nullptr},
	m_IMMDevice{nullptr},
	m_AudioClient{ nullptr},
	m_AudioRenderClient{nullptr}
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
}

Application::~Application()
{
	m_IMMDevice->Release();
	m_IMMDeviceEnumerator->Release();
	CoUninitialize();
}

int Application::RenderAudioData(const std::string& wave_file_path)
{
	std::ifstream ifs(wave_file_path.c_str(), std::ios::binary);
	if (!ifs) return -1;

	WAVHEADER header{};
	ZeroMemory(&header, sizeof(WAVHEADER));
	ifs.read((char*)&header, sizeof(WAVHEADER));
	if (strncmp(header.RIFF, "RIFF", 4) != 0 ||
		strncmp(header.WAVE, "WAVE", 4) != 0 ||
		header.audioFormat != 1) return -1;

	BYTE* audioData = new BYTE[header.dataSize];
	ifs.read((char*)audioData, header.dataSize);

	WAVEFORMATEX m_WAVFormat{};

	m_WAVFormat.wFormatTag = WAVE_FORMAT_PCM;
	m_WAVFormat.nChannels = header.numChannels;
	m_WAVFormat.nSamplesPerSec = header.sampleRate;
	m_WAVFormat.wBitsPerSample = header.bitsPerSample;
	m_WAVFormat.nBlockAlign = header.blockAlign;
	m_WAVFormat.nAvgBytesPerSec = header.byteRate;
	m_WAVFormat.cbSize = 0;

	DWORD streamFlags{ 0 };
	REFERENCE_TIME bufferDuration{ 0 };
	REFERENCE_TIME periodicity{ 0 };
	HRESULT hr;
	WAVEFORMATEX* mixFormat = nullptr;

	hr = m_AudioClient->GetMixFormat(&mixFormat);
	if (FAILED(hr)) {
		std::cout << "GetMixFormat failed: " << std::hex << hr << "\n";
		return -1;
	}

	hr = m_AudioClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		0,
		0,
		0,
		&m_WAVFormat,
		nullptr);
	if (FAILED(hr)) {
		std::cout << "Initialize failed: " << std::hex << hr << "\n";
		return -1;
	}

	uint32_t m_AudioBufferFrameCount{ 0 };
	hr = m_AudioClient->GetBufferSize(&m_AudioBufferFrameCount);
	if (FAILED(hr)) {
		std::cout << "GetBufferSize failed: " << std::hex << hr << "\n";
		return -1;
	}

	hr = m_AudioClient->GetService(IID_PPV_ARGS(&m_AudioRenderClient));
	if (FAILED(hr)) {
		std::cout << "GetService(IAudioRenderClient) failed: " << std::hex << hr << "\n";
		return -1;
	}

	m_AudioClient->Start();

	uint32_t framesWritten{ 0 };
	uint32_t totalFrames{ header.dataSize / header.blockAlign };
	uint32_t remainingBytes = header.dataSize;
	BYTE* audioPtr = audioData;
	while (remainingBytes > 0)
	{
		uint32_t padding{ 0 };
		m_AudioClient->GetCurrentPadding(&padding);
		uint32_t framesAvailable = m_AudioBufferFrameCount - padding;
		uint32_t bytesAvailable = framesAvailable * header.blockAlign;
		uint32_t bytesToWrite = min(bytesAvailable, remainingBytes);
		BYTE* audioBuffer = nullptr;
		m_AudioRenderClient->GetBuffer(framesAvailable, &audioBuffer);
		memcpy(audioBuffer, audioPtr, bytesToWrite);
		m_AudioRenderClient->ReleaseBuffer(framesAvailable, 0);
		audioPtr += bytesToWrite;
		remainingBytes -= bytesToWrite;
		Sleep(2);
	}
	m_AudioClient->Stop();

	delete[] audioData;
	m_AudioRenderClient->Release();
	m_AudioClient->Release();
	return 0;
}
