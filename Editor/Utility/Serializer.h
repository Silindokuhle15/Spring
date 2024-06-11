#pragma once
#include "Common.h"
#include "Scene.h"

class Serializer
{
public:

	Serializer() = default;

	void Serialize(Scene* scene);
	void Deserilize(const char* file_path);
};

