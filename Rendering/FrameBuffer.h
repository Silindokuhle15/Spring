#pragma once
#include "glew.h"


class FrameBuffer
{
private:
	unsigned int m_FrameBuffer;
	unsigned int m_RenderBuffers[2];

	unsigned int m_Width;
	unsigned int m_Height;

public:

	void OnCreate();
	void Bind();

	void SetWidth(unsigned int width) noexcept { m_Width = width; }
	void SetHeight(unsigned int height) noexcept { m_Height = height; }

	unsigned int GetWidth() const { return m_Width; }
	unsigned int GetHeight() const { return m_Height; }
	unsigned int GetFrameBuffer() const { return m_FrameBuffer; }
};

