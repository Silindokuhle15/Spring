#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Camera
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
	float m_Delta;
	float speed = 1.0;

	Camera() :
		m_eye{ glm::vec3(0.0, .0, 5.0f) },
		m_center{ glm::vec3(0.0f) },
		m_up{ glm::vec3(0.0f, 1.0f, 0.0f) }
	{	}

	void OnCreate();
	glm::mat4 GetV() const { return V; }
	glm::mat4 GetVP() const { return VP; }
	void Present();
	void OnResize(int new_width, int new_height);

	void SetWidth(int width)  { m_Width = width; }
	void SetHeight(int height)  { m_Height = height; }
	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }

	void MoveForward() { m_eye += glm::normalize(glm::vec3(.0f, .0f, -1.0f)); }
	void MoveBackward() { m_eye -= glm::normalize(glm::vec3(.0f, .0f, -1.0f)); }

	void MoveRight() { m_eye += glm::normalize(glm::vec3(1.0f, .0f, .0f)); }
	void MoveLeft() { m_eye -= glm::normalize(glm::vec3(1.0f, .0f, 0.0f)); }

	void MoveUp() { m_eye += glm::normalize(glm::vec3(.0f, 1.0f, 0.0f)); }
	void MoveDown() { m_eye -= glm::normalize(glm::vec3(.0f, 1.0f, 0.0f)); }

	void Focus(glm::vec3 move_dir) { m_eye += move_dir; }
	void Rotate(glm::vec3 rot_dir) {

		glm::mat4 rot_mat = glm::mat4(1.0f);
		float rot_speed = 1.0f;
		float rot_angle = 10.0f;
		rot_mat = glm::rotate(rot_mat, glm::radians(rot_speed * rot_angle), rot_dir);
		glm::vec4 rot = rot_mat * glm::vec4(m_eye, 1.0f);

		m_eye = glm::vec3(rot);
	}

	virtual void Reset() = 0;
	virtual glm::vec3& GetPosition() = 0;

	virtual void OnUpdate(float delta_time) = 0;
};

