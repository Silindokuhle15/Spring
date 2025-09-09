#include "BufferLayout.h"

BufferLayout::BufferLayout(const std::initializer_list<LayoutInfo>& layout) :
	m_LayoutInfo{ layout }
{
}

BufferLayout::~BufferLayout()
{
}

GLsizei BufferLayout::GetLayoutStride(const BufferLayout& layout)
{
	GLuint stride{ 0 };
	for (auto& element : layout.GetBufferLayoutInfo())
	{
		stride += sizeof(float) * BufferLayout::GetLayoutInfoElementCount(element);
	}
	return stride;
}

GLint BufferLayout::GetLayoutInfoElementCount(const LayoutInfo& layout_info)
{
	switch (layout_info.Type)
	{
	case ShaderDataType::Bool:		return 1;
	case ShaderDataType::Float:		return 1;
	case ShaderDataType::Float2:	return 2;
	case ShaderDataType::Float3:	return 3;
	case ShaderDataType::Float4:	return 4;
	case ShaderDataType::Mat2:		return 2 * 2;
	case ShaderDataType::Mat3:		return 3 * 3;
	case ShaderDataType::Mat4:		return 4 * 4;
	}
	return 0;
}

GLenum BufferLayout::GetLayoutInfoElementSize(const LayoutInfo& layout_info)
{
	switch (layout_info.Type)
	{
	case ShaderDataType::Bool:		return GL_BOOL;
	case ShaderDataType::Float:		return GL_FLOAT;
	case ShaderDataType::Float2:	return GL_FLOAT;
	case ShaderDataType::Float3:	return GL_FLOAT;
	case ShaderDataType::Float4:	return GL_FLOAT;
	case ShaderDataType::Mat2:		return GL_FLOAT;
	case ShaderDataType::Mat3:		return GL_FLOAT;
	case ShaderDataType::Mat4:		return GL_FLOAT;
	}
	return 0;
}

GLuint BufferLayout::GetLayoutInfoOffset(const BufferLayout& buffer_layout, const LayoutInfo& layout_info)
{
	GLuint offset{ 0 };
	for (auto& layoutInfo : buffer_layout.GetBufferLayoutInfo())
	{
		if (layoutInfo.Name == layout_info.Name)
		{
			return offset;
		}
		offset += sizeof(float) * BufferLayout::GetLayoutInfoElementCount(layoutInfo);
	}
	return offset;
}
