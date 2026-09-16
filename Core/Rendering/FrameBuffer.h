#pragma once
#include "GL/glew.h"

class FrameBuffer {

private:
	GLuint m_Handle;
public:
	FrameBuffer();
	~FrameBuffer();
	FrameBuffer(const FrameBuffer& other) = delete;
	FrameBuffer& operator=(const FrameBuffer& other ) = delete;
	void Bind() const;
	void SetTexture(GLenum attachement, GLuint handle, GLint level) const;
};