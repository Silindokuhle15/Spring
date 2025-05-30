#pragma once
#include "glm/glm.hpp"

class Material {
public:
    uint64_t m_MaterialID;
    float m_d = 1.0f;
    float m_illum = 0.0f;
    glm::vec3 m_Ka = glm::vec3(0.0f);
    float m_Ni = 1.0f;
    glm::vec3 m_Kd = glm::vec3(0.0f);
    float m_Ns = 0.0f;
    glm::vec3 m_Ks = glm::vec3(0.0f);
    float unused_1 = 0.0f;

    void OnInit();
    void OnUpdate();

    Material(uint64_t id = 0xFFFFFFFF) : m_MaterialID(id) {}
};