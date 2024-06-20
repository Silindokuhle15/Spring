#pragma once
#include <stdint.h>
class InputDevice
{
public:
	InputDevice() {}
	~InputDevice() {}

	virtual int64_t CheckForRawInputSupport() = 0;
};

