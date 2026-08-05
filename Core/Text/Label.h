#pragma once
#include <string>
#include <vector>
#include "Square.h"

typedef enum 
{
	LEFT,
	CENTER,
	RIGHT

}Alignment;

class GlyphBox
{
public:
	GlyphBox() :
		m_Transform{glm::mat4(1.0f)}
	{
	}
	
	void SetGlyphID(float id);
	void SetTransform(const glm::mat4& transform);

	glm::mat4 m_Transform;
	primitives::Square m_Box;
};

class Label
{
public:
	
	Label(const std::vector<int>& glyphMap, int32_t separator, int32_t x, int32_t y, uint32_t width, uint32_t height, uint32_t borderWidth, const Alignment& alignment)
		:m_Separator{separator}, m_PosX{ x }, m_PosY{ y }, m_Width{ width }, m_Height{ height }, m_BorderWidth{ borderWidth }, m_Alignment{ alignment }
	{
		Format(glyphMap);
		GenerateGlyphBoxes();
	}

	const std::vector<GlyphBox>& GetGlyphBoxes() const;
	void Format(const std::vector<int>& glyphMap);

	const std::vector<int>& GetGlyphArray() const;
	void SetGlyphArray(const std::vector<int>& glyphs);

	const int32_t GetX() const;
	void SetX(const int64_t& x);

	const int32_t GetY() const;
	void SetY(const int64_t& y);

	const uint32_t GetWidth() const;
	void SetWidth(const uint32_t& width);

	const uint32_t GetHeight() const;
	void SetHeight(const uint32_t& height);

	const uint32_t GetBorderWidth() const;
	void SetBorderWidth(const uint32_t& borderWidth);

	const Alignment GetAlignment() const;
	void SetAlignment(const Alignment& alignment);

private:
	void GenerateGlyphBoxes();

private:
	int32_t m_PosX;
	int32_t m_PosY;
	uint32_t m_Width;
	uint32_t m_Height;
	int32_t m_Separator;
	uint32_t m_BorderWidth;
	Alignment m_Alignment;
	uint32_t m_Flags;
	std::vector<GlyphBox> m_GlyphBoxes;
	std::vector<std::vector<int>> m_FormattedIndices;
	std::vector<int> m_GlyphArray;
};