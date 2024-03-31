#include "Mouse.h"

int Mouse::CheckForRawMotionSupport()
{
    return CheckForRawInputSupport();
}

int Mouse::CheckForRawInputSupport()
{
    return 0;
}