#pragma once
#include "Renderer.h"
#include <iostream>
#include "VertexData.h"

class Engine
{
private:
	ReturnStatus m_InitStatus;
	unsigned int m_ShaderProgram;
	unsigned int vao, ibo;

	EventHandler m_EventHandler;
	Renderer m_Renderer;

public:
	Engine();
	Engine(int t_width, int t_height) : 
		m_InitStatus{ ReturnStatus::INIT_BAD }, 
		m_EventHandler{},
		m_Renderer{Renderer(t_width, t_height)}{} // that's all

	~Engine() = default;

	ReturnStatus Init();
	void Run();
};

