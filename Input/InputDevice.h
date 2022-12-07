#pragma once
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

class InputDevice
{
public:
	InputDevice() {}
	~InputDevice() {}

	virtual int CheckForRawInputSupport() = 0;
};

