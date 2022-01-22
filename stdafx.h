#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

enum class ReturnStatus
{
	INIT_GOOD = 0, INIT_BAD = 1,
	RENDER_INIT_PASS = 2,
	RENDER_INIT_FAIL = 3,
	INIT_TEXTURES_PASS = 4,
	INIT_TEXTURES_FAIL = 5,
	LOAD_SHADERS_FROM_FILE_SUCCESS = 6,
	LOAD_SHADERS_FROM_FILE_FAIL = 7,
	LOAD_OBJECT_FROM_FILE_SUCCESS = 8,
	LOAD_OBJECT_FROM_FILE_FAIL = 9,
	EVENT_HANDLER_INIT_SUCCESS = 10,
	EVENT_HANDLER_INIT_FAIL = 11

};