#pragma once
#include <string>

typedef enum 
{
	LEFT,
	CENTER,
	RIGHT

}Alignment;

class Label
{
public:
	Label(const std::string& text, int64_t x, int64_t y, uint64_t width, uint64_t height,uint32_t borderWidth, const Alignment& alignment)
		: m_Text{ text }, m_PosX{x}, m_PosY{y}, m_Width{width}, m_Height{height}, m_BorderWidth{borderWidth}, m_Alignment{alignment}
	{

	}

	Label(const std::vector<int> glyphMap, int64_t separator, int64_t x, int64_t y, uint64_t width, uint64_t height, uint32_t borderWidth, const Alignment& alignment)
		:m_Separator{separator}, m_PosX{ x }, m_PosY{ y }, m_Width{ width }, m_Height{ height }, m_BorderWidth{ borderWidth }, m_Alignment{ alignment }
	{
		Format(glyphMap);
	}
	void Format(const std::vector<int>& glyphMap);

	const std::string& GetText() const;
	void SetText(const std::string& text);

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
	std::vector<std::vector<int>> m_FormattedIndices;
	std::string m_Text;
	int64_t m_Separator;
	int64_t m_PosX;
	int64_t m_PosY;
	uint64_t m_Width;
	uint64_t m_Height;
	uint64_t m_BorderWidth;
	Alignment m_Alignment;

};