#include "Moon.h"

void Moon::OnInit()
{
    ObjectLoader ob;
    ob.LoadObjectFromFile("Objects/moon/21414_Crescent_Moon_v1.obj");

    m_Positions = ob.m_Positions;
    m_TexCoords = ob.m_TexCoords;
    ob.m_VertexIndices.erase(ob.m_VertexIndices.begin());
    m_VertexIndices = ob.m_VertexIndices;

    m_Positions.shrink_to_fit();
    m_VertexIndices.shrink_to_fit();

    Base::m_IndexCount = m_VertexIndices.size();
}
