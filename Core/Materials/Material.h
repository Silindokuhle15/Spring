#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#include "Common.h"
#include "ShaderResource.h"

struct MTLMaterial
{
public:
    glm::vec4 KaNs;
    glm::vec4 KdNi;
    glm::vec4 KsD;
    glm::vec4 KeIllum;

    MTLMaterial(
        glm::vec4 ka_ns = glm::vec4(0), 
        glm::vec4 kd_ni = glm::vec4(0), 
        glm::vec4 ks_d = glm::vec4(0), 
        glm::vec4 ke_illum = glm::vec4(0)
    )
        :
        KaNs{ ka_ns },
        KdNi{ kd_ni },
        KsD{ ks_d },
        KeIllum{ ke_illum }
    {
    }
};

class Material {
private:
    std::vector<LayoutInfo> m_Uniforms;
public:
    std::map<const std::string, glm::vec4> m_Uniforms4f;
    std::map<const std::string, glm::vec3> m_Uniforms3f;
    std::map<const std::string, glm::vec2> m_Uniforms2f;
    std::map<const std::string, glm::vec1> m_Uniforms1f;

public:
    Material(const std::initializer_list<LayoutInfo>& uniforms);
    ~Material();
    void OnInit();
    void OnUpdate();
    void AddNewUniform(const LayoutInfo& uniform);
    /*
    void SetUniformI(const std::string& uniform_name, int value) const;
    void SetUniform2I(const std::string& uniform_name, int i, int j) const;
    void SetUniform3I(const std::string& uniform_name, int i, int j, int k) const;
    void SetUniform4I(const std::string& uniform_name, int i, int j, int k, int l) const;
    void SetUniformF(const std::string& uniform_name, float value) const;
    void SetUniform2F(const std::string& uniform_name, float x, float y) const;
    void SetUniform3F(const std::string& uniform_name, float x, float y, float z) const;
    void SetUniform4F(const std::string& uniform_name, float x, float y, float z, float w) const;
    void SetUniform2FM(const std::string& uniform_name, float* matrix2) const;
    void SetUniform3FM(const std::string& uniform_name, float* matrix3) const;
    void SetUniform4FM(const std::string& uniform_name, float* matrix4) const;
    */
    Material() : 
        m_Uniforms{},
        m_Uniforms1f{},
        m_Uniforms2f{},
        m_Uniforms3f{},
        m_Uniforms4f{}
    {
    }
};

#endif