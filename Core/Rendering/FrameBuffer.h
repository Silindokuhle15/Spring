#include "GL/glew.h"

class FrameBuffer {

private:
	GLuint m_Handle;
public:
	FrameBuffer();
	~FrameBuffer();
	void Bind() const;
	void SetTexture(GLenum attachement, GLuint handle, GLint level) const;
};