#include "Material.h"
#include "GL/glew.h"

Material::Material(const std::initializer_list<LayoutInfo>& uniforms):
	m_Uniforms{uniforms}
{
}

Material::Material(const ShaderResource& resource)
{
	auto& shader_sources = resource.GetShaderSources();
	for (auto& shader_resource : shader_sources)
	{
		std::istringstream iss{ shader_resource };
		std::string line{};
		while (std::getline(iss, line))
		{
			auto words = getWords(line, " ");
			if (words.empty()) continue;
			if (words.front() == "uniform")
			{
				
				// Process whole line here
			}
			else {
				continue;
			}
		}
	}

}

Material::~Material()
{
}

void Material::OnInit()
{
}
void Material::OnUpdate()
{
}

void Material::AddNewUniform(const LayoutInfo& uniform)
{
	m_Uniforms.push_back(uniform);
}
/*

void Material::SetUniformI(const std::string& uniform_name, int value) const
{
	GLuint location = glGetUniformLocation(static_cast<GLuint>(m_MaterialID), uniform_name.c_str());
	glUniform1i(location, value);
}

void Material::SetUniform2I(const std::string& uniform_name, int i, int j) const
{
	GLuint location = glGetUniformLocation(static_cast<GLuint>(m_MaterialID), uniform_name.c_str());
	glUniform2i(location, i, j);
}

void Material::SetUniform3I(const std::string& uniform_name, int i, int j, int k) const
{
	GLuint location = glGetUniformLocation(static_cast<GLuint>(m_MaterialID), uniform_name.c_str());
	glUniform3i(location, i, j, k);
}

void Material::SetUniform4I(const std::string& uniform_name, int i, int j, int k, int l) const
{
	GLuint location = glGetUniformLocation(static_cast<GLuint>(m_MaterialID), uniform_name.c_str());
	glUniform4i(location, i, j, k, l);
}

void Material::SetUniformF(const std::string& uniform_name, float value) const
{
	GLuint location = glGetUniformLocation(static_cast<GLuint>(m_MaterialID), uniform_name.c_str());
	glUniform1f(location, value);
}

void Material::SetUniform2F(const std::string& uniform_name, float x, float y) const
{
	GLuint location = glGetUniformLocation(static_cast<GLuint>(m_MaterialID), uniform_name.c_str());
	glUniform2f(location, x, y);
}

void Material::SetUniform3F(const std::string& uniform_name, float x, float y, float z) const
{
	GLuint location = glGetUniformLocation(static_cast<GLuint>(m_MaterialID), uniform_name.c_str());
	glUniform3f(location, x, y, z);
}

void Material::SetUniform4F(const std::string& uniform_name, float x, float y, float z, float w) const
{
	GLuint location = glGetUniformLocation(static_cast<GLuint>(m_MaterialID), uniform_name.c_str());
	glUniform4f(location, x, y, z, w);
}

void Material::SetUniform2FM(const std::string& uniform_name, float* matrix2) const
{
	GLuint location = glGetUniformLocation(static_cast<GLuint>(m_MaterialID), uniform_name.c_str());
	glUniformMatrix2fv(location, 4, GL_FALSE, matrix2);
}

void Material::SetUniform3FM(const std::string& uniform_name, float* matrix3) const
{
	GLuint location = glGetUniformLocation(static_cast<GLuint>(m_MaterialID), uniform_name.c_str());
	glUniformMatrix3fv(location, 9, GL_FALSE, matrix3);
}

void Material::SetUniform4FM(const std::string& uniform_name, float* matrix4) const
{
	GLuint location = glGetUniformLocation(static_cast<GLuint>(m_MaterialID), uniform_name.c_str());
	glUniformMatrix4fv(location, 16, GL_FALSE, matrix4);
}
*/