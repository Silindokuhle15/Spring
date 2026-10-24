#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_
#include <glm/glm.hpp>
#include <glm/ext.hpp>
namespace primitives
{
	struct Transform
	{
		glm::quat localOrientation;
		glm::vec3 localPosition;
		glm::quat worldOrientation;
		glm::vec3 worldPosition;
	};
}
#endif // !_TRANSFORM_H_
