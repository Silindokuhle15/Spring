#include "FrameBuffer.h"

void FrameBuffer::OnCreate()
{
	glGenFramebuffers(1, &m_FrameBuffer);
	glFramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, m_Width);
	glFramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, m_Height);

	glGenRenderbuffers(2, m_RenderBuffers);

	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffers[0]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, m_Width, m_Height);

	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffers[1]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, m_Width, m_Height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_RenderBuffers[0]);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffers[1]);
}
void FrameBuffer::Bind()
{
	glBindBuffer(GL_FRAMEBUFFER, m_FrameBuffer);
	glViewport(0, 0, m_Width, m_Height);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_BUFFER_BIT);
	glDepthRange(.010f, 1000.f);
}