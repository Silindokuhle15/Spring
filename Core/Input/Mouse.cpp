#include "Mouse.h"

int64_t Mouse::CheckForRawMotionSupport()
{
    return CheckForRawInputSupport();
}

int64_t Mouse::CheckForRawInputSupport()
{
    return 0;
}