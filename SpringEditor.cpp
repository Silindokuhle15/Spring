#include "BaseApplication.h"

int main(void)
{
    BaseApplication springEditor{ 1920, 1080, "SpringEditor" };
    springEditor.Run();
    springEditor.ShutDown();
}
