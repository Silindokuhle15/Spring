#pragma once
#include "glm/glm.hpp"
#include "glm/ext.hpp"

enum class PROJECTION
{
	ORTHOGRAPHIC = 0,
	PERSPECTIVE = 1
};

class CameraView
{
public:
	glm::vec3 m_eye;
	glm::vec3 m_center;
	glm::vec3 m_up;

	//glm::mat4 V;
	//glm::mat4 VP;
	glm::mat4 m_View;
	//glm::mat4 m_Proj;

	int m_Width;
	int m_Height;
	float m_AspectRatio;

	float m_Speed = 1.0f;
	float m_Delta;

	virtual glm::vec3 GetPosition() { return m_eye; }
	virtual void SetPosition(glm::vec3 new_pos) { m_eye = new_pos; }
	//virtual glm::mat4 GetV() { return m_View; }
	virtual glm::mat4 GetV() { return m_View; }
	//
	//virtual glm::mat4 GetVP() { return m_Projection->m_ProjectionMatrix * m_View; }

	virtual void SetWidth(int width) { m_Width = width; }
	virtual void SetHeight(int height) { m_Height = height; }
	virtual int GetWidth() const { return m_Width; }
	virtual int GetHeight() const { return m_Height; }


	virtual void OnInit()
	{
		m_AspectRatio = float(m_Width) / m_Height;
		m_View = glm::lookAt(m_eye, m_center, m_up);
	}

	virtual void MoveForward() 
	{
		m_eye -= glm::vec3(0.0, 0.0, 1.0) * m_Speed * m_Delta;
	}
	virtual void MoveBackward() 
	{
		m_eye += glm::vec3(0.0, 0.0, 1.0) * m_Speed * m_Delta;
	}
	virtual void MoveRight() 
	{
		m_eye += glm::vec3(1.0, 0.0, 0.0) * m_Speed * m_Delta;
	}
	virtual void MoveLeft() 
	{
		m_eye -= glm::vec3(1.0, 0.0, 0.0) * m_Speed * m_Delta;
	}
	virtual void MoveUp() 
	{
		m_eye += glm::vec3(0.0, 1.0, 0.0) * m_Speed * m_Delta;
	}
	virtual void MoveDown() 
	{
		m_eye -= glm::vec3(0.0, 1.0, 0.0) * m_Speed * m_Delta;
	}

	//virtual void Rotate(glm::vec3 rot_dir) = 0;
	//virtual void Focus(glm::vec3 rot_dir) = 0;

	virtual void OnUpdate(float delta_time) 
	{
		m_Delta = delta_time;
	}
};

class _Projection
{
public:
	//int32_t left, right, up, down;
	glm::mat4 m_ProjectionMatrix;
	PROJECTION m_ProjectionType;

	virtual glm::mat4 GetP() const { return m_ProjectionMatrix; }

	_Projection() : m_ProjectionType{PROJECTION::PERSPECTIVE} {}
};

class Camera : public CameraView, public _Projection
{
public:
	void Present()
	{
		switch(m_ProjectionType)
		{	
		case PROJECTION::ORTHOGRAPHIC:
			m_ProjectionMatrix = glm::ortho(-m_AspectRatio, m_AspectRatio, -1.0f, 1.0f, .10f, 1000.0f);
			break;

		case PROJECTION::PERSPECTIVE:
			m_ProjectionMatrix = glm::perspective(glm::radians(60.f * 2), m_AspectRatio, 0.010f, 100.0f);
			break;
		}
		m_View = glm::lookAt(m_eye, m_center, m_up);
		//V = m_View;
		//VP = m_ProjectionMatrix * m_View;
	}
	Camera() {}
	Camera(int width, int height, float zFar, float zNear)
	{
	}
	glm::mat4 GetVP() const { return m_ProjectionMatrix * m_View; }

};
