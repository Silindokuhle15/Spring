#include "Engine.h"

ReturnStatus Engine::Init()
{
	if (!(m_Renderer.RenderInit() == ReturnStatus::RENDER_INIT_PASS))
	{
		std::cerr << "Render Init() failed\n";
		return ReturnStatus::RENDER_INIT_FAIL;
	}

	return ReturnStatus::RENDER_INIT_PASS;
}

void Engine::Run()
{
	// 
	m_Renderer.Run();
}


Engine::Engine(): Engine(800, 600)
{
}