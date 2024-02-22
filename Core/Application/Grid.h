#pragma once
#include "Mesh.h"

class Grid : public Mesh
{
public:
    unsigned int m_Rows = 0, m_Columns = 0;

    Grid(unsigned int size) : m_Rows{ size }, m_Columns{ size } {}
    Grid(unsigned int rows, unsigned int cols) : m_Rows{ rows}, m_Columns{ cols } {}

    void OnInit() override;

private:


};