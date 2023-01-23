#include "Tank.h"

void Tank::OnInit()
{
	ObjectLoader ob;
	ob.LoadObjectFromFile("Objects/Tank/mytank.obj");

    m_Positions = ob.m_Positions;
    m_TexCoords = ob.m_TexCoords;

    ob.m_VertexIndices.erase(ob.m_VertexIndices.begin());
    ob.m_TextureIndices.erase(ob.m_TextureIndices.begin());
    m_VertexIndices = ob.m_VertexIndices;
    m_TextureIndices = ob.m_TextureIndices;

    m_Positions.shrink_to_fit();
    m_VertexIndices.shrink_to_fit();
    m_TextureIndices.shrink_to_fit();

    Base::m_IndexCount = m_VertexIndices.size();
}
