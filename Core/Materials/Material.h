#pragma once
#include "Common.h"
#include "ShaderResource.h"

class Material {
private:
    uint64_t m_MaterialID;
    std::vector<LayoutInfo> m_Uniforms;

public:
    std::map<const std::string, glm::vec4> m_Uniforms4f;
    std::map<const std::string, glm::vec3> m_Uniforms3f;
    std::map<const std::string, glm::vec2> m_Uniforms2f;
    std::map<const std::string, glm::vec1> m_Uniforms1f;

public:
    Material(const std::initializer_list<LayoutInfo>& uniforms);
    Material(const ShaderResource& resource);
    ~Material();
    void OnInit();
    void OnUpdate();
    void AddNewUniform(const LayoutInfo& uniform);
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

    Material(uint64_t id = 0xFFFFFFFF) : 
        m_MaterialID(id),
        m_Uniforms{},
        m_Uniforms1f{},
        m_Uniforms2f{},
        m_Uniforms3f{},
        m_Uniforms4f{}
    {
    }
};