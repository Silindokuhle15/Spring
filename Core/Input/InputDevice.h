#pragma once

class InputDevice
{
public:
	InputDevice() {}
	~InputDevice() {}

	virtual int CheckForRawInputSupport() = 0;
};

