#include "FrameBuffer.h"

FrameBuffer::FrameBuffer():
	m_Handle{0}
{
	glCreateFramebuffers(1, &m_Handle);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_Handle);
}
void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
}

void FrameBuffer::SetTexture(GLenum attachment, GLuint handle, GLint level) const
{
	glNamedFramebufferTexture(m_Handle, attachment, handle, level);
}
