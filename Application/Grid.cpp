#include "Grid.h"
#include "GL/glew.h"


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
    m_Positions.shrink_to_fit();
    m_VertexIndices.shrink_to_fit();

    glCreateBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glNamedBufferStorage(m_VertexBuffer, sizeof(glm::vec3) * m_Positions.size(), nullptr, GL_DYNAMIC_DRAW);
    glNamedBufferData(m_VertexBuffer, sizeof(glm::vec3) * m_Positions.size(), m_Positions.data(), GL_DYNAMIC_DRAW);

    glCreateBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glNamedBufferData(m_IndexBuffer, sizeof(unsigned int) * m_VertexIndices.size(), m_VertexIndices.data(), GL_DYNAMIC_DRAW);
    
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_VertexIndices.size(), NULL, GL_DYNAMIC_DRAW);
    //glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * m_VertexIndices.size(), m_VertexIndices.data());

    //create_shaders();
    m_Shader.m_Info[0] = this->m_Info[0];
    m_Shader.m_Info[1] = this->m_Info[1];
    m_Shader.OnInit();

    m_VAO.CreateVertexArrayLayout(m_Shader.GetShaderProgram(), VertexAttrib::Position);

    m_VPlocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "VP");
    m_Vlocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "V");
  
    m_IndexCount = m_VertexIndices.size();
    //glBindVertexArray(0);
}


void Grid::OnUpdate()
{
    //glBindVertexArray(m_VAO);
    glUseProgram(m_Shader.GetShaderProgram());
    glUniformMatrix4fv(m_VPlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetVP()));
    glUniformMatrix4fv(m_ModelLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    glUniformMatrix4fv(m_Vlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetV()));
}

void Grid::OnRender()
{   
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPointSize(3.0);
    glDrawArrays(GL_POINTS, 0, m_VertexIndices.size());
    //glDrawElements(GL_POINTS, 4, GL_UNSIGNED_INT, nullptr);
}

void Grid::OnDestroy()
{
}
