#include "Label.h"
#include <vector>

void Label::Format(const std::vector<int>& glyphMap)
{
	std::vector<int> currentLine;
	std::vector<int> currentWord;

	int availableWidth = m_Width;

	auto iter = glyphMap.begin();
	while (iter != glyphMap.end())
	{
		auto glyphIndex = *iter;
		while (!(glyphIndex == m_Separator)) // Extract all glyph indices until the separator
		{
			currentWord.push_back(glyphIndex);
		}// Now we have whole word
		while(currentWord.size() > availableWidth)
		{
			// Handle the word that exceeds the width of the label
			std::vector<int> temp;
			int index = 0;
			for (auto erase_iter = currentWord.begin(); index < availableWidth; erase_iter++)
			{
				temp.push_back(currentWord[index]);
				currentWord.erase(erase_iter);
				index++;
			}
			if (!(temp.empty()))
			{
				m_FormattedIndices.push_back(temp);
				currentWord.shrink_to_fit();
			}

			// Check if adding the current word exceeds the available width for the current line
			if (currentLine.size() + currentWord.size() + 1 > availableWidth)
			{
				m_FormattedIndices.push_back(currentLine);
				currentLine.clear();
				currentLine = currentWord;
			}
			else
			{
				if (!currentLine.empty())
				{
					currentLine.push_back(m_Separator);
				}
				for (auto insert_iter = currentWord.begin(); insert_iter != currentWord.end(); insert_iter++)
				{
					currentLine.push_back(*insert_iter);
				}
			}
		}


	}

	if (!currentLine.empty())
	{
		m_FormattedIndices.push_back(currentLine);
	}
}

const std::string& Label::GetText() const
{

	std::vector<int> temp{ 0, 1, 3, 3, 4, 5,6, 7 };
	auto iter = temp.begin();
	
	for (auto iter = temp.begin(); iter != temp.end(); iter++)
	{


	}

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
