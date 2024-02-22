#pragma once
#include "Camera.h"

/*

class PerspectiveCamera :
    public Camera<Perspective>
{
public:

	float m_Speed;

	PerspectiveCamera()	:
		m_Speed{1.0f}
	{}
    //void OnInit() override;
	//void Present() override;

	void MoveForward() override;
	void MoveBackward() override;

	void MoveRight() override;
	void MoveLeft() override;

	void MoveUp() override;
	void MoveDown() override;

	virtual void Rotate(glm::vec3 rot_dir) override;
	virtual void Focus(glm::vec3 rot_dir) override;

	void OnUpdate(float ts) override;

	glm::mat4 GetV() override;
	glm::mat4 GetVP() override;
};

*/