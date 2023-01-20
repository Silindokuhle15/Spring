#pragma once
#include "Camera.h"
class PerspectiveCamera :
    public Camera
{
public:
	glm::vec3 m_eye;
	glm::vec3 m_center;
	glm::vec3 m_up;

	glm::mat4 V;
	glm::mat4 VP;

	glm::mat4 m_Proj;
	glm::mat4 m_View;

	int m_Width;
	int m_Height;
	float m_AspectRatio;

	float speed = 1.0;
    PerspectiveCamera() : Camera(), 
		m_eye{ glm::vec3(0.0, .0, 0.0f) },
		m_center{ glm::vec3(0.0f) },
		m_up{ glm::vec3(0.0f, 1.0f, 0.0f) }
	{}

	glm::mat4 GetVP() const { return VP; }
	void OnResize(int new_width, int new_height);
    void OnCreate();
	void Present();

	void SetWidth(int width) { m_Width = width; }
	void SetHeight(int height) { m_Height = height; }
	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }

	void MoveForward() { m_eye += glm::normalize(glm::vec3(.0f, .0f, -1.0f)); }
	void MoveBackward() { m_eye -= glm::normalize(glm::vec3(.0f, .0f, -1.0f)); }

	void MoveRight() { m_eye += glm::normalize(glm::vec3(1.0f, .0f, .0f)); }
	void MoveLeft() { m_eye -= glm::normalize(glm::vec3(1.0f, .0f, 0.0f)); }

	void MoveUp() { m_eye += glm::normalize(glm::vec3(.0f, 1.0f, 0.0f)); }
	void MoveDown() { m_eye -= glm::normalize(glm::vec3(.0f, 1.0f, 0.0f)); }

	void Focus(glm::vec3 move_dir) { m_eye += move_dir; }

	void Reset() override;

	glm::vec3& GetPosition() override;
	void OnUpdate(float ts) override;
};

