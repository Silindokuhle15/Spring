#include "Label.h"

const std::string& Label::GetText() const
{
	return m_Text;
}

void Label::SetText(const std::string& text)
{
	m_Text = text;
}

const int64_t Label::GetX() const
{
	return m_PosX;
}

void Label::SetX(const int64_t& x)
{
	m_PosX = x;
}

const int64_t Label::GetY() const
{
	return m_PosY;
}

void Label::SetY(const int64_t& y)
{
	m_PosY = y;
}

const uint64_t Label::GetWidth() const
{
	return m_Width;
}

void Label::SetWidth(const uint64_t& width)
{
	m_Width = width;
}

const uint64_t Label::GetHeight() const
{
	return m_Height;
}

void Label::SetHeight(const uint64_t& height)
{
	m_Height = height;
}

const uint64_t Label::GetBorderWidth() const
{
	return m_BorderWidth;
}

void Label::SetBorderWidth(const uint64_t& borderWidth)
{
	m_BorderWidth = borderWidth;
}

const Alignment Label::GetAlignment() const
{
	return m_Alignment;
}

void Label::SetAlignment(const Alignment& alignment)
{
	m_Alignment = alignment;
}
