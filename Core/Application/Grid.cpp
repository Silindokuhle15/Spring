#include "Grid.h"

void Grid::OnInit()
{
    double SQR_SIZE = 1.0f;
    double padding = 1.0;
    double GRID_SIZE = 2.0f;
    float scale_factor = 2.0f;
    for (int j = 0; j < m_Columns; j++)
    {
        for (int i = 0; i < m_Rows; i++)
        {
            glm::mat4 transform(1.0f);
            //transform = glm::scale(transform, glm::vec3(1.0f / GRID_SIZE, 1.0f / GRID_SIZE,0));
            transform = glm::translate(transform, glm::vec3( i * SQR_SIZE ,  j * SQR_SIZE , 0));
            Square sq;
            sq.OnInit();
            sq.SetTransform(transform);
            m_Cells.push_back(sq);
        }
    }
}
