#ifndef _LAYER_H_
#define _LAYER_H_
#include "Event.h"

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
	virtual void OnUpdate(float ts) = 0;
};
#endif