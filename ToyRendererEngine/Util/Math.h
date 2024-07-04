#pragma once

/*#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE*/
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>

using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using Matrix = glm::mat4;
using Quaternion = glm::quat;

struct MVPUniformBufferObject
{
    alignas(16) Matrix model;
    alignas(16) Matrix view;
    alignas(16) Matrix proj;
};
