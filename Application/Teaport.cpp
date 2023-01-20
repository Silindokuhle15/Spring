#include "Teaport.h"

void Teaport::OnInit()
{
    m_VAO.OnInit();
    m_VAO.Bind();

    ObjectLoader ob;
    ob.LoadObjectFromFile("Objects/Crate/Crate1.obj");
    //ob.LoadObjectFromFile("Objects/Uta_teaport.obj");

    m_Positions = ob.m_Positions;
    m_TexCoords = ob.m_TexCoords;
    m_VertexIndices = ob.m_VertexIndices;

    m_Positions.shrink_to_fit();
    m_VertexIndices.shrink_to_fit();

    std::vector<VertexAttrib> attribs;
    attribs.push_back(VertexAttrib::Position);
    attribs.push_back(VertexAttrib::TexCoord);
    m_VAO.CreateVertexArrayLayout(m_Shader.GetShaderProgram(), attribs);

    Base::m_IndexCount = m_VertexIndices.size();
}