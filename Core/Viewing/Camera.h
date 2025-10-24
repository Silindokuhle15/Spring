#pragma once
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "TimeStep.h"
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
	glm::quat m_orientation;
	glm::mat4 m_View;

	int m_Width;
	int m_Height;
	float m_AspectRatio;

	float m_Speed = 1.0f;
	TimeStep m_Delta;

	virtual glm::vec3 GetCenter() const { return m_center; }
	virtual glm::vec3 GetEye() const { return m_eye; }
	virtual void SetCenter(glm::vec3 new_pos) { m_center = new_pos; }
	virtual void SetEye(glm::vec3 new_dir) { m_eye = new_dir; }
	virtual void SetUp(glm::vec3 new_dir) { m_up = new_dir; }
	virtual glm::mat4 GetV() const { return m_View; }
	
	virtual void SetWidth(int width) { m_Width = width; }
	virtual void SetHeight(int height) { m_Height = height; }
	virtual void SetOrientation(glm::quat orientation) { m_orientation = orientation; }
	virtual int GetWidth() const { return m_Width; }
	virtual int GetHeight() const { return m_Height; }
	virtual const glm::quat GetOrientation() const { return m_orientation; }

	virtual void OnInit()
	{
		m_AspectRatio = float(m_Width) / m_Height;
		m_View = glm::lookAt(m_eye, m_center, m_up);
	}

	virtual void OnUpdate(TimeStep delta_time) 
	{
		m_Delta = delta_time;
	}

	CameraView() :
		m_Width{1920},
		m_Height{1080},
		m_AspectRatio{1.0f},
		m_eye{ 0.0f, 0.0f, 0.0f },
		m_center{0.0f, 0.0f, 1.0f},
		m_up{0.0f, 1.0f, 0.0},
		m_orientation{0.0f, 0.0f, 0.0f, 1.0f},
		m_View{glm::mat4(1.0f)}
	{

	}
};

class _Projection
{
public:
	glm::mat4 m_ProjectionMatrix;
	PROJECTION m_ProjectionType;
	virtual glm::mat4 GetP() const { return m_ProjectionMatrix; }
	_Projection() : m_ProjectionMatrix{ glm::mat4(1.0f)}, m_ProjectionType { PROJECTION::PERSPECTIVE } {}
};

class Camera : public CameraView, public _Projection
{
public:
	virtual void Present()
	{
		switch(m_ProjectionType)
		{	
		case PROJECTION::ORTHOGRAPHIC:
			m_ProjectionMatrix = glm::ortho(-m_AspectRatio, m_AspectRatio, -1.0f, 1.0f, .10f, 1000.0f);
			break;

		case PROJECTION::PERSPECTIVE:
			m_ProjectionMatrix = glm::perspective(glm::radians(75.f), m_AspectRatio, 0.010f, 1000.0f);
			break;
		}
		CameraView::m_View = glm::translate(glm::mat4(1.0f), -m_eye) * glm::mat4_cast(m_orientation);
	}
	Camera() {}
	Camera(int width, int height, float zFar, float zNear)
	{
	}
	virtual glm::mat4 GetVP() const { return m_ProjectionMatrix * m_View; }
	virtual const glm::vec3 UnProjectMouse(const glm::vec2& mouse_position) const;

};
