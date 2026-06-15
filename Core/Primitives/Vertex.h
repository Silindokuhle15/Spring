#ifndef _VERTEX_H_
#define _VERTEX_H_
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace primitives
{
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 tex;
		float  ID;
		glm::vec3 norm;
	};
}

#endif // !_VERTEX_H_
