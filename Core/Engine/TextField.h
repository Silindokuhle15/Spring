#pragma once
#include "FontMgr.h"
#include "Square.h"

class TextField
{
public:
	std::vector<Square> m_GlyphBoxes;

	TextField(std::shared_ptr<FontMgr> fontMgr, std::string text="")
		:
		m_pFontMgr{fontMgr},
		m_Text{text},
		m_Width{8},
		m_Height{8}
	{
		for(size_t index=0; index< m_Text.size(); index++)
		{
			auto& c = m_Text[index];
			auto glyphIndex = m_pFontMgr->FindGlyphIndexFromChar(c);
			double y_offset = 0.0;
			double x_offset = index;
			
			Square sq;
			sq.OnInit();
			if (index > m_Width)
			{
				y_offset = -3;
				x_offset = static_cast<float>(index) - int(m_Width);
			}
			auto transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.09*x_offset, 0.09 * y_offset, 0.10f));
			transform = glm::scale(transform, glm::vec3(0.09f));
			sq.SetTransform(transform);
			sq.SetColor(glm::vec3(1.0f));
			for (auto& v : sq.m_V)
			{
				v.ID = static_cast<float>(glyphIndex.getIndex());
			}

			m_GlyphBoxes.push_back(sq);
		}
	}
	~TextField()
	{

	}

private:
	std::string m_Text;
	float m_Width;
	float m_Height;
	std::shared_ptr<FontMgr> m_pFontMgr;
};