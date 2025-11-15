#pragma once
#include "Event.h"
#include "TimeStep.h"

class Layer : public event::IEventListener
{
public:
	virtual ~Layer()
	{

	};
	virtual void Enable() = 0;
	virtual void OnInit() = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void OnUpdate(TimeStep ts) = 0;
};
