#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

namespace mu	// math utilities
{
    const vec3
        X = vec3(1, 0, 0),
        Y = vec3(0, 1, 0),
        Z = vec3(0, 0, 1),
        ZERO_3 = vec3(0);

    const float

        PI = pi<float>(),

        DEGREES_TO_RAD = PI / 180.0f,
        RAD_TO_DEGREES = 180.0f / PI;
}
