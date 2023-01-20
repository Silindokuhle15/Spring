#include "Cube.h"

void Cube::OnInit()
{
    m_VAO.OnInit();
    m_VAO.Bind();

    ob.LoadObject("Objects/Cube.obj");
    ob.m_VertexIndices.shrink_to_fit();

    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    
    size_t size = sizeof(struct PerVertex) * ob.m_VertexIndices.size();
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
        
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * ob.m_Positions.size(), ob.m_Positions.data());

    size = sizeof(glm::vec3) * ob.m_Positions.size();
    glBufferSubData(GL_ARRAY_BUFFER, size, sizeof(glm::vec2) * ob.m_TexCoords.size(), ob.m_TexCoords.data());

    size = sizeof(glm::vec3) * ob.m_Positions.size() + sizeof(glm::vec2) * ob.m_TexCoords.size();
    glBufferSubData(GL_ARRAY_BUFFER, size, sizeof(glm::vec3) * ob.m_Normals.size(), ob.m_Normals.data());

    glGenBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ob.m_VertexIndices.size(), ob.m_VertexIndices.data(), GL_DYNAMIC_DRAW);

    //create_shaders();
    m_Shader.m_Info[0] = this->m_Info[0];
    m_Shader.m_Info[1] = this->m_Info[1];
    m_Shader.OnInit(0);

    std::vector<VertexAttrib> attribs = { VertexAttrib::Position, VertexAttrib::Normal };
    m_VAO.CreateVertexArrayLayout(m_Shader.GetShaderProgram(), attribs);
    m_ModelLocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "Model");
    

    m_PointLightLocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "LightPosition");
    m_AmbientColorLocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "AmbientColor");

    m_IndexCount = ob.m_VertexIndices.size();
}

void Cube::OnUpdate()
{
    glUseProgram(m_Shader.GetShaderProgram());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(m_ModelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    glUniform3fv(m_PointLightLocation, 1, glm::value_ptr(BaseApplication::m_PointLight->GetPosition()));
    glUniform4fv(m_AmbientColorLocation, 1, AmbientColor);
}

void Cube::OnRender()
{
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    ob.m_VertexIndices.shrink_to_fit();

    glDrawElements(GL_TRIANGLE_STRIP, ob.m_VertexIndices.size(), GL_UNSIGNED_INT, NULL);
    ImGui::Begin("Cube");
    ImGui::ColorEdit4("Ambient Color", AmbientColor);
    ImGui::End();
}

void Cube::OnDestroy(){ }

void Cube::EnableTesselation()
{
    float patch_inner[2] = { 5.0, 5.0 };
    float patch_outer[4] = { 6.0, 5.0, 8.0, 5.0 };
    glPatchParameteri(GL_PATCH_VERTICES,36);
    glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, patch_inner);
    glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, patch_outer);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
}