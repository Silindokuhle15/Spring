#pragma once
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include "Dependencies/imgui/imgui.h"
#include "Dependencies/imgui/backends/imgui_impl_glfw.h"
#include "Dependencies/imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "glm/glm.hpp"

class Application
{
private:
public:

    virtual void OnInit();
    virtual void OnUpdate();
    virtual void OnRender();
    virtual void OnDestroy();

};



