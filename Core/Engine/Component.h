#pragma once
#include <memory>
#include <iostream>
class Component
{
public:
	virtual void OnInit() = 0;
};

using ComponentPtr = std::shared_ptr<Component>;