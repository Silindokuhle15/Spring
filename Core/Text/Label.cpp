#include "Label.h"
#include <vector>

const std::vector<GlyphBox>& Label::GetGlyphBoxes() const
{
	return m_GlyphBoxes;
}

void Label::Format(const std::vector<int>& glyphMap)
{
	std::vector<int> currentLine;
	std::vector<int> currentWord;

	int availableWidth = static_cast<int>(m_Width);

	auto iter = glyphMap.begin();
	while (iter != glyphMap.end())
	{
		while (!(*iter == m_Separator)) // Extract all glyph indices until the separator
		{
			currentWord.push_back(*iter);
			iter++;
			if (iter == glyphMap.end())
			{
				break;
			}
		}// Now we have whole word
		while (currentWord.size() > availableWidth)
		{
			// Handle the word that exceeds the width of the label
			std::vector<int> temp;
			std::vector<int> remainder;
			for (size_t index = 0; index < availableWidth; index++)
			{
				temp.push_back(currentWord[index]);
			}
			int index = 0;
			for (size_t index = availableWidth; index < currentWord.size(); index++)
			{
				remainder.push_back(currentWord[index]);
			}
			currentWord = remainder;
			currentWord.shrink_to_fit();
			if (!(temp.empty()))
			{
				m_FormattedIndices.push_back(temp);
			}
			temp.clear();
			remainder.clear();
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
				currentLine.push_back(static_cast<int>(m_Separator));
			}
			for (auto insert_iter = currentWord.begin(); insert_iter != currentWord.end(); insert_iter++)
			{
				currentLine.push_back(*insert_iter);
			}
			currentWord.clear();
			if (iter == glyphMap.end())
			{
				break;
			}
			else
			{
				iter++;
			}
		}
	}
	if (!currentLine.empty())
	{
		m_FormattedIndices.push_back(currentLine);
	}
}

const std::vector<int>& Label::GetGlyphArray() const
{
	return m_GlyphArray;
}

void Label::SetGlyphArray(const std::vector<int>& glyphs)
{
	m_GlyphArray = glyphs;
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

void Label::GenerateGlyphBoxes()
{
	double xScale = 1.0;
	double yScale = 3.0;
	double z_offset = 0.1;

	double spacingX = 0.2;
	double spacingY = 0.1;

	for (size_t row = 0; row < m_FormattedIndices.size(); row++)
	{
		for (size_t col = 0; col < m_FormattedIndices[row].size(); col++)
		{
			// Place the Sqaures
			auto glyphIndex = m_FormattedIndices[row][col];
			float id = static_cast<float>(glyphIndex);

			double x_offset = static_cast<float>(col * xScale) - 0.5* m_FormattedIndices[row].size();
			double y_offset = static_cast<float>(row * yScale);

			x_offset += (col == 0 ? 0.0 : spacingX);
			y_offset += (row == 0 ? 0.0 : spacingY);

			double x_scale = 0.1;
			double y_scale = 0.1;
			double z_scale = 0.1;

			GlyphBox glyphBox;
			
			glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(x_scale, y_scale, z_scale));
			transform = glm::translate(transform, glm::vec3(x_offset, y_offset, z_offset));

			glyphBox.SetTransform(transform);
			glyphBox.SetGlyphID(id);

			m_GlyphBoxes.push_back(glyphBox);
		}
	}
}

void GlyphBox::SetGlyphID(float id)
{
	for (auto& v : m_Box.m_V)
	{
		v.ID = id;
	}
}

void GlyphBox::SetTransform(const glm::mat4& transform)
{
	m_Transform = transform;
}
