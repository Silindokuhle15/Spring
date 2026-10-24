#ifndef _SQUARE_H_
#define _SQUARE_H_
#include "Mesh.h"
namespace primitives
{
    class Square : public Mesh
    {
    public:
        Square(float size = 1.0f);
    };
}
#endif