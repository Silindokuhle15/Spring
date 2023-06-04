#include "Grid.h"


void Grid::OnInit()
{
    m_VAO.OnInit();
    m_VAO.Bind();

	unsigned int index = 0;

	for (unsigned int i = 0; i < m_Rows; i++)
	{
        float fi = (float)i / m_Rows;

		for (unsigned int j = 0; j < m_Columns; j++)
        {
            float fj = (float)j / m_Columns;

            m_Positions.push_back(glm::vec3((fi - 0.5f)*(float)m_Rows, (fj - 0.5f)*(float)m_Columns, 1.0f));
			m_VertexIndices.push_back(index++);
		}
	}       
  
    m_IndexCount = m_VertexIndices.size();
}
