#include "BaseApplication.h"
#include "Application.h"

int main(void)
{
    BaseApplication springEditor{ 1920, 1080, "SprigEditor" };
    springEditor.Run();
    springEditor.ShutDown();
}
