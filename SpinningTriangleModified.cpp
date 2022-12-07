#include "Cube.h"
#include "BaseApplication.h"
#include "Camera.h"
#include "ObjectLoader.h"

int main(void)
{
    Camera cam;
    BaseApplication::AttachCamera(&cam);

    Cube cb;
    BaseApplication::Run(&cb);
}
