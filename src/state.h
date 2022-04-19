#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Constants to help with location bindings
#define VERTEX_DATA 0
#define VERTEX_NORMAL 1
#define VERTEX_COLOR 2

#define VIEW_POS_X	0.f
#define VIEW_POS_Y	0.f
#define VIEW_POS_Z	-10.f


class State {
private:

	// allow the light to be rotated
	glm::vec4 light = glm::vec4(0.7071067811865475, 0, 0.7071067811865475, 1);
	glm::mat4 light_rotate = glm::mat4(1.0);

public:

	int render_mode = 2;

	// Methods //////////////////////////////////////////////////////////////////////////////////////////////////////////

	void rotate_light(const glm::vec3& axis, const float& amount) {
		light_rotate = glm::rotate(glm::mat4(1.0f), amount, axis) * light_rotate;
	}

	glm::vec3 getLightPos() {
		glm::vec3 L = glm::vec3(light_rotate * light);
		return glm::normalize(L);
	}
};