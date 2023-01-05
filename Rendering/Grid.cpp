#include "Grid.h"
#include "GL/glew.h"

void Grid::OnInit()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
	unsigned int index = 0;
    
	for (unsigned int i = 0; i < m_Rows; i++)
	{
        float x = i - m_Rows/2;

		for (unsigned int j = 0; j < m_Columns; j++)
		{
			float y = j - m_Columns/2;
            m_Positions.push_back(glm::vec3(x, y, 0));
            //m_Positions.push_back(glm::vec3(y, -x, 0));
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
    m_VShader.m_Info[0] = this->m_Info[0];
    m_VShader.m_Info[1] = this->m_Info[1];
    m_VShader.OnInit();

    m_VPOSlocation = glGetAttribLocation(m_VShader.GetShaderProgram(), "Position");

    glVertexAttribPointer(m_VPOSlocation, 3, GL_FLOAT, GL_FALSE,
        sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(m_VPOSlocation);
}


void Grid::OnUpdate()
{
    glUseProgram(m_VShader.GetShaderProgram());
    m_MVPlocation = glGetUniformLocation(m_VShader.GetShaderProgram(), "MVP");
    m_MVlocation = glGetUniformLocation(m_VShader.GetShaderProgram(), "MV");

    //m_PointLightLocation = glGetUniformLocation(m_Program, "LightPosition");

    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(m_MVPlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetMVP()));
    glUniformMatrix4fv(m_MVlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetMV()));
    //glUniform3fv(m_PointLightLocation, 1, glm::value_ptr(BaseApplication::m_PointLight->GetPosition()));
}

void Grid::OnRender()
{
    glUseProgram(m_VShader.GetShaderProgram());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

    glPointSize(3.0);
    //glDrawElements(GL_POINTS, m_VertexIndices.size(), GL_UNSIGNED_INT, m_VertexIndices.data());
    glDrawArrays(GL_POINTS, 0, m_VertexIndices.size());
}

void Grid::OnDestroy()
{
}
