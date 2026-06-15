#include "Square.h"
namespace primitives
{
    Square::Square(float size)
        : Mesh
        {
            std::vector<Vertex>{
                { {+size, +size, 0.0f},{1.0f, 1.0f}, 2, {0.0f, 0.0f, 1.0f} },
                { {-size, +size, 0.0f},{0.0f, 1.0f}, 3, {0.0f, 0.0f, 1.0f} },
                { {-size, -size, 0.0f},{0.0f, 0.0f}, 0, {0.0f, 0.0f, 1.0f} },
                { {+size, -size, 0.0f},{1.0f, 0.0f}, 1, {0.0f, 0.0f, 1.0f} },
                { {+size, +size, 0.0f},{1.0f, 1.0f}, 2, {0.0f, 0.0f, 1.0f} },
                { {-size, -size, 0.0f},{0.0f, 0.0f}, 0, {0.0f, 0.0f, 1.0f} }
            },
            std::vector<uint64_t>{ 2, 3, 0, 1, 2, 0 }
        }
    {

    }
}