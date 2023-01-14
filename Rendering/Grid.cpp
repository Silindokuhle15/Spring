#include "Grid.h"
#include "GL/glew.h"


void Grid::OnInit()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
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

    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_Positions.size(), NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * m_Positions.size(), m_Positions.data());

    glGenBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_VertexIndices.size(), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * m_VertexIndices.size(), m_VertexIndices.data());

    //create_shaders();
    m_Shader.m_Info[0] = this->m_Info[0];
    m_Shader.m_Info[1] = this->m_Info[1];
    m_Shader.OnInit();

    m_VPOSlocation = glGetAttribLocation(m_Shader.GetShaderProgram(), "Position");

    m_MVPlocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "MVP");
    m_MVlocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "MV");

    glVertexAttribPointer(m_VPOSlocation, 3, GL_FLOAT, GL_FALSE,
        sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(m_VPOSlocation);

    m_IndexCount = m_VertexIndices.size();
}


void Grid::OnUpdate()
{
    //glBindVertexArray(m_VAO);
    glUseProgram(m_Shader.GetShaderProgram());
    glUniformMatrix4fv(m_MVPlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetMVP()));
    glUniformMatrix4fv(m_MVlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetMV()));
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
