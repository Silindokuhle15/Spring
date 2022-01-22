#include "Engine.h"

int main(void)
{
	Engine engine;

	if (!(engine.Init() == ReturnStatus::RENDER_INIT_PASS))
	{
		std::cerr << "Engine::Init() failed\n";
		std::getchar();
		return -1;
	}

	engine.Run();


	return 0;
}