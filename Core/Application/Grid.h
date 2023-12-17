#pragma once
#include "Square.h"

class Grid : public Mesh
{
public:
    unsigned int m_Rows = 0, m_Columns = 0;

    Grid(unsigned int size) : m_Rows{ size }, m_Columns{ size } {}

    void OnInit();

    std::vector<Mesh> m_Cells;

private:


};