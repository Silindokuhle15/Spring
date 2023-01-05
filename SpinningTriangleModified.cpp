#include "Rendering/Cube.h"
#include "Engine/BaseApplication.h"
#include "Viewing/Camera.h"
#include "Viewing/PerspectiveCamera.h"
#include "LightAndShadow/PointLight.h"
#include "Rendering/Renderer.h"



int main(void)
{
    PerspectiveCamera pCam;
    PointLight pL;
    Renderer OpenGLrenderer;

    BaseApplication::AttachCamera(&pCam);
    BaseApplication::AddPointLight(&pL);
    BaseApplication::AttachRenderer(&OpenGLrenderer);

    BaseApplication::Run();
}
