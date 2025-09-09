#include "GL/glew.h"
#include "Common.h"

class BufferLayout {
private:

	std::vector<LayoutInfo> m_LayoutInfo;
public:
	BufferLayout(const std::initializer_list<LayoutInfo>& layout);
	~BufferLayout();

	const std::vector<LayoutInfo>& GetBufferLayoutInfo() const
	{
		return m_LayoutInfo;
	}
public:
	static GLsizei GetLayoutStride(const BufferLayout& layout);
	static GLint GetLayoutInfoElementCount(const LayoutInfo& layout_info);
	static GLenum GetLayoutInfoElementSize(const LayoutInfo& layout_info);
	static GLuint GetLayoutInfoOffset(const BufferLayout& buffer_layout, const LayoutInfo& layout_info);
};