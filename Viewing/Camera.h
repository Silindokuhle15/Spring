#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

template<class T>
class Camera
{
public:
	glm::vec3 m_eye;
	glm::vec3 m_center;
	glm::vec3 m_up;

	glm::mat4 V;
	glm::mat4 VP;
	glm::mat4 m_View;
	glm::mat4 m_Proj;

	int m_Width;
	int m_Height;
	float m_AspectRatio;
		
	float speed = 200.0;
	float m_Delta;

	virtual glm::vec3 GetPosition() { return m_eye; }
	virtual glm::mat4 GetV() { return V; }
	virtual glm::mat4 GetP() { return m_Proj; }
	virtual glm::mat4 GetVP() { return VP; }

	virtual void SetWidth(int width) { m_Width = width; }
	virtual void SetHeight(int height) { m_Height = height; }
	virtual int GetWidth() const { return m_Width; }
	virtual int GetHeight() const { return m_Height; }


	virtual void OnInit() = 0;
	virtual void Present() = 0;


	virtual void MoveForward() = 0; 
	virtual void MoveBackward() = 0;
	virtual void MoveRight() = 0; 
	virtual void MoveLeft() = 0;
	virtual void MoveUp() = 0;
	virtual void MoveDown() = 0;

	virtual void Rotate(glm::vec3 rot_dir) = 0;
	virtual void Focus(glm::vec3 rot_dir) = 0;

	virtual void OnUpdate(float delta_time) = 0;
};

