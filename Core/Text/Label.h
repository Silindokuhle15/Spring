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

class Label
{
public:
	
	Label(const std::vector<int>& glyphMap, int64_t separator, int64_t x, int64_t y, uint64_t width, uint64_t height, uint32_t borderWidth, const Alignment& alignment)
		:m_Separator{separator}, m_PosX{ x }, m_PosY{ y }, m_Width{ width }, m_Height{ height }, m_BorderWidth{ borderWidth }, m_Alignment{ alignment }
	{
		Format(glyphMap);
		GenerateGlyphBoxes();
	}

	const std::vector<Square>& GetGlyphBoxes() const;
	void Format(const std::vector<int>& glyphMap);

	const std::vector<int>& GetGlyphArray() const;
	void SetGlyphArray(const std::vector<int>& glyphs);

	const int64_t GetX() const;
	void SetX(const int64_t& x);

	const int64_t GetY() const;
	void SetY(const int64_t& y);

	const uint64_t GetWidth() const;
	void SetWidth(const uint64_t& width);

	const uint64_t GetHeight() const;
	void SetHeight(const uint64_t& height);

	const uint64_t GetBorderWidth() const;
	void SetBorderWidth(const uint64_t& borderWidth);

	const Alignment GetAlignment() const;
	void SetAlignment(const Alignment& alignment);

private:
	void GenerateGlyphBoxes();

private:
	std::vector<Square> m_GlyphBoxes;
	std::vector<std::vector<int>> m_FormattedIndices;
	std::vector<int> m_GlyphArray;
	int64_t m_Separator;
	int64_t m_PosX;
	int64_t m_PosY;
	uint64_t m_Width;
	uint64_t m_Height;
	uint64_t m_BorderWidth;
	Alignment m_Alignment;

};