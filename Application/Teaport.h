#pragma once
#include "Application/Application.h"
#include "Engine/ObjectLoader.h"
class Teaport : public Application
{
public:
	glm::mat4 m_Transform;

	virtual void OnInit();
};