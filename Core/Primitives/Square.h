#pragma once
#include "Mesh.h"

namespace primitives
{
    class Square : public Mesh
    {
    public:
        Square(float size = 1.0f);
    };
}