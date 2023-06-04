#pragma once
#include "Camera.h"

class OrthographicCamera :
    public Camera<OrthographicCamera>
{
public:

	OrthographicCamera() {}

	void OnInit() override;
	void Present()override;

	//void SetWidth(int width) { m_Width = width; }
	//void SetHeight(int height) { m_Height = height; }
	//int GetWidth() const { return m_Width; }
	//int GetHeight() const { return m_Height; }

	void MoveForward() override;
	void MoveBackward() override;

	void MoveRight() override;
	void MoveLeft() override;

	void MoveUp() override;
	void MoveDown() override;


	virtual void Rotate(glm::vec3 rot_dir) override;
	virtual void Focus(glm::vec3 rot_dir) override;

	void OnUpdate(float ts) override;
};

