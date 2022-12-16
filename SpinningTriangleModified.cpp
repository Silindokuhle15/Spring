#include "Cube.h"
#include "BaseApplication.h"
#include "Viewing/Camera.h"
#include "Viewing/OrthographicCamera.h"
#include "Viewing/PerspectiveCamera.h"
#include "ObjectLoader.h"
#include "LightAndShadow/PointLight.h"

int main(void)
{
    PerspectiveCamera pCam;
    OrthographicCamera oCam;

    PointLight pL;

    BaseApplication::AttachCamera(&pCam);
    BaseApplication::AddPointLight(&pL);

    Cube cb;
    BaseApplication::Run(&cb);
}
