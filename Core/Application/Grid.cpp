#include "Grid.h"

void Grid::OnInit()
{
    unsigned int GRID_SIZE = m_Rows;
    float scale_factor = 2.0f;
    for (int j = 0; j < GRID_SIZE; j++)
    {
        for (int i = 0; i < GRID_SIZE; i++)
        {
            glm::mat4 transform(1.0f);
            //transform = glm::scale(transform, glm::vec3(1.0f / GRID_SIZE));
            transform = glm::translate(transform, glm::vec3( 2*i - GRID_SIZE, 2 * j - GRID_SIZE, 0));
            Square sq;
            sq.OnInit();
            sq.SetTransform(transform);
            m_Cells.push_back(sq);
        }
    }
}
