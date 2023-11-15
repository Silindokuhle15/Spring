#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Scene.h"
#include "Application.h"

class Serializer
{
public:

	Serializer() = default;

	void Serialize(Scene* scene);
	void Deserilize(const char* file_path);
};

