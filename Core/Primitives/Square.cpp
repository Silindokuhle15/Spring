#include "Square.h"
namespace primitives
{
    Square::Square()
        : Mesh
        {
            std::vector<glm::vec3>{ {-1.0f, -1.0f, 0.0f}, {+1.0f, -1.0f, 0.0f}, {+1.0f, +1.0f, 0.0f}, {-1.0f, +1.0f, 0.0f} },
            std::vector<glm::vec2>{ {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} },
            std::vector<float>{ 0, 1, 2, 3 },
            std::vector<glm::vec3>{ {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
            std::vector<Vertex>{
                { {+1.0f, +1.0f, 0.0f},{1.0f, 1.0f}, 2, {0.0f, 0.0f, 1.0f} },
                { {-1.0f, +1.0f, 0.0f},{0.0f, 1.0f}, 3, {0.0f, 0.0f, 1.0f} },
                { {-1.0f, -1.0f, 0.0f},{0.0f, 0.0f}, 0, {0.0f, 0.0f, 1.0f} },
                { {+1.0f, -1.0f, 0.0f},{1.0f, 0.0f}, 1, {0.0f, 0.0f, 1.0f} },
                { {+1.0f, +1.0f, 0.0f},{1.0f, 1.0f}, 2, {0.0f, 0.0f, 1.0f} },
                { {-1.0f, -1.0f, 0.0f},{0.0f, 0.0f}, 0, {0.0f, 0.0f, 1.0f} }
            },
            std::vector<uint64_t>{ 2, 3, 0, 1, 2, 0 },
            std::vector<uint64_t>{},
            std::vector<uint64_t>{},
            std::vector<Material>{}
        }
    {

    }
}