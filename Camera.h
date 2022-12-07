#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Camera
{
private:
	glm::vec3 m_eye;
	glm::vec3 m_center;
	glm::vec3 m_up;

	glm::mat4 MVP;

	glm::mat4 m_ModelRotate;
	glm::mat4 m_ModelScale;
	glm::mat4 m_ModelTranslate;

	glm::mat4 m_Proj;
	glm::mat4 m_View;

	int m_Width;
	int m_Height;
	float m_AspectRatio;

public:
	Camera() :
		m_eye{ glm::vec3(0.0, .0, 5.0f) },
		m_center{ glm::vec3(0.0f) },
		m_up{ glm::vec3(0.0f, 1.0f, 0.0f) }
	{	}

	void OnCreate();

	glm::mat4 GetMVP() const { return MVP; }
	void Present();
	void OnResize(int new_width, int new_height);

	void SetWidth(int width)  { m_Width = width; }
	void SetHeight(int height)  { m_Height = height; }
	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }

	void MoveForward() { m_eye += glm::normalize(glm::vec3(.0f, .0f, -1.0f)); }
	void MoveBackward() { m_eye -= glm::normalize(glm::vec3(.0f, .0f, -1.0f)); }

	void MoveRight() { m_eye += glm::normalize(glm::vec3(1.0f, .0f, .0f)); }
	void MoveLeft(){ m_eye -= glm::normalize(glm::vec3(1.0f, .0f, 0.0f)); }

	void MoveUp(){ m_eye += glm::normalize(glm::vec3(.0f, 1.0f, 0.0f)); }
	void MoveDown(){ m_eye -= glm::normalize(glm::vec3(.0f, 1.0f, 0.0f)); }

	void Focus(glm::vec3 move_dir) { m_eye += move_dir; }
};

