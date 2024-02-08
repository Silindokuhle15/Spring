#pragma once
#include <stdint.h>
#include "GL/glew.h"
#include "GL/wglew.h"

class OpenGLFrameBuffer
{
private:

	uint32_t m_Flags;
	uint32_t m_BufferData;
	uint32_t m_FBO_target;
	uint32_t m_Format;
	uint32_t m_Height;
	uint32_t m_Width;
	uint32_t m_Type;
	uint32_t m_FrameBuffer;
	uint32_t m_RenderBuffers[2];

public:

	void OnCreate();
	void Bind();

	void SetWidth(unsigned int width) noexcept { m_Width = width; }
	void SetHeight(unsigned int height) noexcept { m_Height = height; }

	unsigned int GetWidth() const { return m_Width; }
	unsigned int GetHeight() const { return m_Height; }
	unsigned int GetFrameBuffer() const { return m_FrameBuffer; }
};

